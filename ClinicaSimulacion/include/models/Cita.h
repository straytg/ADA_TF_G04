#pragma once
#include <string>

struct Cita {
    int idCita;
    int idPaciente;
    int idMedico;
    std::string fechaHora; // Formato "YYYY-MM-DD HH:MM"
};