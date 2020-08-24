#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

struct field;

struct bandit {
    bool is_goblin,is_moving=true;
    unsigned x,y,hp=200,atk=3;
    bandit(bool _goblin,unsigned _x,unsigned _y){is_goblin=_goblin;x=_x;y=_y;}
    bool operator<(bandit& a){return (hp&&!a.hp) || ((hp||!a.hp) && (y<a.y||(y==a.y&&x<a.x)));}
    bool move(field & cave, std::vector<bandit>& bandits, bool& done);
    bool is_adjacent(bandit a);
    bool fight(field & cave, std::vector<bandit>& bandits);
};

struct field_elt{
    bool free,is_goal,is_wall;
    unsigned seen;
};

struct field{
    static const unsigned size = 100;
    field_elt f[size][size];
    unsigned x_max=0,y_max=0;
    void reset(){for(unsigned i=0;i<=x_max;i++)for(unsigned j=0;j<=y_max;j++)f[i][j].seen=f[i][j].is_goal=false;}
    void set_goals(unsigned x,unsigned y){
        f[x+1][y].is_goal=f[x-1][y].is_goal=f[x][y+1].is_goal=f[x][y-1].is_goal=true;
    }
    void full_reset(std::vector<bandit> bandits){
        for(unsigned i=0;i<=x_max;i++)for(unsigned j=0;j<=y_max;j++)
            f[i][j].free=!f[i][j].is_wall;
        for (auto a:bandits)f[a.x][a.y].free=false;
    }
};

void read_field(std::istream& input_file,std::vector<bandit>& bandits, field& cave) {
    unsigned x=0;
    while(!input_file.eof()) {
        switch(input_file.get()) {
            case '#':
                cave.f[x][cave.y_max].free = false;
                cave.f[x][cave.y_max].is_wall = true;
                x++;if(x>cave.x_max)cave.x_max=x;break;
            case '.':
                cave.f[x][cave.y_max].free = true;
                cave.f[x][cave.y_max].is_wall = false;
                x++;if(x>cave.x_max)cave.x_max=x;break;
            case 'G':
                bandits.emplace_back(true,x,cave.y_max);
                cave.f[x][cave.y_max].is_wall = false;
                cave.f[x][cave.y_max].free = false;
                x++;if(x>cave.x_max)cave.x_max=x;break;
            case 'E':
                bandits.emplace_back(false,x,cave.y_max);
                cave.f[x][cave.y_max].is_wall = false;
                cave.f[x][cave.y_max].free = false;
                x++;if(x>cave.x_max)cave.x_max=x;break;
            case '\n':x=0;cave.y_max++;break;
            default:if(input_file.eof())break;std::cout<<"Weird..."<<std::endl;exit(1);
        }
    }
}

void print(field cave,std::vector<bandit> bandits,std::ostream& out) {
    for(unsigned j=0;j<cave.y_max;j++){
        for(unsigned i=0;i<cave.x_max;i++){
            if (cave.f[i][j].free)
                out<<(cave.f[i][j].is_goal ? '*' : (cave.f[i][j].seen ? static_cast<char>(cave.f[i][j].seen+'0'-1) : '.'));
            else {
                bool has_bandit = false;
                for(auto a:bandits)
                    if(a.hp && a.x==i&&a.y==j){
                        out<<(a.is_goblin ? 'G' : 'E');
                        has_bandit=true;
                    }
                if(!has_bandit)out<<'#';
            }
        }
        out<<"\n";
    }
    out<<std::endl;
}

struct flower{
    static std::pair<int,int> directions[4];
    unsigned x,y,id;
    flower(unsigned _x,unsigned _y, unsigned _id){x=_x;y=_y;id=_id;}
};

std::pair<int,int> flower::directions[4]={std::make_pair(0,1),std::make_pair(1,0),std::make_pair(-1,0),std::make_pair(0,-1)};//in inverse reading order;

typedef std::pair<std::pair<unsigned,unsigned>,unsigned> goal_t;

bool bandit::move(field& cave, std::vector<bandit>& bandits, bool& done) {
    if(!hp)return false;
    unsigned enemy_cnt = 0;
    for(auto b: bandits)
        if(b.hp && is_goblin!=b.is_goblin) {
            cave.set_goals(b.x,b.y);
            enemy_cnt++;
        }
    if(!enemy_cnt){
        done=true;
    }
    if(cave.f[x][y].is_goal){cave.reset();return false;}
    std::vector<flower>flowers[2];
    unsigned short p=0,q=1,id=0;
    std::pair<unsigned,unsigned> next_step[4];
    for(int i=3;i>=0;i--)//inverse loop
        if(cave.f[x+flower::directions[i].first][y+flower::directions[i].second].free) {
            cave.f[x+flower::directions[i].first][y+flower::directions[i].second].seen = id + 1;
            next_step[id] = std::make_pair(x+flower::directions[i].first,y+flower::directions[i].second);
            flowers[p].emplace_back(x+flower::directions[i].first,y+flower::directions[i].second,id);
            id++;
        }
    std::vector<goal_t> found={};
    do {
        for(auto f:flowers[p]) {
            if(cave.f[f.x][f.y].is_goal){
                found.emplace_back(std::make_pair(f.y,f.x),f.id);
            }
        }
        if(found.size())break;
        for(auto f:flowers[p]) {
            for(auto d:flower::directions) {
                unsigned new_x = f.x+d.first,new_y=f.y+d.second;
                if(cave.f[new_x][new_y].free && !cave.f[new_x][new_y].seen) {
                    flowers[q].emplace_back(new_x,new_y,f.id);
                    cave.f[new_x][new_y].seen = f.id + 1;
                }
            }
        }
        flowers[p].clear();
        p=1-p;q=1-q;
    }while(flowers[p].size());
    bool moving = false;
    if(found.size()) {
        moving = true;
        goal_t min = found.back();
        found.pop_back();
        while(found.size()) {
            if(min > found.back())min=found.back();
            found.pop_back();
        }
        cave.f[x][y].free=true;
        x = next_step[min.second].first;
        y = next_step[min.second].second;
        cave.f[x][y].free=false;
    }
    cave.reset();
    return moving;
}

