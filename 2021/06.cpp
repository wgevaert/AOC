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

    // do things with input.
    ull_t *fish = new ull_t[9] {0,0,0,0,0,0,0,0,0};

    do {
        u_t i;
        input>>i;
        fish[i]++;
    } while(input.get() != '\n');
    input.close();

    ull_t *new_fish = new ull_t[9] {0,0,0,0,0,0,0,0,0};
    const u_t days=256;
    for (u_t d=0;d<days;d++){
        for (u_t f=1;f<9;f++)new_fish[f-1]=fish[f];
        new_fish[6] += fish[0];
        new_fish[8] = fish[0];
        if(verb_lvl>1){for(u_t i=0;i<9;i++)std::cout<<fish[i]<<' ';std::cout<<std::endl;}
        ull_t *tmp=fish;
        fish=new_fish;
        new_fish=tmp;
    }
    ull_t r=0;
    for(int i=0;i<9;i++)r+=fish[i];
    std::cout<<r<<std::endl;
    delete[] fish;
    delete[] new_fish;

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
