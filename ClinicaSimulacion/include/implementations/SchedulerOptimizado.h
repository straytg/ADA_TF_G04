#pragma once
#include "interfaces/ISchedulerCitas.h"
#include <unordered_map>
#include <set> // Para búsqueda O(log k)

class SchedulerOptimizado : public ISchedulerCitas {
private:
    // Key: idMedico
    // Value: Un Árbol (set) de sus horarios ocupados
    std::unordered_map<int, std::set<std::string>> citasPorMedico;

    // Almacén simple para getCitasDelMedico (se puede optimizar más)
    std::vector<Cita> listaCitas;
public:
    bool programarCita(const Cita& nuevaCita) override;
    std::vector<Cita> getCitasDelMedico(int idMedico) override;
};