bool bandit::is_adjacent(bandit b){
    return (x==b.x&&y==b.y-1) || (x==b.x-1&&y==b.y) || (x==b.x+1&&y==b.y) || (x==b.x&&y==b.y+1);
}

bool bandit::fight(field& cave, std::vector<bandit>& bandits) {
    if(!hp)return false;//dead men tell no tales
    bool killed = false;
    unsigned min_hp=0,the_i;
    for(unsigned i=0;i<bandits.size();i++)
        if(bandits[i].is_goblin != is_goblin && is_adjacent(bandits[i]) && bandits[i].hp && (bandits[i].hp<min_hp || !min_hp)) {
            min_hp=bandits[i].hp;
            the_i = i;
        }
    if(min_hp){
        if (bandits[the_i].hp <= atk) {
            bandits[the_i].hp=0;
            cave.f[bandits[the_i].x][bandits[the_i].y].free = true;
            killed = true;
        } else bandits[the_i].hp -= atk;
    }
    return killed;
}

bool step_bandits(field& cave, std::vector<bandit>& bandits,bool try_move, bool& done){
    bool should_move_on_next_turn = false;
    for(auto& a:bandits){
        if (/*try_move &&*/ a.hp && a.move(cave,bandits,done))should_move_on_next_turn = true;
        if (a.fight(cave,bandits))should_move_on_next_turn = true;
    }
    return should_move_on_next_turn;
}

unsigned simulate (field& cave, std::vector<bandit> bandits, bool verbose) {// bandits by value.
    cave.full_reset(bandits);
    unsigned goblin_cnt=0,elf_cnt=0,rounds=0;
    for(auto a:bandits)if(a.is_goblin)goblin_cnt++;else elf_cnt++;
    bool should_move = true,done=false,elf_killed=false;
    while(goblin_cnt && elf_cnt) {
        ++rounds;
        if ((should_move = step_bandits(cave,bandits,should_move,done)) && verbose) {
//            std::cout<<"round "<<rounds<<':'<<std::endl;
            print(cave,bandits,std::cout);
        }
        if(done){
//            if(verbose)std::cout<<"HEY!"<<std::endl;
            rounds--;
        }
        std::sort(bandits.begin(),bandits.end());
        while(!bandits.back().hp) {
            if (bandits.back().is_goblin)goblin_cnt--;
            else {
                elf_cnt--;elf_killed=true;
//                if(verbose)std::cout<<"an elf has been killed"<<std::endl;
            }
            bandits.pop_back();
        }
//        if(verbose){unsigned elf_hp_total=0,gob_hp_total=0;
//        for(auto a:bandits){if(a.is_goblin)gob_hp_total+=a.hp;else elf_hp_total+=a.hp;std::cout<<(a.is_goblin?'G':'E')<<a.hp<<std::endl;}
//        std::cout<<goblin_cnt<<'-'<<gob_hp_total<<','<<elf_hp_total<<'-'<<elf_cnt<<std::endl;}//'='<<gob_hp_total<<'+'<<elf_hp_total<<" found"<<std::endl;}
        if(goblin_cnt+elf_cnt != bandits.size())std::cout<<"HELP!!!"<<std::endl;
    }
    unsigned hp_total=0;
    for(unsigned i=0;i<bandits.size();i++) {
//        if (verbose || !elf_killed)std::cout<<bandits[i].hp<<(i==bandits.size()-1 ? '=' : '+');
        hp_total+=bandits[i].hp;
    }
//    if(verbose||!elf_killed)std::cout<<hp_total<<" multiplied with "<<rounds<<", i.e. "<<rounds*hp_total<<" should be the answer"<<std::endl;
    return elf_killed ? 0 : hp_total*rounds;
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    field cave;
    std::vector<bandit> bandits={};
    read_field(input_file,bandits,cave);
    unsigned base_atk=3,result;
    if (argc < 3)
        simulate(cave,bandits,true);
    else {
        do {
            base_atk++;
//            std::cout<<"trying "<<base_atk<<" attack"<<std::endl;
            for(auto&a : bandits)
                if(!a.is_goblin)
                    a.atk = base_atk;
           result = simulate(cave,bandits,base_atk==12);
        } while(!result);
//        std::cout<<" at "<<base_atk<<" attack, the elves all live, with a result of "<<result<<std::endl;
    }
    return 0;
}
