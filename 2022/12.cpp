#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>/*
#include <unordered_map>*/

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

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

class tile {
public:
    bool seen = false;
    char height;
    tile(char h): height(h) {}
};

class flower {
public:
    u_t steps;
    long x,y;
    flower(u_t s,long X,long Y): steps(s),x(X),y(Y) {}
};

const long dirs[4][2] = {{1,0},{-1,0},{0,1},{0,-1}};

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

    u_t sx,sy,ex,ey,x=0,xm=0,y=0;
    std::vector<std::vector<tile>> field={};

    do {
        x=0;
        field.emplace_back();
        char a = input.get();
        do {
            switch (a) {
                case 'S':
                    sx=x;
                    sy=y;
                    field.back().emplace_back('a');
                    break;
                case 'E':
                    ex=x;
                    ey=y;
                    field.back().emplace_back('z');
                    break;
                default:
                    if (verb_lvl)std::cout<<a;
                    field.back().emplace_back(a);
                    break;
            }
            a = input.get();
            x++;
        } while (a != '\n' && !input.eof());
        if (x>xm)xm=x;
        y++;
    } while (!input.eof() && input.peek()!='\n');
    input.close();
    field.pop_back();
    x=xm;

    std::vector<flower> flowers[2] = {{flower(0,ex,ey)},{}};
    field[ey][ex].seen=true;
    u_t cur=0;
    bool solved2=false;

    while (!field[sy][sx].seen) {
        if(verb_lvl>3)std::cout<<flowers[cur].size()<<std::endl;
        for (auto a:flowers[cur]) {
            for (auto d:dirs) {
                flower nw(a.steps+1, a.x+d[0], a.y+d[1]);
                if(verb_lvl>5)std::cout<<"Trying "<<nw.x<<','<<nw.y<<" with "<<x<<','<<y<<std::endl;
                if (nw.x<x&&nw.x>=0&&nw.y<y&&nw.y>=0&&!field[nw.y][nw.x].seen&&field[nw.y][nw.x].height+1>=field[a.y][a.x].height) {
                    flowers[(cur+1)%2].push_back(nw);
                    field[nw.y][nw.x].seen=true;
                    if (nw.y==sy&&nw.x==sx)std::cout<<nw.steps<<std::endl;
                    if (!solved2&&field[nw.y][nw.x].height=='a'){solved2=true;std::cout<<nw.steps<<std::endl;}
                }
            }
        }
        flowers[cur].clear();
        cur = (cur+1)%2;
        if(verb_lvl>4)for (auto a:field){for (auto b:a)std::cout<<(b.seen ? static_cast<char>(b.height-'a'+'A') : b.height);std::cout<<std::endl;}
    }

    // do things without input
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
