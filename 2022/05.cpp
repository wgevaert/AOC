#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>/*
#include <unordered_map>*/

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;
bool part2=true;//change for part 1;

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

std::vector<char> stacks[9];
std::vector<std::string> arrment;
std::string in;

    do {
        std::getline(input,in);
        arrment.push_back(in);
    } while (in[1]!='1');

    for (u_t j=arrment.size()-1;j+1!=0;j--)for(u_t i=0;i<9;i++){u_t k=4*i+1;if(arrment[j][k]!=' ')stacks[i].push_back(arrment[j][k]);}

    read_or_die("\n",input);
    while(input.peek()!='\n'&&!input.eof()) {
        u_t amount, from, to;
        read_or_die("move ",input);
        input>>amount;read_or_die(" from ",input);input>>from;read_or_die(" to ",input);input>>to;read_or_die("\n",input);

        if (stacks[from-1].size()<amount) {std::cerr<<"Cannot move "<<amount<<" from stack of size "<<stacks[from-1].size()<<std::endl;return 1;}

        if (! part2) {
            for (u_t i=0;i<amount;i++){
                stacks[to-1].push_back((stacks[from-1].back()));
                stacks[from-1].pop_back();
            }
        } else {
            for (u_t i=stacks[from-1].size()-amount;i<stacks[from-1].size();i++)stacks[to-1].push_back(stacks[from-1][i]);
            for (u_t i=0;i<amount;i++)stacks[from-1].pop_back();
        }
    }

    for (u_t i=0;i<9;i++){for(auto a:stacks[i])std::cout<<a<<' ';std::cout<<std::endl;}

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
