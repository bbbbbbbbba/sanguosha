#include "ConsolePlayer.h"

string ConsolePlayer::roleString[4]={"主公","忠臣","反贼","内奸"};
string ConsolePlayer::natureString[3]={"","雷属性","火属性"};

ConsolePlayer::ConsolePlayer(string n)
{
    name=n;
}

string ConsolePlayer::parseZone(string s)
{
    if(s=="deck") return "牌堆";
    if(s=="resArea") return "处理区";
    if(s=="discardPile") return "弃牌堆";
    int i=s.find_first_of(':');
    string head=s.substr(0,i),tail=s.substr(i+1);
    if(head=="hand") return tail+"的手牌";
    if(head=="equipArea") return tail+"的装备区";
    if(head=="judgeArea") return tail+"的判定区";
    if(head=="onGeneral") return tail+"武将牌上";
    if(head=="byGeneral") return tail+"武将牌旁";
    return s;
}

void ConsolePlayer::inform(string message)
{
    if(message.length())
    {
        cout<<"["<<name<<"] ";
        int i=message.find_first_of(':');
        string head=message.substr(0,i),tail=message.substr(i+1);
        if(head=="role") cout<<"你的角色是"<<roleString[atoi(tail.c_str())];
        else if(head=="lord") cout<<"主公是玩家"<<tail;
        else if(head=="init_hand")
        {
            cout<<"你的起始手牌是";
            while((i=tail.find_first_of(';'))!=string::npos)
            {
                cout<<tail.substr(0,i);
                tail=tail.substr(i+1);
            }
        }
        else if(head=="move")
        {
            while((i=tail.find_first_of(';'))!=string::npos)
            {
                int j=tail.find_first_of('-'),k=tail.find_first_of('@');
                string card=tail.substr(0,k),src=tail.substr(k+1,j-k-1),dest=tail.substr(j+1,i-j-1);
                if(card=="hidden") card="一张牌";
                cout<<parseZone(src)<<"的"<<card<<"移动到"<<parseZone(dest);
                tail=tail.substr(i+1);
                if(!tail.empty()) cout<<"，";
            }
        }
        else if(head=="damage")
        {
            i=tail.find_first_of(':');
            string source=tail.substr(0,i);
            tail=tail.substr(i+1);
            i=tail.find_first_of(':');
            string player=tail.substr(0,i);
            tail=tail.substr(i+1);
            i=tail.find_first_of(':');
            int value=atoi(tail.substr(0,i).c_str()),nature=atoi(tail.substr(i+1).c_str());
            if(source.empty()) cout<<player<<"受到";
            else cout<<source<<"对"<<player<<"造成";
            cout<<value<<"点"<<natureString[nature]<<"伤害";
        }
        else if(head=="hp")
        {
            i=tail.find_first_of(':');
            string player=tail.substr(0,i);
            tail=tail.substr(i+1);
            cout<<player<<"的体力："<<tail;
        }
        else if(head=="dying")
        {
            cout<<tail<<"进入濒死状态";
        }
        else if(head=="death")
        {
            cout<<tail<<"死亡";
        }
        else if(head=="death_role")
        {
            i=tail.find_first_of(':');
            cout<<tail.substr(0,i)<<"的身份是"<<roleString[atoi(tail.substr(i+1).c_str())];
        }
        else if(head=="use")
        {
            i=tail.find_first_of(':');
            string player=tail.substr(0,i);
            tail=tail.substr(i+1);
            i=tail.find_first_of(':');
            string card=tail.substr(0,i);
            tail=tail.substr(i+1);
            i=card.find_first_of('<');
            if(i==string::npos) cout<<player<<"使用了"<<card;
            else
            {
                string info=card.substr(0,i),trans=card.substr(i+1);
                i=trans.find_first_of('<');
                string list=trans.substr(i+1);
                trans=trans.substr(0,i);
                cout<<player<<"发动"<<trans<<"将";
                while((i=list.find_first_of(';'))!=string::npos)
                {
                    cout<<list.substr(0,i);
                    list=list.substr(i+1);
                    if(!list.empty()) cout<<"、";
                }
                cout<<"当"<<info<<"使用";
            }
            if(!tail.empty())
            {
                cout<<"，目标是";
                while((i=tail.find_first_of(';'))!=string::npos)
                {
                    cout<<tail.substr(0,i);
                    tail=tail.substr(i+1);
                    if(!tail.empty()) cout<<"，";
                }
            }
        }
        else if(head=="resolve")
        {
            i=tail.find_first_of(':');
            string player=tail.substr(0,i);
            tail=tail.substr(i+1);
            i=tail.find_first_of(':');
            string card=tail.substr(0,i);
            tail=tail.substr(i+1);
            if(!player.empty()) cout<<player<<"使用的";
            cout<<card;
            if(!tail.empty()) cout<<"对"<<tail;
            cout<<"结算";
        }
        else if(head=="error")
        {
            if(tail=="cannot_use") cout<<"没有合理的目标！";
            else if(tail=="illegal_target") cout<<"目标不合法！";
        }
        else cout<<message;
        cout<<endl;
    }
}

