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

    const u_t number=14; // Change number to 4 for part 1;
    u_t ans=number-1;
    bool distinct;
    char mychars[number];
    for (u_t j=1;j<number;j++) mychars[j]=input.get();

    do {
        for (u_t j=0;j<number-1;j++)mychars[j]=mychars[j+1];
        mychars[number-1]=input.get();
        ans++;

        distinct = true;
        for (u_t j=0;distinct&&j<number-1;j++)for (u_t k=j+1;k<number;k++)if(mychars[j]==mychars[k]){
            distinct=false;
            break;
        }
    } while (!distinct && 'a'<=mychars[number-1]&&mychars[number-1]<='z');

    for (auto a:mychars)std::cout<<a;std::cout<<':'<<ans<<std::endl;

    /* my first implementation for part 1, isn't it beatiful?
    char prevprevprev=input.get(),prevprev=input.get(),prev=input.get(),cur=input.get();

    u_t ans1=4;

    while ('a'<=cur&&cur<='z'&&(prevprevprev==prevprev||prevprevprev==prev||prevprevprev==cur||prevprev==prev||prevprev==cur||prev==cur)) {
        prevprevprev=prevprev;prevprev=prev;prev=cur;cur=input.get();
        ans1++;
    }

    std::cout<<prevprevprev<<prevprev<<prev<<cur<<':'<<ans1<<std::endl;
    */
    // do things with input.

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
