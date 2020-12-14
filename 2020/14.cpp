#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <cstring>
#include <string>
#include <vector>
#include <bitset>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

void write_value(
    const uint64_t value,
    std::unordered_map<uint64_t, uint64_t>& program,
    const uint64_t index,
    std::vector<uint64_t>& floaters
) {
    if (floaters.size()) {
        uint64_t floater = floaters.back();
        floaters.pop_back();
        write_value(value, program, index & ~ floater, floaters);
        write_value(value, program, index | floater, floaters);
        floaters.emplace_back(floater);
    } else {
        program[index] = value;
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

    uint64_t zeroes = 0xffff, ones = 0x0000, index = 0x8000;

    std::unordered_map<uint64_t,uint64_t> program1 = {}, program2 = {};

    std::vector<uint64_t> floaters;

    while (input.get()=='m') {
        switch(input.get()) {
            case 'a':
                zeroes = 0xffffffffffffffff;
                ones = 0x0000000000000000;
                floaters.clear();
                index = static_cast<uint64_t>(1) << static_cast<uint64_t>(35);
                if (
                    input.get() != 's' ||
                    input.get() != 'k' ||
                    input.get() != ' ' ||
                    input.get() != '=' ||
                    input.get() != ' '
                ) {
                    std::cerr<<"did not find sk = "<<std::endl;
                    exit(1);
                }
                while (index) {
                    switch (input.get()) {
                        case 'X':
                            floaters.emplace_back(index);
                            break;
                        case '1':
                            ones |= index;
                            break;
                        case '0':
                            zeroes &= ~index;
                            break;
                        default:
                            std::cerr<<"Weird..."<<std::endl;
                            exit(1);
                    }
                    index>>=1;
                    if (verb_lvl > 8)
                        std::cout<<index<<std::endl;
                }
                if (verb_lvl > 4) {
                    std::cout<<"Found zeroes: "<<std::bitset<64>(zeroes)<<std::endl;
                    std::cout<<"Found ones:   "<<std::bitset<64>(ones)<<std::endl;
                }
                break;
            case 'e':
                if (
                    input.get()!='m' ||
                    input.get()!='['
                ){
                    std::cerr<<"did not find m["<<std::endl;
                    exit(1);
                }
                input >> index;
                if (
                    input.get()!=']' ||
                    input.get()!=' ' ||
                    input.get()!='=' ||
                    input.get()!=' '
                ) {
                    std::cerr<<"did not find ] = "<<std::endl;
                    exit(1);
                }
                input >> program1[index];

                write_value(program1[index], program2, index | ones, floaters);

                // Apply mask to program entry.
                program1[index] |= ones;
                program1[index] &= zeroes;

                break;
            default:
                std::cerr<<"Very weird..."<<std::endl;
                exit(1);
        }
        if (input.get()!='\n') {
            std::cerr<<"newline expected..."<<std::endl;
            exit(1);
        }
    }

    input.close();

    uint64_t ans1 = 0, ans2=0;

    for (auto a : program1) {
        ans1+=a.second;
    }

    for (auto a : program2) {
        ans2+=a.second;
    }

    std::cout<<ans1<<' '<<ans2<<std::endl;
    // do things without input
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
