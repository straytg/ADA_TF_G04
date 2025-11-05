#pragma once
#include "models/Cita.h"
#include <vector>

class ISchedulerCitas {
public:
    virtual bool programarCita(const Cita& nuevaCita) = 0;
    virtual std::vector<Cita> getCitasDelMedico(int idMedico) = 0;
    virtual ~ISchedulerCitas() = default;
};