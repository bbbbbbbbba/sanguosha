#include "Lib.h"
#include "Cheat.h"
#include "DamageEvent.h"

Cheat::Cheat(Player *p) : Trigger(p)
{
    timings.push_back(doMainPhase);
}

vector<void *> Cheat::getTrigger(Event *e, vector<void*> past)
{
    return vector<void*>(1,(void*)NULL);
}

bool Cheat::trigger(Event *e, void* index)
{
    vector<string> choices;
    int i,v;
    for(i=0;i<game->nPlayer;i++) choices.push_back(game->players[i]->toString());
    i=player->getChoice("choose a player to damage",choices);
    cout<<"How many damage? ";
    cin>>v;
    (new DamageEvent(game->players[i],player,v))->happen();
    return true;
}

string Cheat::toString(Event *e, void *index)
{
    return "作弊";
}
