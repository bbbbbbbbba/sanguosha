#include "Game.h"
#include "MoveEvent.h"
#include "Cheat.h"
#include "YieldEvent.h"
#include "NeedYieldEvent.h"
#include "PrimitiveMoveEvent.h"
#include <map>
Game *game;

void initStarterDeck(vector<CardInfo*> &cards);

Game::Game(const vector<ClientPlayer*> &clients, bool cheatMode):
    deck("deck",NULL,false),discardPile("discardPile"),resArea("resArea")
{
	try
	{
        game=this;
		nPlayer=clients.size();
		int i,pc=0;
        vector<CardInfo*> cards;
        vector<Role> roles;
        roles.push_back(Renegade);
        initStarterDeck(cards);
        random_shuffle(cards.begin(),cards.end());
        roles.push_back(Lord);
        for(i=0;i<(nPlayer-3)/2;i++) roles.push_back(Loyalist);
        for(i=0;i<nPlayer/2;i++) roles.push_back(Rebel);
        random_shuffle(roles.begin(),roles.end());
        for(i=0;i<nPlayer;i++)
        {
            ostringstream s;
            s<<"role:"<<roles[i];
            clients[i]->inform(s.str());
        }
        int lordId;
        for(i=0;i<nPlayer;i++) if(roles[i]==Lord) lordId=i;
        for(i=0;i<nPlayer;i++)
        {
            ostringstream s;
            s<<"lord:"<<lordId;
            clients[i]->inform(s.str());
        }
        //TODO: choose generals
        for(i=0;i<nPlayer;i++)
        {
            players.push_back(new Player(clients[i],roles[i]));
            if(i>0) players[i-1]->nextPlayer=players[i];
        }
        players[nPlayer-1]->nextPlayer=players[0];
        curPlayer=lordId;
        //TODO: register triggers
        if(cheatMode) for(i=0;i<nPlayer;i++)
        {
            Player *p=players[i];
            (new Cheat(p))->init();
        }
        handleTiming(revealGeneral,NULL);
        handleTiming(initHandCard,NULL);
		for(i=0;i<nPlayer;i++) for(int j=0;j<4;j++) players[i]->hand.push_back(cards[pc++]);
		for(i=0;i<nPlayer;i++)
		{
			Player *p=players[i];
            string s="init_hand:";
            for(Card* n=p->hand.next;n!=&p->hand;n=n->next) s+=n->getPhysicalCardInfo().toString()+";";
            p->inform(s);
		}
		while(pc<cards.size()) deck.push_back(cards[pc++]);
        handleTiming(gameStart,NULL);
		while(1)
		{
            (new TurnEvent(players[curPlayer]))->happen();
            //TODO: extra turns
			do curPlayer=(curPlayer+1)%nPlayer; while(!players[curPlayer]->alive);
		}
	}
    catch(GameOverException &e)
	{
        cout<<"Game is over!\nResult: "<<e.result<<"\n";
        delete this;
	}
}

Game::~Game()
{
	int i;
	for(i=0;i<nPlayer;i++) delete players[i];
    for(i=0;i<timeline.size();i++) delete timeline[i];
    for(i=0;i<cardTypes.size();i++) delete cardTypes[i];
}

