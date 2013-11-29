#include "Lib.h"
#include "Card.h"

Card::Card(){}

Card::Card(CardInfo *c, Card *p)
{
	info=c;insert(p);
}

Card::~Card()
{
	if(info) delete info;
    erase();
}

CardInfo Card::getCardInfo()
{
    return *info;//TODO: Skills like 红颜
}

void Card::erase()
{
	zone->size--;prev->next=next;next->prev=prev;
}

void Card::insert(Card *p)
{
	zone=p->zone;zone->size++;prev=p;next=p->next;p->next=next->prev=this;
}

void Card::move(Card *p)
{
    if(p!=this) {erase();insert(p);}
}

bool Card::isVisibleTo(Player *p)
{
    bool res=zone->isVisibleTo(p);//TODO:face down
    pair<Card*, Player*> data=make_pair(this,p);
    game->applyStatics(visibilityValue,&res,&data);
    return res;
}

string Card::toString(Player *p)
{
    string res="";
    if(isVisibleTo(p)) res+=getPhysicalCardInfo().toString();
    else res+="hidden";
    res+="@";
    res+=zone->toString();
    return res;
}
