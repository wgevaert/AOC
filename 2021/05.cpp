#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

void read_or_die(std::istream& input,std::string pattern) {
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

    static u_t field[1000][1000];
    bool part_1=false; //change for part 2

    while(!input.eof()) {
        u_t x0,y0,x1,y1;
        input>>x0;
        if (input.eof())break;
        read_or_die(input,",");
        input>>y0;
        read_or_die(input," -> ");
        input>>x1;
        read_or_die(input,",");
        input>>y1;
        read_or_die(input,"\n");

if(verb_lvl>2){std::cout<<"\nfor "<<x0<<','<<y0<<" -> "<<x1<<','<<y1<<std::endl;}
        while (x0!=x1||y0!=y1) {
            if (!part_1 || x0==x1 || y0 == y1) {
                field[x0][y0]++;
                if (x0>x1)x0--;else if (x0<x1)x0++;
                if (y0>y1)y0--;else if (y0<y1)y0++;
            } else break;
        }
        if (x0==x1&&y0==y1)field[x0][y0]++;
if(verb_lvl>2)for(int a=0;a<20;a++){for(int b=0;b<20;b++)std::cout<<field[b][a]<<' ';std::cout<<"\n";}
    }

    // do things with input.

    input.close();

    ull_t result = 0;
    for(int a=0;a<1000;a++)for(int b=0;b<1000;b++)result +=field[a][b]>1;
    std::cout<<result<<std::endl;

if(verb_lvl>2)for(int a=0;a<20;a++){for(int b=0;b<20;b++)std::cout<<field[b][a]<<' ';std::cout<<"\n";}
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
