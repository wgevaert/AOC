#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <set>
#include <utility>

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
    std::set<std::pair<u_t,u_t>> dots[2] = {{},{}};
    u_t cd=1;//current dots
    char a;
    do {
        u_t i,j;
        input>>i;if(input.eof())break;read_or_die(",",input);input>>j;read_or_die("\n",input);
        if(verb_lvl>2)std::cout<<i<<' '<<j<<std::endl;
        dots[cd].emplace(i,j);
    } while(input.peek()!='\n');

    input.get();
    u_t fl=0;//folds
    do {
        if (input.eof())break;
        read_or_die("fold along ",input);
        bool along=input.get()=='x';
        read_or_die("=",input);
        u_t l;
        input>>l;
        read_or_die("\n",input);
        if(verb_lvl>2)std::cout<<"folding "<<along<<' '<<l<<std::endl;
        if(verb_lvl>9)for (auto a:dots[cd])std::cout<<a.first<<' '<<a.second<<std::endl;
        if(verb_lvl>8){for (int i=0;i<100;i++){for(int j=0;j<80;j++)if(dots[cd].count(std::pair<u_t,u_t>(j,i)))std::cout<<'@';else std::cout<<'.';std::cout<<' '<<i<<std::endl;}}
        if (along) {
            for (auto& a:dots[cd]) {
                if (a.first > l) {
                    dots[(cd+1)%2].emplace(l-(a.first-l),a.second);
                } else {
                    dots[(cd+1)%2].insert(a);
                }
            }
        } else {
            for (auto& a:dots[cd]) {
                if (a.second > l) {
                    dots[(cd+1)%2].emplace(a.first,l-(a.second-l));
                } else {
                    dots[(cd+1)%2].insert(a);
                }
            }
        }
        dots[cd].clear();
        cd = (cd+1)%2;
        if (!fl) {
            ull_t rtr=0;
            for(auto a:dots[cd])rtr++;
            std::cout<<rtr<<" Is the answer to part 1"<<std::endl;
        }

        fl++;
    } while (input.peek()!='\n');

    for (int i=0;i<10;i++){for(int j=0;j<40;j++)if(dots[cd].count(std::pair<u_t,u_t>(j,i)))std::cout<<'@';else std::cout<<' ';std::cout<<' '<<i<<std::endl;}

    input.close();

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
