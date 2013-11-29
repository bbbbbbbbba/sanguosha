#ifndef SOUTHERNINTRUDER_H
#define SOUTHERNINTRUDER_H

#include "Lib.h"
#include "NonEquipCard.h"

class SouthernIntruder : public NonEquipCard
{
    bool legalityCheck(Player *target, PreUseStruct *d);
public:
    SouthernIntruder();
    void resolve(TargetStruct *target, UseStruct *d);
    bool isMandatoryTarget(Player *target, PreUseStruct *d);
};

#endif // SOUTHERNINTRUDER_H
