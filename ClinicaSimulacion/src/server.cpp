#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "httplib.h"

#include "models/Paciente.h"
#include "models/Medico.h"
#include "models/Cita.h"

#include "implementations/RegistroEficiente.h"
#include "implementations/SchedulerOptimizado.h"
#include "implementations/AsignadorBalanceado.h"

using namespace std;

// ========= ESTADO GLOBAL DEL SISTEMA (igual que en main.cpp) =========
RegistroEficiente registro;           // Tabla hash de pacientes
SchedulerOptimizado scheduler;        // Manejo de citas
AsignadorBalanceado asignador;       // Asignación balanceada de médicos
vector<Medico> medicosDisponibles = {
    {1, "Dr. House", "Cardiologia", 0},
    {2, "Dra. Grey", "Cirugia", 0},
    {3, "Dr. Shepherd", "Neurocirugia", 0},
    {4, "Dr. Wilson", "Cardiologia", 0}
};
int proximoIdCita = 1;

// ========= UTILIDAD PARA LEER ARCHIVOS (HTML/CSS) =========
string leer_archivo(const string &ruta) {
    ifstream file(ruta);
    if (!file.is_open()) return "";
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    httplib::Server svr;

    // ========= RUTA: PÁGINA PRINCIPAL =========
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        auto html = leer_archivo("../web/index.html");   // importante el ../
        if (html.empty()) {
            res.status = 500;
            res.set_content("No se pudo leer ../web/index.html", "text/plain; charset=UTF-8");
            return;
        }
        res.set_content(html, "text/html; charset=UTF-8");
    });

    // ========= RUTA: CSS =========
    svr.Get("/styles.css", [](const httplib::Request&, httplib::Response& res) {
        auto css = leer_archivo("../web/styles.css");
        if (css.empty()) {
            res.status = 500;
            res.set_content("No se pudo leer ../web/styles.css", "text/plain; charset=UTF-8");
            return;
        }
        res.set_content(css, "text/css; charset=UTF-8");
    });

    // ========= API: REGISTRAR PACIENTE =========
    // Body x-www-form-urlencoded: idPaciente, nombre, historial
    svr.Post("/api/registrar", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int id = stoi(req.get_param_value("idPaciente"));
            string nombre = req.get_param_value("nombre");
            string historial = req.get_param_value("historial");

            Paciente p{id, nombre, historial};
            registro.agregarPaciente(p);

            res.set_content("Paciente registrado: " + nombre, "text/plain; charset=UTF-8");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("Error al registrar paciente", "text/plain; charset=UTF-8");
        }
    });

    // ========= API: BUSCAR PACIENTE POR ID =========
    // GET /api/buscar?idPaciente=101
    svr.Get("/api/buscar", [](const httplib::Request& req, httplib::Response& res) {
        if (!req.has_param("idPaciente")) {
            res.status = 400;
            res.set_content("Falta parametro idPaciente", "text/plain; charset=UTF-8");
            return;
        }

        int id = stoi(req.get_param_value("idPaciente"));
        auto resultado = registro.buscarPaciente(id);

        if (!resultado) {
            res.status = 404;
            res.set_content("Paciente con ID " + to_string(id) + " no encontrado", "text/plain; charset=UTF-8");
            return;
        }

        auto p = *resultado;
        string respuesta = "Paciente encontrado:\n";
        respuesta += "ID: " + to_string(p.idPaciente) + "\n";
        respuesta += "Nombre: " + p.nombre + "\n";
        respuesta += "Historial: " + p.historialMedico + "\n";

        res.set_content(respuesta, "text/plain; charset=UTF-8");
    });

    // ========= API: PROGRAMAR CITA =========
    // Body: idPaciente, especialidad, fechaHora
    svr.Post("/api/programar_cita", [](const httplib::Request& req, httplib::Response& res) {
        try {
            int idPaciente = stoi(req.get_param_value("idPaciente"));
            string especialidad = req.get_param_value("especialidad");
            string fechaHora = req.get_param_value("fechaHora");

            // 1. Verificar que el paciente exista
            auto pacienteOpt = registro.buscarPaciente(idPaciente);
            if (!pacienteOpt) {
                res.status = 404;
                res.set_content("Paciente con ID " + to_string(idPaciente) + " no encontrado", "text/plain; charset=UTF-8");
                return;
            }
            auto paciente = *pacienteOpt;

            // 2. Buscar médico óptimo según especialidad y carga
            auto medicoOpt = asignador.asignarMedico(medicosDisponibles, especialidad);
            if (!medicoOpt) {
                res.status = 404;
                res.set_content("No se encontraron medicos para la especialidad '" + especialidad + "'", "text/plain; charset=UTF-8");
                return;
            }
            auto medico = *medicoOpt;

            // 3. Intentar programar la cita
            Cita nuevaCita{proximoIdCita, paciente.idPaciente, medico.idMedico, fechaHora};
            bool exito = scheduler.programarCita(nuevaCita);

            if (!exito) {
                res.status = 409;
                res.set_content("Conflicto de horario: el Dr(a). " + medico.nombre +
                                " ya tiene cita en " + fechaHora, "text/plain; charset=UTF-8");
                return;
            }

            // 4. Actualizar carga del médico
            for (auto& med : medicosDisponibles) {
                if (med.idMedico == medico.idMedico) {
                    med.cargaActual++;
                    break;
                }
            }
            proximoIdCita++;

            // 5. Respuesta de éxito
            string respuesta = "Cita programada con éxito:\n";
            respuesta += "Paciente: " + paciente.nombre + "\n";
            respuesta += "Medico: " + medico.nombre + " (" + medico.especialidad + ")\n";
            respuesta += "Fecha y hora: " + fechaHora + "\n";

            res.set_content(respuesta, "text/plain; charset=UTF-8");
        } catch (const std::exception& e) {
            res.status = 400;
            res.set_content("Error al programar cita", "text/plain; charset=UTF-8");
        }
    });

    // ========= API: VER CARGA ACTUAL DE MÉDICOS =========
    // GET /api/carga_medicos
    svr.Get("/api/carga_medicos", [](const httplib::Request&, httplib::Response& res) {
        string out = "Carga actual de médicos:\n";
        for (const auto& m : medicosDisponibles) {
            out += "- " + m.nombre + " (" + m.especialidad + "): ";
            out += to_string(m.cargaActual) + " pacientes\n";
        }
        res.set_content(out, "text/plain; charset=UTF-8");
    });

    cout << "Servidor web escuchando en http://localhost:8080\n";
    svr.listen("0.0.0.0", 8080);

    return 0;
}
