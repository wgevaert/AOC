#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric> // for std::iota

#define slashing 0
#define fire 1
#define bludgeoning 2
#define radiation 3
#define cold 4
#define no_target static_cast<unsigned>(-1)

struct group {
    unsigned units,hp,init,atk,atk_type,target;
    bool good,weak[5]={false,false,false,false,false},immune[5]={false,false,false,false,false},targeted;
    unsigned eff_pow() const{return atk*units;}
    unsigned simulate_atk(group b) const {return b.immune[atk_type]?0:(b.weak[atk_type]?2:1)*eff_pow();}
};

std::ostream& operator<<(std::ostream& out,group g){
    out<<g.units<<(g.good ? " good":" bad")<<" units with "<<g.hp<<" hp, "<<g.atk<<" atk of type "<<g.atk_type<<", "<<g.init<<" init, weaknesses";for(int i=0;i<5;i++)out<<(i?',':' ')<<g.weak[i];std::cout<<" and immunities";for(int i=0;i<5;i++)out<<(i?',':' ')<<g.immune[i];
    return out;
}

void read(std::string check,std::istream& input){
    char A;
    for(auto a:check)if((A=input.get())!=a){
       if(input.eof()){std::cout<<"Unexpected eof found instead of"<<check<<std::endl;exit(1);}
       std::cout<<check<<" expected, '"<<A<<"' found instead of '"<<a<<"' (value "<<static_cast<int>(A)<<')'<<std::endl;exit(1);
    }
}

unsigned read_atk(std::istream& input_file){
    switch(input_file.get()) {
        case 's':read("lashing",input_file);return slashing;
            break;
        case 'f':read("ire",input_file);return fire;
            break;
        case 'b':read("ludgeoning",input_file);return bludgeoning;
            break;
        case 'r':read("adiation",input_file);return radiation;
            break;
        case 'c':read("old",input_file);return cold;
            break;
        default:std::cout<<" unknown weapon found "<<std::endl;exit(1);
    }
}


void read_army(std::istream& input_file,bool good,std::vector<group>& rtr) {
    if(input_file.get()!='I'){std::cout<<"I expected, right file?"<<std::endl;exit(1);}
    while(input_file.get()!='\n');

    while(input_file.peek()!='\n'&&!input_file.eof()) {
//std::cout<<"reading..."<<std::endl;
        group cur;
        cur.good=good;
        input_file>>cur.units;
        read(" units each with ",input_file);
        input_file>>cur.hp;
        read(" hit points ",input_file);
        if(input_file.get()=='(') {
            switch(input_file.get()) {
                case 'i':
                    read("mmune to ",input_file);
                    do {
                        cur.immune[read_atk(input_file)]=true;
                    }while(input_file.get()==','&&input_file.get()==' ');
                    break;
                case 'w':
                    read("eak to ",input_file);
                    do {
                        cur.weak[read_atk(input_file)]=true;
                    }while(input_file.get()==','&&input_file.get()==' ');
                    break;
                default:
                    std::cout<<"No weaknesses or immunities found"<<std::endl;exit(1);
            }
            read(" ",input_file);
            switch(input_file.get()) {
                case 'i':
                    read("mmune to ",input_file);
                    do {
                        cur.immune[read_atk(input_file)]=true;
                    }while(input_file.get()==','&&input_file.get()==' ');
                    break;
                case 'w':
                    if(input_file.get()=='i')break;
                    read("ak to ",input_file);
                    do {
                        cur.weak[read_atk(input_file)]=true;
                    }while(input_file.get()==','&&input_file.get()==' ');
                    break;
                default:
                    std::cout<<"No second weaknesses or immunities found"<<std::endl;exit(1);
            }
        } else while(input_file.get()!='i');
        if(input_file.get()==' ')while(input_file.get()!='t');
        read("h an attack that does ",input_file);
        input_file>>cur.atk;
        read(" ",input_file);
        cur.atk_type=read_atk(input_file);
        read(" damage at initiative ",input_file);
        input_file>>cur.init;
        read("\n",input_file);
        rtr.push_back(cur);
//std::cout<<"Pushed "<<cur<<" on the units"<<std::endl;
    }
}

