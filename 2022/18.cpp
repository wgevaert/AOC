#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>/*
#include <unordered_map>*/
#include <algorithm>

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

class flower {
public:
    int x,y,z;
    flower(){}
    flower(int X, int Y, int Z): x(X),y(Y),z(Z) {}
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

    u_t x,y,z;
    const int max=30;
    static bool blocks[max][max][max];//static means all false

    ull_t ans1=0;
    while (std::isdigit(input.peek())) {
        input>>x;input.get();input>>y;input.get();input>>z;input.get();
        ans1+=6;
        if (x+1>=max||y+1>=max||z+1>=max) {
            std::cerr<<max<<" too small for "<<x<<','<<y<<','<<z<<std::endl;exit(1);
        }
        blocks[x+1][y+1][z+1]=true;
        if (blocks[x+2][y+1][z+1])ans1-=2;
        if (blocks[x][y+1][z+1])ans1-=2;
        if (blocks[x+1][y+2][z+1])ans1-=2;
        if (blocks[x+1][y][z+1])ans1-=2;
        if (blocks[x+1][y+1][z+2])ans1-=2;
        if (blocks[x+1][y+1][z])ans1-=2;
    }
    input.close();
    std::cout<<ans1<<std::endl;

    std::vector<flower> flowers[2] = {{flower(0,0,0)},{}};
    blocks[0][0][0]=true;
    u_t cur=0;

    while (flowers[cur].size()) {
        for (auto a:flowers[cur]) {
            // use std::max and std::min instead of if/else for convenience, even though if/else should result in faster code if compiler doesn't have galaxy brain
            for (int i=std::max(0,a.x-1);i<std::min(max,a.x+2);i++) {
                if (!blocks[i][a.y][a.z]) {
                    blocks[i][a.y][a.z] = true;
                    flowers[(cur+1)%2].emplace_back(i,a.y,a.z);
                }
            }
            for (int j=std::max(0,a.y-1);j<std::min(max,a.y+2);j++) {
                if (!blocks[a.x][j][a.z]) {
                    blocks[a.x][j][a.z] = true;
                    flowers[(cur+1)%2].emplace_back(a.x,j,a.z);
                }
            }
            for (int k=std::max(0,a.z-1);k<std::min(max,a.z+2);k++) {
                if (!blocks[a.x][a.y][k]) {
                    blocks[a.x][a.y][k] = true;
                    flowers[(cur+1)%2].emplace_back(a.x,a.y,k);
                }
            }
        }
        flowers[cur].clear();
        cur = (cur+1)%2;
    }
    for (u_t i=1;i<max-1;i++)for(u_t j=1;j<max-1;j++)for(u_t k=1;k<max-1;k++)if(!blocks[i][j][k]){
        if (blocks[i+1][j][k])ans1--;
        if (blocks[i-1][j][k])ans1--;
        if (blocks[i][j+1][k])ans1--;
        if (blocks[i][j-1][k])ans1--;
        if (blocks[i][j][k+1])ans1--;
        if (blocks[i][j][k-1])ans1--;
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
