#include <iostream>
#include <fstream>
#include <chrono>
#include <array>

#define NORTH 0
#define EAST 1
#define SOUTH 2
#define WEST 3

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

typedef std::pair<ll_t,ll_t> pos_t;

const std::array<pos_t, 4> directions = {
    std::make_pair(1,0), // north
    std::make_pair(0,1), // east
    std::make_pair(-1,0), // south
    std::make_pair(0,-1) // west
};

pos_t operator* (ll_t mult, pos_t pos) {
    return pos_t(pos.first * mult, pos.second * mult);
}

pos_t operator+ (pos_t a, pos_t b) {
    return pos_t(a.first + b.first, a.second + b.second);
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

    u_t p1_dir = EAST;
    pos_t p1_pos(0,0), ship_pos(0,0), way_pos = 10*directions[EAST]+directions[NORTH];

    while (!input.eof()) {
        ll_t move_dist;
        pos_t move_dir;
        switch(input.get()) {
            case 'N':
                move_dir = directions[NORTH];
                input>>move_dist;
                break;
            case 'W':
                move_dir = directions[WEST];
                input>>move_dist;
                break;
            case 'S':
                move_dir = directions[SOUTH];
                input>>move_dist;
                break;
            case 'E':
                move_dir = directions[EAST];
                input>>move_dist;
                break;
            case 'F':
                input>>move_dist;
                ship_pos = ship_pos + move_dist * way_pos;
                p1_pos = p1_pos + move_dist * directions[p1_dir];
                move_dist = 0;
                break;
            case 'R':
                // misusing my variable
                input>>move_dist;
                switch (move_dist){
                    case 90:
                        p1_dir = (p1_dir + 1)%4;
                        way_pos = pos_t(-1*way_pos.second, way_pos.first);
                        break;
                    case 180:
                        p1_dir = (p1_dir + 2)%4;
                        way_pos = -1*way_pos;
                        break;
                    case 270:
                        p1_dir = (p1_dir + 3)%4;
                        way_pos = pos_t(way_pos.second, -1*way_pos.first);
                        break;
                    default:
                        std::cerr<<"CANNOT TURN BY "<<move_dist<<" DEGREES!"<<std::endl;
                        exit(1);
                }
                move_dist=0;
                break;
            case 'L':
                // misusing my variable
                input>>move_dist;
                switch (move_dist){
                    case 90:
                        p1_dir = (p1_dir + 3)%4;
                        way_pos = pos_t(way_pos.second, -1*way_pos.first);
                        break;
                    case 180:
                        p1_dir = (p1_dir + 2)%4;
                        way_pos = -1*way_pos;
                        break;
                    case 270:
                        p1_dir = (p1_dir + 1)%4;
                        way_pos = pos_t(-1*way_pos.second, way_pos.first);
                        break;
                    default:
                        std::cerr<<"CANNOT TURN BY "<<move_dist<<" DEGREES!"<<std::endl;
                        exit(1);
                }
                move_dist=0;
                break;
        }

        if (move_dist) {
            p1_pos = p1_pos + move_dist * move_dir;
            way_pos = way_pos + move_dist * move_dir;
        }

        if (verb_lvl > 3) {
            std::cout<<"Now at "<<p1_pos.first<<", "<<p1_pos.second<<" for part 1"<<std::endl;
            std::cout<<"Now at "<<ship_pos.first<<", "<<ship_pos.second<<" with waypoint "<<way_pos.first<<", "<<way_pos.second<<" for part 2"<<std::endl;
        }
    }

    input.close();
    std::cout<<(std::max(p1_pos.first, -1 * p1_pos.first) + std::max(p1_pos.second, -1 * p1_pos.second))<<' '
        <<(std::max(ship_pos.first, -1 * ship_pos.first) + std::max(ship_pos.second, -1 * ship_pos.second))<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
