#include <iostream>
#include <vector>

#define Magic_Missle 0
#define Drain 1
#define Shield 2
#define Poison 3
#define Recharge 4

std::ostream& operator<< (std::ostream& os,const std::vector<unsigned short>& val){
    if(!val.size()){os<<"HELP!"<<std::endl;return os;}
    for(unsigned i=0;i<val.size()-1;i++)os<<val[i]<<',';
    os<<val[val.size()-1];
    return os;
};

struct monster {
    unsigned short hp=58,atk=9;
};

struct wizard {
    unsigned short hp=50,mana=500,mana_spent=0,armor_timer=0,poison_timer=0,recharge_timer=0,turn=0;
    bool hard_difficulty=false;
};

std::ostream& operator<< (std::ostream& os,const wizard& me){
    std::cout<<'['<<me.hp<<','<<me.mana<<','<<me.mana_spent<<','<<me.armor_timer<<']';
    return os;
};

unsigned short cast_spell(unsigned short spell_nr,monster& ugly_beast, wizard& me,bool verbose){
    switch(spell_nr){
        case Magic_Missle:
            if(me.mana<53){if(verbose)std::cout<<me.mana<<" IS UNSUFFICIENT MANA"<<std::endl;return 2;}
            me.mana-=53;
            me.mana_spent+=53;
            if (ugly_beast.hp<=4){ugly_beast.hp=0;if(verbose)std::cout<<"MONSTER KILLED BY MAGIC MISSLE"<<std::endl;return 1;}
            ugly_beast.hp-=4;
            break;
        case Drain:
            if(me.mana<73){if(verbose)std::cout<<me.mana<<" IS UNSUFFICIENT MANA"<<std::endl;return 2;}
            me.mana-=73;
            me.mana_spent+=73;
            me.hp+=2;
            if (ugly_beast.hp<=2){ugly_beast.hp=0;if(verbose)std::cout<<"MONSTER KILLED BY DRAIN"<<std::endl;return 1;}
            ugly_beast.hp-=2;
            break;
        case Shield:
            if(me.mana<113){if(verbose)std::cout<<me.mana<<" IS UNSUFFICIENT MANA"<<std::endl;return 2;}
            if(me.armor_timer>1){if(verbose)std::cout<<"CANNOT CAST SHIELD: TIMER IS "<<me.armor_timer<<std::endl;return 2;}
            me.mana-=113;
            me.mana_spent+=113;
            me.armor_timer = 6;
            break;
        case Poison:
            if(me.mana<173){if(verbose)std::cout<<me.mana<<" IS UNSUFFICIENT MANA"<<std::endl;return 2;}
            if(me.poison_timer>1){if(verbose)std::cout<<"CANNOT CAST POISON: TIMER IS "<<me.poison_timer<<std::endl;return 2;}
            me.mana-=173;
            me.mana_spent+=173;
            me.poison_timer = 6;
            break;
        case Recharge:
            if(me.mana<229){if(verbose)std::cout<<me.mana<<" IS UNSUFFICIENT MANA"<<std::endl;return 2;}
            if(me.recharge_timer>1){if(verbose)std::cout<<"CANNOT CAST RECHARGE: TIMER IS "<<me.recharge_timer<<std::endl;return 2;}
            me.mana-=229;
            me.mana_spent+=229;
            me.recharge_timer = 5;
            break;
        default:
            std::cout<<"UNKNOWN SPELL: "<<spell_nr<<std::endl;return 2;
    }
    return 0;
}

struct battle_result{
    unsigned short mana_spent, mana_left, turn;
    unsigned short result;
    static const unsigned short won=1,lost=0,undecided=2;
    battle_result(){}
    battle_result(wizard me,unsigned short _result){mana_spent=me.mana_spent;result=_result;mana_left=me.mana;turn=me.turn;}
};

