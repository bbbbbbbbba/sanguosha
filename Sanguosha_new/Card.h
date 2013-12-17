/*
 * Card.h
 * 
 */
#ifndef _CARD_H
#define _CARD_H

#include "Lib.h"
#include "CardInfo.h"
#include "SkillEffect.h"

class Card
{
protected:
    CardInfo *info;
public:
    Zone* zone;
	Card* prev;
	Card* next;
    vector<SkillEffect*> skills;
	Card();
    Card(CardInfo *c,Card *p);
	virtual ~Card();
    CardInfo getCardInfo();
    inline CardInfo getPhysicalCardInfo() {return *info;}
	void erase();
	void insert(Card *p);
	void move(Card *p);
    bool isVisibleTo(Player *p);
    string toString(Player *p);
};

#endif
