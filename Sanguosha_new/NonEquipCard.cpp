#include "Lib.h"
#include "NonEquipCard.h"
#include "PrimitiveMoveEvent.h"
#include "UseEvent.h"

NonEquipCard::NonEquipCard(string name,Type t) : CardType(name,t)
{
    maxTargets=1;
}

bool NonEquipCard::use(PreUseStruct *d)
{
    vector<MoveStruct*> testData;
    Zone* targetZone=&game->resArea;//TODO: delayed trick
    int i;
    for(i=0;i<d->cards.size();i++) testData.push_back(new MoveStruct(d->cards[i],targetZone));
    d->testMove=new PrimitiveMoveEvent(testData);
    d->testMove->happen();
    if(!d->isLegal())
    {
        d->player->inform("error:illegal_use");
        d->testMove->undo();
        delete d;
        return false;
    }
    vector<Player*> targets;
    if(int max=d->getMaxTargets())
    {
        vector<Player*> choices;
        vector<string> choiceStrings;
        vector<bool> mandatory;
        for(i=game->curPlayer;i<game->curPlayer+game->nPlayer;i++)
        {
            Player *p=game->players[i%game->nPlayer];
            if(isLegalTarget(p,d))
            {
                choices.push_back(p);
                choiceStrings.push_back(p->toString());
                mandatory.push_back(isMandatoryTarget(p,d));
            }
            else if(isMandatoryTarget(p,d))
            {
                d->player->inform("error:cannot_use");
                d->testMove->undo();
                delete d;
                return false;
            }
        }
        vector<int> res=d->player->getMultiChoice("target:"+d->toString(),choiceStrings,max,mandatory);
        for(i=0;i<res.size();i++) targets.push_back(choices[res[i]]);
        //TODO: skills like 巧说
    }
    //TODO: cards like 借刀杀人
    UseStruct* data=new UseStruct(d,targets);
    if(!data->isLegal())
    {
        if(!targets.empty()) d->player->inform("error:illegal_targets");
        d->testMove->undo();
        delete data;
        return false;
    }
    d->testMove->undo();
    (new UseEvent(data))->happen();
    return true;
}
