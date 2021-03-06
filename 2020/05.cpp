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


    static bool exists[1024]; // init to 0
    u_t step=512, index=0, ind_max=0;
    bool reading=true;

    while(reading) {
        switch(input.get()) {
            case 'B':
            case 'R':
                index |= step;
                // no break
            case 'F':
            case 'L':
               step /= 2;
               break;
            case '\n':
                if(input.peek() == '\n' || input.peek() == -1) {
                    reading = false;
                }
                exists[index] = true;
                if (index > ind_max)
                    ind_max = index;
                step = 512;
                index = 0;
                break;
            default:
                input.unget();
                std::cout<<"Unexpected char with value "<<static_cast<int>(input.get())<<" found"<<std::endl;
                return 1;
        }
    }

    input.close();

    std::cout<<ind_max<<std::endl;

    for(int i=1; i<1023; i++){
        if (!exists[i] && exists[i-1] && exists[i+1]){
            std::cout<<i<<std::endl;
        }
    }
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc, argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