battle_result simulate(std::vector<unsigned short>& strategy, bool verbose, unsigned short mana_max, const bool hard_difficulty) {
    wizard me;
    monster ugly_beast;
    for(;;me.turn++) {
        unsigned short armor= me.armor_timer;
        unsigned short poison= me.poison_timer;
        unsigned short recharge= me.recharge_timer;
        if(!(me.turn%2)) {
            // It's my turn!
            unsigned short spell, result;
            if (hard_difficulty){
                if(me.hp<=1){if(verbose)std::cout<<"YOU DIED, THE GAME IS TOO HARD FOR YOU"<<std::endl;return battle_result(me,battle_result::lost);}
                me.hp--;
            }
            if(me.mana<53){
                if(verbose)std::cout<<"I RAN OUT OF MANA"<<std::endl;
                return battle_result(me,battle_result::lost);
            }
            do {
                if (strategy.size()>me.turn/2)
                    spell = strategy[me.turn/2];
                else {
                    if(!verbose)
                        return battle_result(me,battle_result::undecided);
                    std::cout<<"Ugly beast has "<<ugly_beast.hp<<" health, I have "<<me.hp<<" health and "<<me.mana<<" mana.";
                    if (me.armor_timer)std::cout<<" armor is active ("<<me.armor_timer<<')';
                    if (me.poison_timer)std::cout<<" poison is active ("<<me.poison_timer<<')';
                    if (me.recharge_timer)std::cout<<" recharge is active ("<<me.recharge_timer<<')';
                    std::cout<<"\n What spell should I play? (0=magic missle, 1=drain, 2=shield, 3=poison, 4=recharge)"<<std::endl;
                    do {std::cin>>spell;}while(spell>5);
                }
                switch(result = cast_spell(spell,ugly_beast,me,verbose)){
                    case 1:
                        if(verbose)
                            std::cout<<"I WON AT TURN "<<me.turn<<" AND HAVE SPEND "<<me.mana_spent<<" MANA "<<std::endl;
                        return battle_result(me,battle_result::won);
                    case 2:
                        if(verbose)
                            std::cout<<"COULD NOT CAST SPELL AT TURN "<<me.turn<<std::endl;
                        else return battle_result(me,battle_result::lost);
                        break;
                    case 0:
                        break;
                    default:
                        std::cout<<"YOU BROKE THE GAME, WELL DONE"<<std::endl;
                }
            }while(result==2);
            if(me.mana_spent>mana_max)return battle_result(me,battle_result::lost);
            if(strategy.size()<=me.turn/2)
                strategy.push_back(spell);
        } else {
            // Help, I'm under attack!
            unsigned short atk = ugly_beast.atk - (armor?7:0);
            if (atk >= me.hp){
                if(verbose)
                    std::cout<<"I die at turn "<<me.turn<<std::endl;
                return battle_result(me,battle_result::lost);
            }
            me.hp-=atk;
        }
        if (armor && armor==me.armor_timer)me.armor_timer--;
        if (poison){
            if(ugly_beast.hp<=3){
                if(verbose)
                    std::cout<<"MONSTER DIED OF POISON AT TURN "<<me.turn<<", I HAVE SPENT "<<me.mana_spent<<" MANA"<<std::endl;
                return battle_result(me,battle_result::won);
            }
            ugly_beast.hp-=3;
            if(poison==me.poison_timer)me.poison_timer--;
        }
        if (recharge){me.mana+=101;if(recharge==me.recharge_timer)me.recharge_timer--;}
    }
}

void find_optimal_strategy(unsigned short& mana_min,std::vector<unsigned short>& strategy, const bool hard_difficulty){
    static unsigned depth=0;
    battle_result current_result;
    for(int i=0;i<5;i++){
        strategy.push_back(i);
        current_result = simulate(strategy,false,mana_min,hard_difficulty);
        switch(current_result.result){
            case 0:
                break;
            case 1:
                if(mana_min>current_result.mana_spent){mana_min=current_result.mana_spent;std::cout<<"Strategy ";for(auto a:strategy)std::cout<<a<<' ';std::cout<<" worked better, with only "<<mana_min<<" mana"<<std::endl;}
                break;
            case 2:
                find_optimal_strategy(mana_min,strategy,hard_difficulty);
                break;
        }
        strategy.pop_back();
    }
    depth--;
}

int main(){
    bool hard_difficulty = true;

    std::vector<unsigned short>strategy ={};
    battle_result player_result;
    std::cout<<"WELCOME TO THIS GAME!\n PLEASE SELECT THE DIFFICULTY YOU WANT TO PLAY (Hard, Easy)"<<std::endl;
    char a;do {a=std::cin.get();}while(a!='H'&&a!='E');hard_difficulty=(a=='H');
    while((player_result = simulate(strategy,true,-1,hard_difficulty)).result == 0){std::cout<<"Please try harder"<<std::endl;strategy={};};
    std::cout<<"Well done, strategy ";for(auto a:strategy)std::cout<<a<<' ';std::cout<<"worked!"<<std::endl;
    strategy = {};
    find_optimal_strategy(player_result.mana_spent,strategy,hard_difficulty);
    std::cout<<player_result.mana_spent<<" IS OPTIMAL FOR THIS DIFFICULTY"<<std::endl;
    return 0;
}
