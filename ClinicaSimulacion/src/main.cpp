#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <limits>

// Incluimos todos los modelos y las implementaciones CONCRETAS
#include "models/Paciente.h"
#include "models/Medico.h"
#include "models/Cita.h"

#include "implementations/RegistroEficiente.h"
#include "implementations/SchedulerOptimizado.h"
#include "implementations/AsignadorBalanceado.h"

using namespace std;

// --- Función Auxiliar para limpiar la consola (simulación) ---
void limpiarPantalla() {
    for (int i = 0; i < 50; ++i) {
        cout << endl;
    }
}

// --- Función Auxiliar para esperar al usuario ---
void esperarUsuario() {
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- Función Auxiliar para leer un entero de forma segura ---
int leerOpcion() {
    int opcion;
    while (!(cin >> opcion)) {
        cout << "Error: Debes ingresar un número. Intenta de nuevo: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return opcion;
}

// --- Función Auxiliar para leer texto ---
string leerTexto() {
    string texto;
    getline(cin, texto);
    return texto;
}


int main() {
    // --- 1. INICIALIZACIÓN DEL SISTEMA ---

    shared_ptr<IAsignadorPersonal> asignador =
        make_shared<AsignadorBalanceado>();

    shared_ptr<IRegistroPacientes> registro =
        make_shared<RegistroEficiente>();
    shared_ptr<ISchedulerCitas> scheduler =
        make_shared<SchedulerOptimizado>();

    vector<Medico> medicosDisponibles = {
        {1, "Dr. House", "Cardiologia", 0},
        {2, "Dra. Grey", "Cirugia", 0},
        {3, "Dr. Shepherd", "Neurocirugia", 0},
        {4, "Dr. Wilson", "Cardiologia", 0}
    };

    registro->agregarPaciente({101, "Paciente A", "Historial A..."});
    registro->agregarPaciente({102, "Paciente B", "Historial B..."});

    int proximoIdCita = 1;

    // --- 2. BUCLE PRINCIPAL DE LA APLICACIÓN (FRONTEND DE CONSOLA) ---
    bool corriendo = true;
    while (corriendo) {
        limpiarPantalla();
        cout << "=========================================" << endl;
        cout << "  SISTEMA DE GESTION - CLINICA RICARDO PALMA" << endl;
        cout << "=========================================" << endl;
        cout << "\n--- Modulo: Registro ---" << endl;
        cout << "  1. Registrar nuevo paciente" << endl;
        cout << "  2. Buscar paciente por ID" << endl;
        cout << "\n--- Modulo: Citas y Asignacion ---" << endl;
        cout << "  3. Programar Cita (con Asignacion Optima)" << endl;
        cout << "  4. Ver carga actual de medicos" << endl;
        cout << "\n  0. Salir" << endl;
        cout << "\nSelecciona una opcion: ";

        int opcion = leerOpcion();

        switch (opcion) {
            case 1: { // REGISTRAR PACIENTE
                limpiarPantalla();
                cout << "--- 1. Registrar Nuevo Paciente ---" << endl;
                Paciente nuevoPaciente;
                cout << "Ingresa ID (numerico): ";
                nuevoPaciente.idPaciente = leerOpcion();

                cout << "Ingresa Nombre: ";
                nuevoPaciente.nombre = leerTexto();

                cout << "Ingresa Historial: ";
                nuevoPaciente.historialMedico = leerTexto();

                registro->agregarPaciente(nuevoPaciente);
                cout << "\n¡Paciente '" << nuevoPaciente.nombre << "' registrado con exito!" << endl;
                esperarUsuario();
                break;
            }
            case 2: { // BUSCAR PACIENTE
                limpiarPantalla();
                cout << "--- 2. Buscar Paciente por ID ---" << endl;
                cout << "Ingresa ID del paciente a buscar: ";
                int idBuscar = leerOpcion();

                auto resultado = registro->buscarPaciente(idBuscar);
                if (resultado) {
                    cout << "\nPaciente Encontrado:" << endl;
                    cout << "  Nombre: " << resultado->nombre << endl;
                    cout << "  Historial: " << resultado->historialMedico << endl;
                } else {
                    cout << "\nError: Paciente con ID " << idBuscar << " no encontrado." << endl;
                }
                esperarUsuario();
                break;
            }
            case 3: { // PROGRAMAR CITA (FLUJO COMPLETO)
                limpiarPantalla();
                cout << "--- 3. Programar Cita (con Asignación Óptima) ---" << endl;

                cout << "Ingresa ID del Paciente: ";
                int idPaciente = leerOpcion();
                auto pacienteOpt = registro->buscarPaciente(idPaciente);

                if (!pacienteOpt) {
                    cout << "\nError: Paciente con ID " << idPaciente << " no encontrado." << endl;
                    esperarUsuario();
                    break;
                }
                auto paciente = *pacienteOpt;
                cout << "  Paciente: " << paciente.nombre << endl;

                cout << "Ingresa Especialidad (Ej: Cardiologia, Cirugia): ";
                string especialidad = leerTexto();

                cout << "Ingresa Fecha y Hora (Ej: 2025-11-05 10:00): ";
                string fechaHora = leerTexto();

                auto medicoOpt = asignador->asignarMedico(medicosDisponibles, especialidad);

                if (!medicoOpt) {
                    cout << "\nError: No se encontraron medicos para la especialidad '" << especialidad << "'." << endl;
                    esperarUsuario();
                    break;
                }
                auto medico = *medicoOpt;
                cout << "  Medico óptimo encontrado (por carga): " << medico.nombre << endl;

                Cita nuevaCita = {proximoIdCita, paciente.idPaciente, medico.idMedico, fechaHora};

                bool exito = scheduler->programarCita(nuevaCita);

                if (exito) {
                    for (auto& med : medicosDisponibles) {
                        if (med.idMedico == medico.idMedico) {
                            med.cargaActual++;
                            break;
                        }
                    }
                    proximoIdCita++;
                    cout << "\n¡Cita programada con éxito!" << endl;
                    cout << "  Paciente: " << paciente.nombre << endl;
                    cout << "  Médico: " << medico.nombre << endl;
                    cout << "  Fecha: " << fechaHora << endl;
                } else {
                    cout << "\nError: ¡Conflicto de Horario!" << endl;
                    cout << "El Dr. " << medico.nombre << " ya tiene una cita a las " << fechaHora << "." << endl;
                }
                esperarUsuario();
                break;
            }
            case 4: { // VER CARGA DE MÉDICOS
                limpiarPantalla();
                cout << "--- 4. Carga Actual de Medicos ---" << endl;
                for (const auto& medico : medicosDisponibles) {
                    cout << "  - " << medico.nombre
                              << " (" << medico.especialidad << "): "
                              << medico.cargaActual << " pacientes" << endl;
                }
                esperarUsuario();
                break;
            }
            case 0: { // SALIR
                cout << "Cerrando el sistema..." << endl;
                corriendo = false;
                break;
            }
            default: {
                cout << "Opcion no valida. Intentalo de nuevo." << endl;
                esperarUsuario();
                break;
            }
        } // fin del switch
    } // fin del while

    return 0;
}