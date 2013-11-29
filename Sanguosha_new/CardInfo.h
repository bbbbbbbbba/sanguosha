/*
 * CardInfo.h
 * 
 */
#ifndef _CARDINFO_H
#define _CARDINFO_H

#include "Lib.h"
class CardType;
class Card;

class CardInfo
{
public:
    int suit,rank;
    CardType *name;
    static string rankString[14],suitString[7];
    CardInfo(CardType *n,int s,int r);
    CardInfo(CardType *n, vector<Card*> cards);
    bool isBlack();
    bool isRed();
    string toString();
};

#endif
