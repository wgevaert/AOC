#include <iostream>
#include <fstream>
#include <chrono>
#include <string>/*
#include <vector>
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
    bool seen=false;
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

    const u_t fieldsize = 1000, ropelength = 10, margin=0;//Change ropelength to 10 for part 2, 2 for part 1.
    long rx[ropelength],ry[ropelength];
    for(u_t i=0;i<ropelength;i++)rx[i]=ry[i]=fieldsize/2;
    long xmin=fieldsize/2,xmax=fieldsize/2,ymin=fieldsize/2,ymax=fieldsize/2;
    tile field[fieldsize][fieldsize];

    u_t ans=1;
    field[fieldsize/2][fieldsize/2].seen=true;
    char a=input.get();
    do {
        read_or_die(" ",input);
        long p;
        input>>p;
        read_or_die("\n",input);
        switch(a) {
            case 'U':ry[0]+=p;break;
            case 'D':ry[0]-=p;break;
            case 'L':rx[0]-=p;break;
            case 'R':rx[0]+=p;break;
            default: std::cerr<<"Unexpected char: "<<a<<'('<<static_cast<int>(a)<<')'<<std::endl; return 1;
        }
        if (xmin>rx[0]) xmin=rx[0];
        if (xmax<rx[0]) xmax=rx[0];
        if (ymin>ry[0]) ymin=ry[0];
        if (ymax<ry[0]) ymax=ry[0];

        while (rx[0]-rx[1]>1||rx[1]-rx[0]>1||ry[0]-ry[1]>1||ry[1]-ry[0]>1) {
            for (u_t i=1;i<ropelength;i++) {
                u_t hx = rx[i-1], hy = ry[i-1], tx = rx[i], ty = ry[i];
                if (rx[i-1]-rx[i]>1){
                    rx[i]++;
                    if(ry[i-1]>ry[i])
                        ry[i]++;
                    else if(ry[i-1]<ry[i])
                        ry[i]--;
                } else if (rx[i]-rx[i-1]>1){
                    rx[i]--;
                    if (hy>ty)
                        ry[i]++;
                    else if (hy<ty)
                        ry[i]--;
                } else if (ry[i-1]-ry[i]>1){
                    ry[i]++;
                    if (hx>tx)
                        rx[i]++;
                    else if (hx<tx)
                        rx[i]--;
                } else if (ry[i]-ry[i-1]>1){
                    ry[i]--;
                    if (hx>tx)
                        rx[i]++;
                    else if (hx<tx)
                        rx[i]--;
                }
            }
            if (!field[ry[ropelength-1]][rx[ropelength-1]].seen) {
                field[ry[ropelength-1]][rx[ropelength-1]].seen=true;
                ans++;
            }
        }
        if (verb_lvl > 5){for(auto i=ymax+margin;i>=ymin-margin;i--){for(auto j=xmin-margin;j<=xmax+margin;j++) {
            char d=field[i][j].seen?'#':'.';
            for (u_t k=1;k<ropelength;k++)if(rx[k]==j&&ry[k]==i){d=k+'0';}
            d=(rx[0]==j&&ry[0]==i?'H':(rx[ropelength-1]==j&&ry[ropelength-1]==i?'T':d));
            std::cout<<d;
        }std::cout<<std::endl;}std::cout<<std::endl;}
        a=input.get();
    } while ('A'<=a&&a<='Z');

    std::cout<<ans<<std::endl;

    input.close();

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
