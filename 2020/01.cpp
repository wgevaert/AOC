#include <iostream>
#include <fstream>
#include <chrono>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

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

    bool in_list[2020];

    for (u_t i=0;i<2020;i++) {
        in_list[i]=false;
    }

    u_t number;

    while(!input.eof()) {
        input>>number;
        in_list[number] = true;

        if (verb_lvl > 1) {
            std::cout<<"Read number "<<number<<std::endl;
        }

        if (in_list[2020-number]) {
            std::cout<<"Part 1: "<<(number * (2020-number)) <<" with i="<<number<<" and j="<<(2020-number)<<std::endl;
        }
    }

    input.close();

    // all info we need for part 2 is now in in_list.

    for (u_t i=1;i<2020/3;i++) for(u_t j=i+1;j<2020/2;j++)
        if (in_list[i] && in_list[j] && in_list[2020-i-j]) {
            std::cout<<"Part 2: "<<(i*j*(2020-i-j))<<" with i="<<i<<" and j="<<j<<" and k="<<(2020-i-j)<<std::endl;
        } else if (verb_lvl > 2) {
            std::cout<<"Trying "<<i<<','<<j<<std::endl;
        }

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