void Game::handleTiming(Timing t,Event *e,int i)
{
    if(i<0) i=curPlayer;
    for(;i<curPlayer+nPlayer;i++)
	{
		Player *p=players[i%nPlayer];
        if(t==doMainPhase&&p!=dynamic_cast<PhaseEvent*>(e)->player) continue;
        for(int j=0;j<3;j++)
        {
            if(t==doMainPhase&&j<2) continue;
            map<Trigger*,vector<void*> > triggerDone;
            bool cardUsed=false;
            triggerDone.clear();
            do
            {
                vector<pair<Trigger*,void*> > triggers;
                vector<Card*> cards;
                vector<Card*> yields;
                vector<pair<Transform*,CardType*> > transforms_use, transforms_yield;
                vector<string> choices;
                triggers.clear();
                cards.clear();
                choices.clear();
                transforms_use.clear();
                transforms_yield.clear();
                bool cancelable=true,autoable=true;
                int k;
                for(int j2=0;j2<3;j2++)
                {
                    if(j2!=j&&t!=doMainPhase) continue;
                    for(k=0;k<p->triggerTable[t][j2].size();k++)
                    {
                        Trigger *tr=p->triggerTable[t][j2][k];
                        vector<void*> t=tr->getTrigger(e,triggerDone[tr]);
                        if(!t.empty())
                        {
                            for(int l=0;l<t.size();l++)
                            {
                                triggers.push_back(make_pair(tr,t[l]));
                                choices.push_back(tr->toString(e,t[l]));
                            }
                            if(tr->isMandatory) cancelable=false;
                        }
                    }
                }
                if(t==doMainPhase||j==2&&!cardUsed)
                {
                    for(Card *c=p->hand.next;c!=&p->hand;c=c->next) if(p->canUse(c,t,e))
                    {
                        cards.push_back(c);
                        choices.push_back(c->getPhysicalCardInfo().toString());
                    }
                    for(k=0;k<cardTypes.size();k++)
                    {
                        CardType *type=cardTypes[k];
                        if(type->legalToUse(t,e,p))
                        {
                            autoable=false;
                            for(int kk=0;kk<p->transformTable[type].size();kk++)
                            {
                                Transform *trans=p->transformTable[type][kk];
                                if(trans->canTransform(type,e,transformUse))
                                {
                                    transforms_use.push_back(make_pair(trans,type));
                                    choices.push_back(trans->toString(type));
                                }
                            }
                        }
                    }
                    NeedYieldEvent *need;
                    if(t==needYield&&(need=dynamic_cast<NeedYieldEvent*>(e))->player==p)
                    {
                        autoable=false;
                        for(Card *c=p->hand.next;c!=&p->hand;c=c->next) if(p->canYield(c,e))
                        {
                            yields.push_back(c);
                            choices.push_back(c->getPhysicalCardInfo().toString());
                        }
                        for(k=0;k<cardTypes.size();k++)
                        {
                            CardType *type=cardTypes[k];
                            if(need->filter->filter(CardInfo(type,0,0))) //TODO: potential oddballs like "need yield a red slash"
                            {
                                for(int kk=0;kk<p->transformTable[type].size();kk++)
                                {
                                    Transform *trans=p->transformTable[type][kk];
                                    if(trans->canTransform(type,e,transformYield))
                                    {
                                        transforms_yield.push_back(make_pair(trans,type));
                                        choices.push_back(trans->toString(type));
                                    }
                                }
                            }
                        }
                    }
                }
                if(cancelable) choices.push_back("Cancel");
                int x=0;
                if(!autoable||choices.size()>1)
                {
                    ostringstream s;
                    s<<"response:"<<t<<":";
                    if(e) s<<e->toString(p);
                    x=p->getChoice(s.str(),choices);
                }
                if(x<triggers.size())
                {
                    pair<Trigger*,void*> tr=triggers[x];
                    if(tr.first->trigger(e,tr.second)) triggerDone[tr.first].push_back(tr.second);
                }
                else if((x-=triggers.size())<cards.size())
                {
                    Card *c=cards[x];
                    PreUseStruct *d=new PreUseStruct(p,c,t,e);
                    if(d->use()) cardUsed=true;
                }
                else if((x-=cards.size())<transforms_use.size())
                {
                    pair<Transform*,CardType*> trans=transforms_use[x];
                    vector<Card*> c=trans.first->chooseCards(trans.second,e);
                    if(!c.empty())
                    {
                        PreUseStruct *d=new PreUseStruct(p,c,CardInfo(trans.second,c),t,e,trans.first);
                        if(d->use()) cardUsed=true;
                    }
                }
                else if((x-=transforms_use.size())<yields.size())
                {
                    Card *c=yields[x];
                    (new YieldEvent(p,c,e))->happen();
                    cardUsed=true;
                }
                else if((x-=yields.size())<transforms_yield.size())
                {
                    pair<Transform*,CardType*> trans=transforms_yield[x];
                    vector<Card*> c=trans.first->chooseCards(trans.second,e);
                    if(!c.empty())
                    {
                        (new YieldEvent(p,c,CardInfo(trans.second,c),e,trans.first))->happen();
                        cardUsed=true;
                    }
                }
                else break;
                if(!e->curTiming)
                {
                    e->curTiming=nTimings;
                    throw DisturbedException(i);
                }
            }
            while(1);
        }
    }
}

void Game::applyStatics(StaticValue sv, void *value, void *data)
{
    for(int i=0;i<3;i++) for(int j=0;j<staticTable[sv][i].size();j++) staticTable[sv][i][j]->apply(value,data);
}

void Game::doPrimitive(PrimitiveEvent *pe)
{
	game->timeline.push_back(pe);
    for(int i=0;i<nPlayer;i++) players[i]->inform(pe->toString(players[i]));
    pe->happen();
}

void Game::shuffle()
{
	vector<Card*> cards;
    vector<MoveStruct*> d;
    for(Card *c=discardPile.next;c!=&discardPile;c=c->next)
    {
        cards.push_back(c);
        d.push_back(new MoveStruct(c,&deck));
    }
    (new MoveEvent(d))->happen();
    d.clear();
    random_shuffle(cards.begin(),cards.end());
	for(int i=0;i<cards.size();i++) d.push_back(new MoveStruct(cards[i],&deck));
    doPrimitive(new PrimitiveMoveEvent(d));
}

vector<Card*> Game::getTopCards(int n)
{
	vector<Card*> res;
	if(deck.size<n)
    {
		if(deck.size+discardPile.size<n)
		{
            throw GameOverException();
		}
		else shuffle();
    }
	Card* c=deck.next;
	for(int i=0;i<n;i++)
	{
		res.push_back(c);
		c=c->next;
	}
	return res;
}
