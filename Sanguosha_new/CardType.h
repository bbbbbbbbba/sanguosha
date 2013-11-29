#ifndef CARDTYPE_H
#define CARDTYPE_H

#include "Lib.h"
#include "PreUseStruct.h"
class Player;
class Card;

enum Type
{
    Basic,
    Trick,
    Equip
};

class CardType
{
protected:
    virtual bool legalityCheck(Player *target,PreUseStruct *d)=0;
    virtual bool distanceCheck(Player *target,PreUseStruct *d);
    virtual bool canBeUsed(Timing t, Event *e, Player *p);
public:
    int maxTargets;
    string name;
    Type type;
    bool isDelayedTrick;
    CardType(string n,Type t);
    virtual ~CardType();
    bool legalToUse(Timing t, Event *e, Player *p);
    virtual bool use(PreUseStruct *d)=0;
    bool isLegalTarget(Player *target,PreUseStruct *d);
    virtual bool isMandatoryTarget(Player *target, PreUseStruct *d);
};

#endif // CARDTYPE_H
