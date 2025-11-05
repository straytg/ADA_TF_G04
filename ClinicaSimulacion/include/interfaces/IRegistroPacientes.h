#pragma once
#include "models/Paciente.h"
#include <optional>
#include <memory>

class IRegistroPacientes {
public:
    virtual void agregarPaciente(const Paciente& paciente) = 0;
    virtual std::optional<Paciente> buscarPaciente(int idPaciente) = 0;
    virtual ~IRegistroPacientes() = default; // Destructor virtual
};