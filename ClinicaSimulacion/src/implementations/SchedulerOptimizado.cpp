#include "implementations/SchedulerOptimizado.h"

using namespace std;

bool SchedulerOptimizado::programarCita(const Cita& nuevaCita) {
    auto& horariosOcupados = citasPorMedico[nuevaCita.idMedico];

    if (horariosOcupados.count(nuevaCita.fechaHora)) {
        return false;
    }

    horariosOcupados.insert(nuevaCita.fechaHora);
    listaCitas.push_back(nuevaCita);
    return true;
}

vector<Cita> SchedulerOptimizado::getCitasDelMedico(int idMedico) { // sin std::
    vector<Cita> citasMedico; // sin std::
    for (const auto& cita : listaCitas) {
        if (cita.idMedico == idMedico) {
            citasMedico.push_back(cita);
        }
    }
    return citasMedico;
}