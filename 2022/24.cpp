#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <numeric>
#include <vector>/*
#include <unordered_map>*/

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

class storm {
public:
    u_t x,y;
    storm(u_t X,u_t Y): x(X),y(Y) {}
};

class field {
public:
    char f[27][122];
    u_t X,Y;
    field(u_t x,u_t y): X(x),Y(y) {
        for (u_t Y=0;Y<=y+1;Y++) {
            f[Y][0]='#';
            f[Y][x+1]='#';
            for (u_t X=1;X<=x;X++) {
                if (Y==0&&X!=1)f[Y][X]='#';
                else if (Y==y+1&&X!=x)f[Y][X]='#';
                else f[Y][X]='.';
            }
        }
    }
    void print() {
        for (u_t j=0;j<=Y+1;j++){for(u_t i=0;i<=X+1;i++)std::cout<<f[j][i];std::cout<<std::endl;}std::cout<<std::endl;
    }
};

class flower {
public:
    u_t x,y;
    flower(u_t X,u_t Y):x(X),y(Y){}
};

bool is_walkable(char a, u_t walk) {
    if (a=='.')return true;
    if (a>='$' && a<('$'+walk))return true;
    return false;
}

void read_or_die(std::string pattern, std::istream& input) {
    for (auto a:pattern) {
        if (a != input.get()) {
            std::string parsed_pattern = "";
            for (auto b:pattern) {
                if (b < ' ' || b > 126)
                    parsed_pattern += '?';
                else
                    parsed_pattern += b;
            }
            std::cerr<<'\''<<parsed_pattern<<'\''<<" expected"<<std::endl;
            exit(1);
        }
    }
}