int ConsolePlayer::getChoice(string reason, const vector<string> &choices)
{
	int res;
    cout<<"["<<name<<"] ";
    int i=reason.find_first_of(':');
    string head=reason.substr(0,i),tail=reason.substr(i+1);
    if(head=="response")
    {
        i=tail.find_first_of(':');
        int timing=atoi(tail.substr(0,i).c_str());
        string event=tail.substr(i+1);
        if(timing==12) cout<<"出牌阶段，";
        if(timing==21) cout<<"牌生效前，";
        if(timing==34) cout<<"处于濒死状态，";
        cout<<"请响应：";
    }
    else if(head=="card")
    {
        i=tail.find_first_of(':');
        string head2=tail.substr(0,i),tail2=tail.substr(i+1);
        if(head2=="discard")
        {
            if(tail2=="discard_phase") cout<<"弃牌阶段，";
            else cout<<tail2<<"，";
            cout<<"请弃牌：";
        }
        else cout<<"请选择牌：";
    }
    else if(head=="target")
    {
        cout<<"请选择目标：";
    }
    else cout<<"please choose for <"<<reason<<">";
    cout<<"\n";
    for(int i=0;i<choices.size();i++)
    {
        cout<<i<<". ";
        if(head=="card")
        {
            int j=choices[i].find_first_of('@');
            string card=choices[i].substr(0,j),zone=choices[i].substr(j+1);
            if(card=="cancel") cout<<zone;
            else
            {
                if(card=="hidden") card="一张牌";
                if(zone!="hand:"+name) cout<<parseZone(zone)+"的";
                cout<<card;
            }
        }
        else cout<<choices[i];
        cout<<"\n";
    }
    cout<<"请选择数字：";
    cin>>res;
    cin.clear();
    cin.ignore(65535,'\n');
    if(res<0||res>=choices.size()) return choices.size()-1;
    return res;
}

vector<int> ConsolePlayer::getMultiChoice(string reason, const vector<string> &choices, int max, const vector<bool> &mandatory)
{
    vector<int> res;
    vector<bool> selected;
    vector<string> newChoices;
    int i,n=choices.size(),num=0;
    for(i=0;i<n;i++)
    {
        selected.push_back(mandatory[i]);
        if(mandatory[i]) {num++;newChoices.push_back("* "+choices[i]);}
        else newChoices.push_back(choices[i]);
    }
    newChoices.push_back("Cancel");
    if(num) newChoices.push_back("OK");
    if(max>=0&&num>max) return res;
    if(max>choices.size()) max=choices.size();
    if(num!=max)
    {
        if(max==1)
        {
            i=getChoice(reason,newChoices);
            if(i<n) res.push_back(i);
            return res;
        }
        while(1)
        {
            i=getChoice(reason,newChoices);
            if(i==n) return res;
            if(i==n+1) break;
            if(selected[i]) if(mandatory[i]) cout<<"This is mandatory!";
            else {selected[i]=false;newChoices[i]=choices[i];if(--num==0) newChoices.pop_back();}
            else if(num==max) cout<<"Can't select more!\n";
            else {selected[i]=true;newChoices[i]="* "+choices[i];if(num++==0) newChoices.push_back("OK");}
        }
    }
    for(i=0;i<n;i++) if(selected[i]) res.push_back(i);
    return res;
}
