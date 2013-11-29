#include "Lib.h"
#include "CardInfo.h"
#include "Slash.h"
#include "Dodge.h"
#include "Peach.h"
#include "Dismantle.h"
#include "Thievery.h"
#include "Divination.h"
#include "SouthernIntruder.h"
#include "RandomShot.h"
#include "Duel.h"
#include "TimeWalk.h"
#include "Lightning.h"
#include "CiXiongSword.h"
#include "Spear.h"
#include "QiLinBow.h"
#include "OffensiveHorse.h"
#include "DefensiveHorse.h"
class DaYuan : public OffensiveHorse
{
public:
    DaYuan() : OffensiveHorse("大宛") {}
};

class ChiTu : public OffensiveHorse
{
public:
    ChiTu() : OffensiveHorse("赤兔") {}
};

class ZiXing : public OffensiveHorse
{
public:
    ZiXing() : OffensiveHorse("紫骍") {}
};

class JueYing : public DefensiveHorse
{
public:
    JueYing() : DefensiveHorse("绝影") {}
};

class ZhuaHuangFeiDian : public DefensiveHorse
{
public:
    ZhuaHuangFeiDian() : DefensiveHorse("爪黄飞电") {}
};

class DiLu : public DefensiveHorse
{
public:
    DiLu() : DefensiveHorse("的卢") {}
};

void initStarterDeck(vector<CardInfo *> &cards)
{
    CardType *slash=game->getCardType<Slash>(),
            *dodge=game->getCardType<Dodge>(),
            *peach=game->getCardType<Peach>(),
            *dismantle=game->getCardType<Dismantle>(),
            *thievery=game->getCardType<Thievery>(),
            *divination=game->getCardType<Divination>(),
            *southernIntruder=game->getCardType<SouthernIntruder>(),
            *duel=game->getCardType<Duel>(),
            *timeWalk=game->getCardType<TimeWalk>(),
            *lightning=game->getCardType<Lightning>();
    for(int r=2;r<=13;r++)
    {
        cards.push_back(new CardInfo(slash,1,r));
        cards.push_back(new CardInfo(dodge,3,r));
        cards.push_back(new CardInfo(slash,4,r));
    }
    for(int r=2;r<=7;r++)
    {
        cards.push_back(new CardInfo(game->getCardType<Spear>(),1,r));
        cards.push_back(new CardInfo(game->getCardType<QiLinBow>(),2,r));
    }
    cards.push_back(new CardInfo(peach,2,3));
    cards.push_back(new CardInfo(peach,2,4));
    cards.push_back(new CardInfo(peach,2,6));
    cards.push_back(new CardInfo(peach,2,7));
    cards.push_back(new CardInfo(peach,2,8));
    cards.push_back(new CardInfo(peach,2,9));
    cards.push_back(new CardInfo(peach,2,12));
    cards.push_back(new CardInfo(peach,3,12));
    cards.push_back(new CardInfo(dismantle,1,3));
    cards.push_back(new CardInfo(dismantle,1,4));
    cards.push_back(new CardInfo(dismantle,1,12));
    cards.push_back(new CardInfo(dismantle,2,12));
    cards.push_back(new CardInfo(dismantle,4,3));
    cards.push_back(new CardInfo(dismantle,4,4));
    cards.push_back(new CardInfo(thievery,1,3));
    cards.push_back(new CardInfo(thievery,1,4));
    cards.push_back(new CardInfo(thievery,1,11));
    cards.push_back(new CardInfo(thievery,3,3));
    cards.push_back(new CardInfo(thievery,3,4));
    cards.push_back(new CardInfo(divination,2,7));
    cards.push_back(new CardInfo(divination,2,8));
    cards.push_back(new CardInfo(divination,2,9));
    cards.push_back(new CardInfo(divination,2,11));
    cards.push_back(new CardInfo(southernIntruder,1,7));
    cards.push_back(new CardInfo(southernIntruder,1,13));
    cards.push_back(new CardInfo(southernIntruder,4,7));
    cards.push_back(new CardInfo(game->getCardType<RandomShot>(),2,1));
    cards.push_back(new CardInfo(duel,1,1));
    cards.push_back(new CardInfo(duel,3,1));
    cards.push_back(new CardInfo(duel,4,1));
    cards.push_back(new CardInfo(timeWalk,1,6));
    cards.push_back(new CardInfo(timeWalk,2,6));
    cards.push_back(new CardInfo(timeWalk,4,6));
    cards.push_back(new CardInfo(lightning,1,1));
    cards.push_back(new CardInfo(lightning,2,12));
    cards.push_back(new CardInfo(game->getCardType<DaYuan>(),1,13));
    cards.push_back(new CardInfo(game->getCardType<ChiTu>(),2,5));
    cards.push_back(new CardInfo(game->getCardType<ZiXing>(),3,13));
    cards.push_back(new CardInfo(game->getCardType<JueYing>(),1,5));
    cards.push_back(new CardInfo(game->getCardType<ZhuaHuangFeiDian>(),2,13));
    cards.push_back(new CardInfo(game->getCardType<DiLu>(),4,5));
}
