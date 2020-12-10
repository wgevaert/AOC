#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <algorithm>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

typedef unsigned long adapter_t;

unsigned verb_lvl = 0;

ull_t give_local_perms(u_t length) {
    static std::vector<ull_t> answers = {1};//{1, 1, 2, 4, 7};
    static std::vector<bool> seen = {true};//{true, true, true, true, true};
    if (length<seen.size() && seen[length]) {
        if (verb_lvl > 2) {
            std::cout<<"Answer from cache for length "<<length<<": "<<answers[length]<<std::endl;
        }
        return answers[length];
    }

    // In my input, I only have lengths of size <=4, so nothing interesting happens...
    // But this _should_ also work for bigger lengths.

    // number of local perms = 1<<(length-1) - (number of local perms with gap of size 4)
    ull_t ans = 1<<(length-1);
    ull_t bad_perms = 0;
    for (int i=0; i+4<=length; i++) {
        // number of local perms with gap of size 4 = sum over i of number of local perms with gap at i and no gap before i.
        if (verb_lvl > 1) {
            std::cout<<" Doing things for i="<<i<<" at length "<<length<<std::endl;
        }
        bad_perms += give_local_perms(i)*(1<<(length-(i+4)));
    }

    if (bad_perms > ans) {
        std::cerr<<"ERROR: "<<bad_perms <<'>'<<ans<<std::endl;exit(1);
        exit(1);
    }

    ans -= bad_perms;

    if (answers.size() < length) {
        // I got errors for some reason for length + 1...
        answers.resize(length + 4);
        seen.resize(length + 1, false);
    }

    answers[length] = ans;
    seen[length] = true;

    return ans;
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

    adapter_t one;

    std::vector<adapter_t> adapters;

    std::vector<int> difs={};
    while (!input.eof()) {
        input>>one;
        if (!input.eof()) {
            adapters.emplace_back(one);
        }
    }

    input.close();

    std::sort(adapters.begin(),adapters.end());
    difs.reserve(adapters.size()+1);

    one = 0;
    ull_t ones=0, threes=1;
    for (auto a:adapters) {
        difs.emplace_back(a-one);
        if (difs.back() == 1) ones++;
        else if (difs.back() == 3) threes++;
        one=a;
        if (verb_lvl > 0) {
            std::cout<<'('<<difs.back()<<')'<<a;
        }
    }
    difs.emplace_back(3);
    if (verb_lvl > 0) {
        std::cout<<'('<<difs.back()<<')'<<std::endl;
    }

    std::cout<<(ones*threes)<<std::endl;

    ull_t ans2=1;

    u_t length = 0;
    for (auto a:difs) {
        if (a==1) {
            length++;
        } else if (a==3) {
            if (verb_lvl > 1) {
                std::cout<<"Next: "<<length<<" with ans before: "<<ans2<<std::endl;;
            }
            ans2*=give_local_perms(length);
            length = 0;
        } else {
            // note: My input only has difs of one and three, never 2.
            // This makes me very happy :D
            std::cerr<<"Diff of "<<a<<" unexpected"<<std::endl;
            exit(1);
        }
    }

    std::cout<<ans2<<std::endl;
    // do things without input
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
