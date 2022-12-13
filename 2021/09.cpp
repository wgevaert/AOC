#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

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
    u_t x,y;
    flower(u_t i, u_t j){x=i;y=j;}
};//something that flows.

ull_t determine_basin(u_t i, u_t j, u_t field[][102]) {
    static bool visited[102][102];
    
    std::vector<flower> flowers[2] = {{},{}};
    u_t cf=0;//current flower
    flowers[cf].emplace_back(i,j);
    ull_t s=0;
    while (flowers[cf].size()) {
        for (auto f:flowers[cf]) {
            // lazy solution: make too much flowers.
            if (!visited[f.x][f.y] && field[f.x][f.y]<9) {
                s++;
                visited[f.x][f.y]=true;
                flowers[(cf+1)%2].emplace_back(f.x+1,f.y);
                flowers[(cf+1)%2].emplace_back(f.x-1,f.y);
                flowers[(cf+1)%2].emplace_back(f.x,f.y-1);
                flowers[(cf+1)%2].emplace_back(f.x,f.y+1);
            }
        }
        flowers[cf].clear();
        cf = (cf+1)%2;
    }
    return s;
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

    static u_t in[102][102], max=9;
    char a;
    size_t i=0,jmax=0;
    while ((a=input.get()) != '\n' && !input.eof()) {
        size_t j=0;
        while(a != '\n'&&!input.eof()) {
            in[i+1][j+1] = a-'0';
            j++;
            if(j>jmax)jmax=j;
            a=input.get();
        }
        i++;
    }
    input.close();

    for (u_t I=0;I<=i+1;I++) {
        in[I][0]=max;
        in[I][jmax+1]=max;
    }
    for(u_t j=0;j<=jmax+1;j++) {
        in[0][j]=max;
        in[i+1][j]=max;
    }

    u_t s=0;//score
    std::vector<ull_t> basin_sizes = {};
    for (u_t I=1;I<=i;I++) {
        for (u_t j=1;j<=jmax;j++) {
            if (
                  in[I][j] < in[I][j+1]
                &&in[I][j] < in[I][j-1]
                &&in[I][j] < in[I+1][j]
                &&in[I][j] < in[I-1][j]
            ) {
                s+=in[I][j]+1;
                basin_sizes.emplace_back(determine_basin(I,j,in));
            }
        }
    }
    std::cout<<s<<std::endl;
    std::sort(basin_sizes.rbegin(),basin_sizes.rend());
    std::cout<<(basin_sizes[0]*basin_sizes[1]*basin_sizes[2])<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
