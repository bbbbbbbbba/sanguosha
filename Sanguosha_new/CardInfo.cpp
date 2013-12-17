#include "Lib.h"
#include "CardInfo.h"
#include "CardType.h"
string CardInfo::rankString[14]={"","A","2","3","4","5","6","7","8","9","10","J","Q","K"};
string CardInfo::suitString[7]={"","黑桃","红桃","方块","梅花","黑色","红色"};

CardInfo::CardInfo(CardType *n,int s,int r)
{
    name=n;suit=s;rank=r;
}

CardInfo::CardInfo(CardType *n, vector<Card *> cards)
{
    name=n;suit=rank=0;
    if(!cards.empty())
    {
        CardInfo info0=cards[0]->getCardInfo();
        if(cards.size()==1)
        {
            suit=info0.suit;rank=info0.rank;
        }
        else if(info0.isBlack())
        {
            suit=5;
            for(int k=1;k<cards.size();k++) if(!cards[k]->getCardInfo().isBlack()) {suit=0;break;}
        }
        else
        {
            suit=6;
            for(int k=0;k<cards.size();k++) if(!cards[k]->getCardInfo().isRed()) {suit=0;break;}
        }
    }
}

bool CardInfo::isBlack()
{
    return suit==1||suit==4||suit==5;
}

bool CardInfo::isRed()
{
    return suit==2||suit==3||suit==6;
}

string CardInfo::toString()
{
    return suitString[suit]+rankString[rank]+"["+name->name+"]";
}
