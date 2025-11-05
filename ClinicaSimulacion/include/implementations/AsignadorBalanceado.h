#pragma once
#include "interfaces/IAsignadorPersonal.h"

class AsignadorBalanceado : public IAsignadorPersonal {
public:
    std::optional<Medico> asignarMedico(
        const std::vector<Medico>& medicosDisponibles,
        const std::string& especialidad) const override;
};