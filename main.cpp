#include "Game.h"
#include "ConsolePlayer.h"
#include <cstdio>

int main(int argc, char **argv)
{
	int i;
	srand(time(0));
	vector<ClientPlayer*> p;
    for(i=0;i<8;i++)
    {
        char buf[100];
        sprintf(buf,"Player %d",i);
        p.push_back(new ConsolePlayer(buf));
    }
    new Game(p,true);
	for(i=0;i<8;i++) delete p[i];
	return 0;
}
