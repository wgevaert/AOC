#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <utility>

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

std::vector<std::pair<u_t,u_t>> adj(u_t i,u_t j) {
    if (i==0&&j==0)
        return {
            {i+1,j},
            {i+1,j+1},
            {i,j+1}
        };
    if (i==0&&j==9)
        return {
            {i+1,j-1},
            {i+1,j},
            {i,j-1}
        };
    if (i==9&&j==0)
        return {
            {i-1,j},
            {i-1,j+1},
            {i,j+1}
        };
    if (i==9&&j==9)
        return {
            {i-1,j},
            {i-1,j-1},
            {i,j-1}
        };
    if (i==0)
        return {
            {i+1,j-1},
            {i+1,j},
            {i+1,j+1},
            {i,j+1},
            {i,j-1}
        };
    if (i==9)
        return {
            {i,j+1},
            {i-1,j+1},
            {i-1,j},
            {i-1,j-1},
            {i,j-1}
        };
    if (j==0)
        return {
            {i+1,j},
            {i+1,j+1},
            {i,j+1},
            {i-1,j+1},
            {i-1,j}
        };
    if (j==9)
        return {
            {i+1,j-1},
            {i+1,j},
            {i-1,j},
            {i-1,j-1},
            {i,j-1}
        };
    return {
        {i+1,j-1},
        {i+1,j},
        {i+1,j+1},
        {i,j+1},
        {i-1,j+1},
        {i-1,j},
        {i-1,j-1},
        {i,j-1}
    };
}

class oct {
public:
    u_t energy;
    bool flashed=false;
    oct (u_t e){energy=e;flashed=false;}
    oct (){}
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

    oct dombos[10][10];
    for(int i=0;i<10;i++){for(int j=0;j<10;j++)dombos[i][j]=oct(input.get()-'0');read_or_die("\n",input);}
    input.close();

    if(verb_lvl>2)for(int i=0;i<10;i++){for(int j=0;j<10;j++)std::cout<<dombos[i][j].energy<<' ';std::cout<<std::endl;}

    const u_t steps=100;
    ull_t flashes=0;
    u_t step=0;
    while(true) {
        step++;
        if(verb_lvl)std::cout<<"Step "<<step<<":\n";
        u_t flashers = 0,cur_fl=0;
        for (int i=0;i<10;i++)
            for (int j=0;j<10;j++) {
                dombos[i][j].flashed=false;
                dombos[i][j].energy++;
                if (dombos[i][j].energy >9)flashers++;
            }

        while(flashers>0) {
            for (int i=0;i<10;i++)
                for(int j=0;j<10;j++) {
                    if (!dombos[i][j].flashed && dombos[i][j].energy > 9) {
                        if(verb_lvl > 3)std::cout<<"Flashed at "<<i<<' '<<j<<std::endl;
                        dombos[i][j].flashed = true;
                        flashes++;
                        cur_fl++;
                        flashers--;
                        for (auto a:adj(i,j)) {
                            dombos[a.first][a.second].energy++;
                            if (!dombos[a.first][a.second].flashed && dombos[a.first][a.second].energy == 10) {
                                flashers++;
                            }
                        }
                    }
                }
        }
        if(verb_lvl>2)std::cout<<"On step "<<step<<", "<<cur_fl<<" dombos flashed"<<std::endl;
        if(cur_fl==100){std::cout<<"All flashed at step "<<step<<std::endl;break;}
        for (int i=0;i<10;i++)for(int j=0;j<10;j++)if(dombos[i][j].flashed)dombos[i][j].energy=0;
        if(step==100)std::cout<<flashes<<std::endl;
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
