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

    u_t cur=0;
    std::vector<std::string> field[2];
    std::string line;
    std::getline(input,line);
    while(line!="") {
        field[0].push_back(line);
        field[1].push_back(line);
        std::getline(input,line);
    }
    input.close();

    bool moved;
    ull_t step=0;
    do {
        moved=false;
        if (verb_lvl){for (auto a:field[cur])std::cout<<a<<std::endl;std::cout<<std::endl;}
        for (auto y=0;y<field[cur].size();y++) {
            for (auto x=0;x<field[cur][y].size();x++) {
                if (field[cur][y][x]=='>'&&field[cur][y][(x+1)%field[cur][y].size()]=='.') {
                    moved = true;
                    field[(cur+1)%2][y][x]='.';
                } else if (field[cur][y][x]=='.'&&field[cur][y][(x+field[cur][y].size()-1)%field[cur][y].size()]=='>') {
                    moved = true;
                    field[(cur+1)%2][y][x]='>';
                } else {
                    field[(cur+1)%2][y][x]=field[cur][y][x];
                }
            }
        }
        cur = (cur+1)%2;
        for (auto y=0;y<field[cur].size();y++) {
            for (auto x=0;x<field[cur][y].size();x++) {
                if (field[cur][y][x]=='v'&&field[cur][(y+1)%field[cur].size()][x]=='.') {
                    moved = true;
                    field[(cur+1)%2][y][x]='.';
                } else if (field[cur][y][x]=='.'&&field[cur][(y+field[cur].size()-1)%field[cur].size()][x]=='v') {
                    moved = true;
                    field[(cur+1)%2][y][x]='v';
                } else {
                    field[(cur+1)%2][y][x]=field[cur][y][x];
                }
            }
        }
        cur = (cur+1)%2;
        step++;
    } while (moved);

    std::cout<<step<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
