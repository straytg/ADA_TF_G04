#include "implementations/RegistroEficiente.h"

void RegistroEficiente::agregarPaciente(const Paciente& paciente) {
    // Inserción en Tabla Hash: O(1) en promedio
    mapaPacientes[paciente.idPaciente] = paciente;
}

std::optional<Paciente> RegistroEficiente::buscarPaciente(int idPaciente) {
    // Búsqueda en Tabla Hash: O(1) en promedio
    auto it = mapaPacientes.find(idPaciente);
    if (it != mapaPacientes.end()) {
        // it->second es el valor (el Paciente)
        return it->second;
    }
    return std::nullopt; // No se encontró
}