std::pair<bool,unsigned> simulate(std::vector<group> units,unsigned boost) {
    unsigned good_units=0,bad_units=0,prev_good_units=-1,prev_bad_units=-1;
    std::vector<size_t> indices(units.size());
    std::iota(indices.begin(),indices.end(),0);
    for(auto& a:units)if(a.good){good_units+=a.units;a.atk+=boost;}else bad_units+=a.units;
    while(good_units&&bad_units) {
        std::sort(indices.begin(),indices.end(),[units](size_t a,size_t b){return std::make_pair(units[a].eff_pow(),units[a].init) > std::make_pair(units[b].eff_pow(),units[b].init);});
//for(auto a:units) std::cout<<a<<std::endl;std::cout<<std::endl;
        for(auto &a:units)a.targeted=(a.units==0);
        for(auto j:indices) if(units[j].units){
            size_t max_i;
            std::pair<std::pair<unsigned,unsigned>,unsigned> max = {{0,0},0};
            for(size_t i=0;i<units.size();i++) {
                if(units[i].good!=units[j].good && !units[i].targeted && units[i].units){
                    std::pair<std::pair<unsigned,unsigned>,unsigned> new_max = std::make_pair(std::make_pair(units[j].simulate_atk(units[i]),units[i].eff_pow()),units[i].init);
//std::cout<<j<<" considers "<<i<<" with "<<new_max.first.first<<" damage and "<<units[i].hp<<" hp"<<std::endl;
                    if(max<new_max){max_i=i;max=new_max;}
                }
            }
            if(max.first.first!=0){
                units[j].target=max_i;
                units[max_i].targeted = true;
//std::cout<<j<<" targets "<<max_i<<std::endl;
            } else {
                units[j].target = no_target;
//std::cout<<j<<" targets no one"<<std::endl;
            }
        }
        std::sort(indices.begin(),indices.end(),[units](size_t a,size_t b){return units[a].init > units[b].init;});
        for(auto j:indices) if(units[j].units) {
            if(units[j].target == no_target)continue;
            unsigned atk = units[j].simulate_atk(units[units[j].target]);
            unsigned units_lost = atk/units[units[j].target].hp;
            if (units[units[j].target].units>=units_lost) {
//std::cout<<units[j].target<<" loses "<<units_lost<<" units to an attack of "<<j<<" with "<<atk<<" damage"<<std::endl;
                units[units[j].target].units -= units_lost;
                if(units[j].good)bad_units-=units_lost;
                else good_units-=units_lost;
            } else {
//std::cout<<units[j].target<<" dies to an attack of "<<j<<" with "<<atk<<" damage"<<std::endl;
                if(units[j].good)bad_units-=units[units[j].target].units;
                else good_units-=units[units[j].target].units;
                units[units[j].target].units = 0;
            }
        }
        if(good_units==prev_good_units && bad_units == prev_bad_units){std::cout<<"Impasse found"<<std::endl;return {false,0};}
        prev_good_units=good_units;prev_bad_units=bad_units;
//std::cout<<good_units<<','<<bad_units<<std::endl;
    }
    return {good_units,good_units+bad_units};//one of these is 0
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<group> units={};
    read_army(input_file,true,units);
    read("\n",input_file);
    read_army(input_file,false,units);
    std::cout<<units.size()<<" groups found"<<std::endl;
    simulate(units,0);
    unsigned jets=1,boost=0;
    auto result = simulate(units,jets);
    while(!result.first){
        jets<<=1;
        result = simulate(units,jets);
        std::cout<<"simulation with boost "<<jets<<(result.first ? " succesfull":" disastrous")<<" with "<<result.second<<" units remaining"<<std::endl;
    }
    while(jets) {
        result=simulate(units,boost+jets);
        std::cout<<jets<<"simulation with boost "<<boost+jets<<(result.first ? " succesfull":" disastrous")<<" with "<<result.second<<" units remaining"<<std::endl;
        if(!result.first)
            boost+=jets;
        jets>>=1;
    }
    result = simulate(units,boost);
    std::cout<<(result.first ? result.second : simulate(units,boost+1).second)<<std::endl;

    return 0;
}
