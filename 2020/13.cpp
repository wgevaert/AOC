#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned long ul_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

ull_t gcd (ull_t a, ull_t b){
    if(b==0)
        return a;
    return gcd(b, a%b);
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

    ul_t my_time, waiting_time, min_waiting_time=-1, ans1, index=0, new_bus;

    input>>my_time;

    if (input.get()!='\n') {
        std::cerr<<"Newline expected"<<std::endl;
        exit(1);
    }

    ull_t ans2 = 0, mod2 = 1;

    do {
        switch(input.get()) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                input.unget();
                input>>new_bus;
                waiting_time = new_bus - ((my_time-1) % new_bus) - 1;
                if (waiting_time < min_waiting_time) {
                    if (verb_lvl > 3)
                        std::cout<<new_bus<<" has waiting time "<<waiting_time<<'<'<<min_waiting_time<<std::endl;
                    min_waiting_time = waiting_time;
                    ans1 = waiting_time*new_bus;
                }

                if (mod2!=1&&gcd(new_bus,mod2)!=1) {
                    std::cerr<<"HELP"<<new_bus<<','<<mod2<<std::endl;
                    exit(1);
                }

                while ((ans2+index) % new_bus != 0) {
                    if (verb_lvl > 4)
                        std::cout << ans2
                            << " is " << (ans2%new_bus)
                            << " and should be " << ((1+index/new_bus)*new_bus - index)
                            << " mod " << new_bus
                            << ", adding " << mod2 << std::endl;
                    ans2 += mod2;
                }

                mod2 *= new_bus;

                break;
            case 'x':
                break;
            default:
                std::cerr<<"ERROR!"<<std::endl;
                exit(1);
        }
        index++;
    } while(input.get()==',');
    input.close();

    std::cout<<ans1<<' ' <<ans2<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
