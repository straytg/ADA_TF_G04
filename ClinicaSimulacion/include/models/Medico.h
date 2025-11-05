#pragma once
#include <string>

struct Medico {
    int idMedico;
    std::string nombre;
    std::string especialidad; // ej. "Cardiologia", "Pediatria", "Cirugia"
    int cargaActual = 0;
};