#include "Lib.h"
#include "ResolveEvent.h"
#include "NonEquipCard.h"

void ResolveEvent::execute()
{
    if(!target||target->valid)
    {
        bool effective=true;
        game->applyStatics(useEffectiveness,&effective,this);
        if(effective)
            {
            countered=false;
            onTiming(beforeResolve);
            if(!countered) dynamic_cast<NonEquipCard*>(data->data->info.name)->resolve(target,data);//TODO
        }
        else for(int i=0;i<game->nPlayer;i++)
            game->players[i]->inform("non-effective:"+toString(game->players[i]));
    }
}

ResolveEvent::ResolveEvent(UseStruct *d, TargetStruct *t) : countered(false)
{
    data=d;target=t;
}

string ResolveEvent::toString(Player *p)
{
    string res="resolve:";
    if(data->data->player) res+=data->data->player->toString();
    res+=":"+data->data->info.toString()+":";
    if(target) res+=target->toString();
    return res;
}

