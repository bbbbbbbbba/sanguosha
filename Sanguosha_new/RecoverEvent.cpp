#include "Lib.h"
#include "RecoverEvent.h"
#include "DyingEvent.h"

void RecoverEvent::execute()
{
    //TODO: skills like 救援
    if(value>player->maxHp-player->hp) value=player->maxHp-player->hp;
    if(!player->alive||value<=0) return;
    player->hp+=value;
    cout<<player->toString()<<"'s HP: "<<player->hp<<"/"<<player->maxHp<<"\n";//debug
    onTiming(afterRecover);
    for(int i=0;i<game->stack.size();i++)
        if(DyingEvent *d=dynamic_cast<DyingEvent*>(game->stack[i])) d->curTiming=nullTiming;
}

RecoverEvent::RecoverEvent(Player *p, int v)
{
    player=p;value=v;
}
