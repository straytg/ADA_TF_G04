#include "implementations/AsignadorBalanceado.h"
#include <limits> // Necesario para numeric_limits

using namespace std; // <-- AÑADIDO

optional<Medico> AsignadorBalanceado::asignarMedico(
    const vector<Medico>& medicosDisponibles,
    const string& especialidad) const {

    int minCarga = numeric_limits<int>::max();
    optional<Medico> medicoSeleccionado = nullopt;

    for (const auto& medico : medicosDisponibles) {
        if (medico.especialidad == especialidad) {
            if (medico.cargaActual < minCarga) {
                minCarga = medico.cargaActual;
                medicoSeleccionado = medico;
            }
        }
    }

    return medicoSeleccionado;
}