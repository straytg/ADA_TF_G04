#pragma once
#include "models/Paciente.h"
#include <optional>
#include <memory>

class IRegistroPacientes {
public:
    //Metodos virtuales puros 
    //Define que se debe poder de guadar un paciente
    virtual void agregarPaciente(const Paciente& paciente) = 0;
    //Deffine que se debe poder buscar un paciente por su ID
    virtual std::optional<Paciente> buscarPaciente(int idPaciente) = 0;
    //Destructor virtual
    //si se borra un puntero de la interfaz se borre de la clase derivada
    virtual ~IRegistroPacientes() = default; // Destructor virtual
};