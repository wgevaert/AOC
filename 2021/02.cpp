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
    ll_t hor=0,vert=0,in,hor2=0,vert2=0,aim=0;
    while (!input.eof()) {
        char a=input.get();
        switch(a) {
            case 'f':
                read_or_die("orward ",input);
                input>>in;
                hor += in;
                hor2 += in;
                vert2 += in*aim;
                break;
            case 'd':
                read_or_die("own ",input);
                input>>in;
                vert += in;
                aim += in;
                break;
            case 'u':
                read_or_die("p ",input);
                input>>in;
                vert -= in;
                aim -= in;
                break;
            default:
                std::cout<<" Read "<< a<<'('<<static_cast<int>(a)<<')'<<std::endl;
        }
        if (!input.eof()) {
            read_or_die("\n",input);
        }
    }
    std::cout<<(vert*hor)<<' '<<vert<<' '<<hor<<' '<<(vert2*hor2)<<' '<<vert2<<' '<<hor2<<std::endl;

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