int real_main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr<<"Usage: "<<argv[0]<<" [-v {verbosity_level}] {input_file}"<<std::endl;
        exit(1);
    }
    verb_lvl = argc > 3 && argv[1][0] == '-' && argv[1][1] == 'v' ? std::stoul(argv[2]) : 0;

    std::ifstream input(argv[argc - 1]);

    if (!input.good()) {
        std::cerr<<"Input file "<<argv[argc - 1]<<" did not open correctly"<<std::endl;
        exit(1);
    }

    if (verb_lvl > 0) {
        std::cout<<"Running in verbosity mode "<<verb_lvl<<std::endl;
    }

    read_or_die("#.",input);
    u_t x=0, y=0;
    std::vector<storm> down={},up={},left={},right={};
    while(input.get()=='#')x++;
    bool last_line=false;
    while(!input.eof()&&!last_line) {
        read_or_die("#",input);
        for (u_t i=0;i<x;i++) {
            char a=input.get();
            if (a == '#') {
                last_line=true;
                break;
            }
            if (a!='.') {
                switch (a) {
                    case '^':up.emplace_back(i,y);break;
                    case 'v':down.emplace_back(i,y);break;
                    case '>':right.emplace_back(i,y);break;
                    case '<':left.emplace_back(i,y);break;
                    default:std::cerr<<"Unexpected: "<<a<<std::endl;exit(1);
                }
            }
        }
        if (!last_line) {
            y++;
            read_or_die("#\n",input);
        }
    }
    input.close();

    u_t k=std::lcm(x,y);

    std::vector<field> fs;//fields
    fs.reserve(k);
    if(verb_lvl>2)std::cout<<'\n';
    for (u_t i=0;i<k;i++) {
        fs.emplace_back(x,y);
        for (auto a:up) {
            fs.back().f[(a.y+k-i)%y+1][a.x+1] = '^';
        }
        for (auto a:down) {
            u_t ny = (a.y+i)%y+1;
            if (fs.back().f[ny][a.x+1] == '.')
                fs.back().f[ny][a.x+1] = 'v';
            else
                fs.back().f[ny][a.x+1] = '2';
        }
        for (auto a:right) {
            u_t nx = (a.x+i)%x+1;
            if (fs.back().f[a.y+1][nx] == '.')
                fs.back().f[a.y+1][nx] = '>';
            else if (std::isdigit(fs.back().f[a.y+1][nx]))
                fs.back().f[a.y+1][nx]++;
            else
                fs.back().f[a.y+1][nx] = '2';
        }
        for (auto a:left) {
            u_t nx = (a.x+k-i)%x+1;
            if (fs.back().f[a.y+1][nx] == '.')
                fs.back().f[a.y+1][nx] = '<';
            else if (std::isdigit(fs.back().f[a.y+1][nx]))
                fs.back().f[a.y+1][nx]++;
            else
                fs.back().f[a.y+1][nx] = '2';
        }
        if(verb_lvl>2)fs.back().print();
    }

    bool solved = false;
    u_t step=0, cur=0, walk=0;

    std::vector<flower> fls[2] = {{{1,0}},{}};
    fs[0].f[0][1]=static_cast<char>('$'+walk);

    while(!solved) {
        step++;
        bool next_walk = false;
        for (auto a:fls[cur]) {
            if (a.y==y+1) {
                if (walk==0){
                    std::cout<<"ans1:"<<step-1<<'\n'<<std::endl;
                    next_walk = true;
                    break;
                } else if(walk==2) {
                    std::cout<<"ans2:"<<step-1<<'\n'<<std::endl;
                    solved = true;
                    break;
                }
            } else if (a.y==0&&walk==1) {
                next_walk=true;
                break;
            } else {
                if (is_walkable(fs[step%k].f[a.y][a.x], walk)) {
                    fs[step%k].f[a.y][a.x]=static_cast<char>('$'+walk);
                    fls[(cur+1)%2].emplace_back(a.x,a.y);
                }
                if (a.y!=y+1) {
                    if (is_walkable(fs[step%k].f[a.y+1][a.x], walk)) {
                        fs[step%k].f[a.y+1][a.x]=static_cast<char>('$'+walk);
                        fls[(cur+1)%2].emplace_back(a.x,a.y+1);
                    }
                }
                if (a.y!=0) {
                    if (is_walkable(fs[step%k].f[a.y-1][a.x], walk)) {
                        fs[step%k].f[a.y-1][a.x]=static_cast<char>('$'+walk);
                        fls[(cur+1)%2].emplace_back(a.x,a.y-1);
                    }
                }
                if (a.y!=0&&a.y!=y+1) {
                    if (is_walkable(fs[step%k].f[a.y][a.x+1], walk)) {
                        fs[step%k].f[a.y][a.x+1]=static_cast<char>('$'+walk);
                        fls[(cur+1)%2].emplace_back(a.x+1,a.y);
                    }
                    if (is_walkable(fs[step%k].f[a.y][a.x-1], walk)) {
                        fs[step%k].f[a.y][a.x-1]=static_cast<char>('$'+walk);
                        fls[(cur+1)%2].emplace_back(a.x-1,a.y);
                    }
                }
            }
        }
        if (next_walk) {
            if (walk == 0) {
                fs[step%k].f[y+1][x]=static_cast<char>('$'+1);
                fls[1].clear();
                fls[0] = {{x,y+1}};
                if (is_walkable(fs[step%k].f[y][x], walk+1)) {
                    fs[step%k].f[y][x]=static_cast<char>('$'+1);
                    fls[0].emplace_back(x,y);
                }
                cur = 0;
            } else if (walk == 1) {
                fs[step%k].f[0][1]=static_cast<char>('$'+2);
                fls[1].clear();
                fls[0] = {{1,0}};
                if (is_walkable(fs[step%k].f[1][1], walk+1)) {
                    fs[step%k].f[1][1]=static_cast<char>('$'+2);
                    fls[0].emplace_back(1,1);
                }
                cur = 0;
            }
            walk++;
        } else {
            fls[cur].clear();
            cur = (cur+1)%2;
        }
        if(verb_lvl>1)fs[step%k].print();
    }

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
