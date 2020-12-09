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

    ull_t valids1 = 0, valids2 = 0, min,max, checkcount, index;
    char check,a;

    while(true) {
        input>>min;

        if (verb_lvl > 0) {
            std::cout<<"Min is "<<min<<std::endl;
        }

        if (input.get() != '-') {
            if (input.eof()) {
                break;
            }
            std::cerr<<"Weird..."<<std::endl;
            exit(1);
        }
        input>>max;


        if (verb_lvl > 0) {
            std::cout<<"Max is "<<min<<std::endl;
        }

        if (input.get()!=' '){
            std::cerr<<"Weird...."<<std::endl;
            exit(1);
        }

        check=input.get();
        if (input.get()!=':' || input.get()!=' '){
            std::cerr<<"Weird....."<<std::endl;
            exit(1);
        }
        if (verb_lvl > 0) {
            std::cout<<"check is "<<check<<std::endl;
        }

        checkcount=0;
        index = 1;
        bool found = false;

        while((a=input.get())!='\n') {
            if (a==check) {
                checkcount++;
                if (index==min || index==max)
                    found = !found;
            }
            index++;
        }

        if (verb_lvl > 0) {
            std::cout<<"checkcount is "<<checkcount<<" and found is "<<(found ? "true":"false")<<std::endl;
        }

        if (checkcount >= min && checkcount <= max) {
            valids1++;
        }

        if (found) {
            valids2++;
        }
    }
    std::cout<<"Found "<<valids1<<' '<<valids2<<" valid passwords"<<std::endl;

    input.close();

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
