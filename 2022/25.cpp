#include <iostream>
#include <fstream>
#include <chrono>
#include <string>/*
#include <vector>
#include <unordered_map>*/

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

std::string to_snafu(ull_t x) {
    u_t next = 0, cur;
    std::string rtr = "";
    while( x > 0 || next != 0 ) {
        cur = x%5 + next;
        next = 0;
        x /= 5;
        if (cur>=5) { // might happen if x%5 == 4 and next >= 1
            cur -= 5;
            next++;
        }
        switch(cur) {
            case 4:
                next++;
                rtr = std::string(1, '-') + rtr;
                break;
            case 3:
                next++;
                rtr = std::string(1,'=') + rtr;
                break;
            case 2:
                rtr = std::string(1, '2') + rtr;
                break;
            case 1:
                rtr = std::string(1, '1') + rtr;
                break;
            case 0:
                rtr = std::string(1, '0') + rtr;
                break;
            default:
                std::cerr<<"Unexpected: "<<cur<<std::endl;
                exit(1);
        }
    }
    return rtr;
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

    char a=input.get();
    ull_t ans=0,sub=0;
    do {
        if(input.eof())break;
        do {
            switch(a) {
                case '2':
                    sub=5*sub+2;
                    break;
                case '1':
                    sub=5*sub+1;
                    break;
                case '0':
                    sub=5*sub;
                    break;
                case '-':
                    sub=5*sub-1;
                    break;
                case '=':
                    sub=5*sub-2;
                    break;
                default:
                    std::cerr<<"Unexpected "<<a<<static_cast<int>(a)<<std::endl;
                    exit(1);
            }
            a = input.get();
        } while(a != '\n'&&!input.eof());
        ans += sub;
        sub = 0;
        a = input.get();
    } while(a!='\n');
    input.close();

    std::cout<<to_snafu(ans)<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
