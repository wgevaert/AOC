#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

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

std::string get_ox_rating(std::vector<std::string>& ins, size_t pos) {
    if (ins.size()<=1) {
        return ins[0];
    }
    std::vector<std::string> mynew = {};
    ull_t cnt = 0;
    for(auto a:ins) {
        cnt += a[pos]=='1';
    }
    for (auto a:ins)
        if ( (cnt*2 >= ins.size()) != (a[pos]=='0')) {
           mynew.push_back(a);
        }
    return get_ox_rating(mynew, pos+1);
}

std::string get_co_rating(std::vector<std::string>& ins, size_t pos) {
    if (ins.size()<=1) {
        return ins[0];
    }
    std::vector<std::string> mynew = {};
    ull_t cnt = 0;
    for(auto a:ins) {
        cnt += a[pos]=='1';
    }
    for (auto a:ins)
        if ( (cnt*2 < ins.size()) != (a[pos]=='0')) {
           mynew.push_back(a);
        }
    return get_co_rating(mynew, pos+1);
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

    std::vector<ull_t> cnts = {};
    std::vector<std::string> inputs = {};
    ull_t cnt = 0;
    while(!input.eof()) {
        std::string a;
        std::getline(input, a);
        inputs.push_back(a);
        if (a.size() > cnts.size()) {
            cnts.resize(a.size(),0);
        }
        for (u_t i=0;i<a.size();i++) {
            cnts[i] += a[i]=='1';
        }
        cnt++;
    }
    ull_t g=0,e=0;
    for (auto a: cnts) {
        g <<=1;
        e <<=1;
        g |= (2*a > cnt);
        e |= (2*a <= cnt);
    }
    std::cout<<(g*e)<<std::endl;
    input.close();

    // Remove last stuff
    inputs.pop_back();
    ull_t ox=0,co=0;
    for(auto a:get_ox_rating(inputs,0)) {
        ox <<= 1;
        ox |= a-'0';
    }
    for(auto a:get_co_rating(inputs,0)) {
        co <<= 1;
        co |= a-'0';
    }

    std::cout<<co<<' '<<ox<<' '<<(co*ox)<<std::endl;

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
