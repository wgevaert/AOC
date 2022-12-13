#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

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

ull_t calc_cost(u_t a, u_t b) {
    if (a>=b)return (a-b)*(a-b+1)/2;
    return calc_cost(b,a);
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

    const size_t mysize = 2000;
    static ull_t fuel_cost[mysize], fuel_cost2[mysize];
    do {
        u_t i;
        input>>i;
        if (i>mysize){std::cerr<<"Mysize "<<mysize<<" not sufficient for "<<i<<std::endl;exit(1);}
        for(u_t a=0;a<mysize;a++) {
            if (a<i) fuel_cost[a]+=i-a;else fuel_cost[a] +=a-i;
            fuel_cost2[a]+=calc_cost(a,i);
        }
    } while (input.get()!='\n');

    input.close();

    ull_t min=-1,min2=-1;
    for(u_t a=0;a<mysize;a++){if(fuel_cost[a]<min)min=fuel_cost[a];if(verb_lvl>1)std::cout<<fuel_cost[a]<<' ';}
    std::cout<<min<<std::endl;
    for(u_t a=0;a<mysize;a++){if(fuel_cost2[a]<min2)min2=fuel_cost2[a];if(verb_lvl>1)std::cout<<fuel_cost2[a]<<' ';}
    std::cout<<min2<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
