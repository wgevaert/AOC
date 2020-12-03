#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;
const u_t MAX_SIZE = 500;


ull_t check_slope(bool trees[][MAX_SIZE],u_t x_max, u_t y_max, u_t sx, u_t sy) {
    ull_t tree_cnt=0;
    u_t i=0;
    for (u_t j=0;j<y_max;j+=sy) {
        if (trees[i][j])
            tree_cnt++;
        i=(i+sx)%x_max;
    }
    return tree_cnt;
}


int main(int argc, char** argv) {
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

    static bool trees[500][500]; // initialised to 0 as is static.
    bool reading = true;
    u_t x=0,y=0,x_max=0,y_max=0;
    while (reading) {
        switch (input.get()) {
            case '#':
                trees[x][y]=true;
                // no break
            case '.':
                x++;
                x_max = std::max(x_max,x);
                break;
            case '\n':
                if (input.peek() == '\n' || input.eof()) {
                    reading = false;
                }
                x=0; y++;
                y_max=y;
                break;
            default:
                input.unget();
                std::cerr<<"Unexpected char with value "<<static_cast<int>(input.get())<<" found\n"<<std::flush;
        }
    }

    input.close();

    u_t tree_cnt = 0;

    for (u_t j=0;j<y_max;j++) {
        for (u_t i=0;i<x_max;i++) {
            if ((j*3)%x_max == i) {
                if (trees[i][j]) {
                    tree_cnt++;
                    if(verb_lvl > 0) {
                        std::cout<<'X';
                    }
                } else if (verb_lvl >0) {
                    std::cout<<'O';
                }
            } else if (verb_lvl > 0) {
                std::cout<<(trees[i][j] ? '#' : '.');
            }
        }
        if (verb_lvl > 0)
            std::cout<<std::endl;
    }

    std::cout<<tree_cnt<<std::endl;

    assert(check_slope(trees,x_max,y_max,3,1) == tree_cnt); // make Bert happy.

    std::vector<std::pair<u_t,u_t>> other_slopes = {{1, 1},{5,1},{7,1},{1,2}};

    ull_t ans_2=tree_cnt;
    for (auto [sx,sy] : other_slopes) {
        ans_2 *= check_slope(trees,x_max,y_max,sx,sy);
    }

    std::cout<<ans_2<<std::endl;

    return 0;
}
