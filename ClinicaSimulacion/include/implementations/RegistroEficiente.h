#pragma once
#include "interfaces/IRegistroPacientes.h"
#include <unordered_map> // Tabla Hash

class RegistroEficiente : public IRegistroPacientes {
private:
    std::unordered_map<int, Paciente> mapaPacientes;
public:
    void agregarPaciente(const Paciente& paciente) override;
    std::optional<Paciente> buscarPaciente(int idPaciente) override;
};