#include <iostream>
#include <fstream>
#include <chrono>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

const uint32_t steps = 30000000, steps1 = 2020;

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

    static uint32_t prev_time[steps + 1];

    uint32_t index = 0, in, prev_number = -1, prevprev = -1;

    do {
        input >> in;
        if (prev_number <= steps) {
            prev_time[prev_number] = index;
        }
        if (verb_lvl > 4) {
            std::cout << in << std::endl;
        }
        prev_number = in;
        index++;
    } while (input.get() == ',');

    input.close();

    while (index < steps) {
        prevprev = prev_number;
        if (prev_time[prev_number]) {
            if (verb_lvl > 5) {
                std::cout<<prev_number<<" has been seen on "<<prev_time[prev_number]<<std::endl;
            }
            prev_number = index - prev_time[prev_number];
        } else {
            if (verb_lvl > 5)
                std::cout<<prev_number<<" is new"<<std::endl;
            prev_number = 0;
        }
        prev_time[prevprev] = index;
        if (verb_lvl > 4)
            std::cout<<prev_number<<std::endl;
        index++;
        if (index == steps1)
            // Answer to part 1
            std::cout<<prev_number<<std::endl;
    }

    std::cout<<prev_number<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
