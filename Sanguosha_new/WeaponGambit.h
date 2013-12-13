#ifndef WEAPONGAMBIT_H
#define WEAPONGAMBIT_H

#include "Lib.h"
#include "NonEquipCard.h"

class WeaponGambit : public NonEquipCard
{
    bool legalityCheck(Player *target, PreUseStruct *d);
public:
    WeaponGambit();
    void resolve(TargetStruct *target, UseStruct *d);
    bool extraChoice(UseStruct *d);
};

#endif // WEAPONGAMBIT_H
