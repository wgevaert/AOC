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

constexpr ll_t myabs(const ll_t x) {
    if (x<0)return -1*x;
    return x;
}

constexpr ll_t dist(const ll_t x, const ll_t y, const ll_t ox, const ll_t oy) {
    return myabs(x-ox)+myabs(y-oy);
}

class unit {
public:
    ll_t sx,sy,bx,by, d;
    unit(ll_t SX, ll_t SY, ll_t BX, ll_t BY): sx(SX), sy(SY), bx(BX), by(BY), d(dist(sx,sy,bx,by)) {}
    bool allowed(ll_t x, ll_t y) {
        return dist(sx,sy,x,y) > d;
    }
    ll_t next_x(ll_t x, ll_t y) {
        if (allowed(x,y))return x+1;
        return sx + d - abs(sy-y);
    }
};

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

    std::vector<char> at2000000y;
    at2000000y.resize(7000000,'.'); //big array

    const ll_t x_off = 1000000;
    ull_t ans1=0;

    std::vector<unit> units = {};

    while (input.peek()=='S') {
        ll_t sx,sy,bx,by,d, xmin,xmax;
        read_or_die("Sensor at x=",input);
        input>>sx;
        read_or_die(", y=",input);
        input>>sy;
        read_or_die(": closest beacon is at x=",input);
        input>>bx;
        read_or_die(", y=",input);
        input>>by;
        read_or_die("\n",input);

        units.emplace_back(sx,sy,bx,by);

        d=dist(sx,sy,bx,by);
        xmin = sx - d + myabs(sy-2000000);
        xmax = sx + d - myabs(sy-2000000);

        if (by==2000000) {
            if (at2000000y[bx+x_off]=='#') {
                ans1--;
            }
            at2000000y[bx+x_off]='B';
        }

        if (xmin<=xmax) {
            if (xmin+x_off<0){std::cerr<<"HElp!"<<std::endl;exit(1);}
            if (xmax+x_off>=7000000){std::cerr<<"HElp!"<<std::endl;exit(1);}
            for (ll_t i=xmin;i<=xmax;i++) if (at2000000y[i+x_off]=='.'){at2000000y[i+x_off]='#';ans1++;}
        }
    }
    input.close();

    for (ll_t y=0;y<=4000000;y++)for (ll_t x=0;x<=4000000;x++) {
        bool allowed = true;
        for (auto a:units) {
            if (!a.allowed(x,y)) {
                allowed = false;
                x = a.next_x(x,y);
                if (x>4000000)x=4000000;
                break;
            }
        }
        if (allowed) {
            std::cout<<"Ans 2 "<<x<<' '<<y<<" gives "<<(4000000*x+y)<<std::endl;
        }
    }

    std::cout<<ans1<<std::endl;
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
