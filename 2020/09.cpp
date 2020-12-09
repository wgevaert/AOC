#include <iostream>
#include <fstream>
#include <chrono>
#include <unordered_set>
#include <vector>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

const u_t pre_size = 25;

struct cookie {
    ull_t b, e, v;
    cookie(ull_t B, ull_t E, ull_t V): b(B), e(E), v(V) {};
};

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

    ull_t in, ans;

    std::unordered_set<ull_t> u_received = {};
    std::vector<ull_t> v_received = {};

    for (int i=0;i<pre_size;i++) {
        input>>in;
        u_received.emplace(in);
        v_received.emplace_back(in);
        if (input.get()!='\n'){
            std::cerr<<"Newline expected..."<<std::endl;
            exit(1);
        }
    }

    while(input.peek()!=-1) {
        input>>in;
        bool found = false;
        for (int i=v_received.size()-1;i>=0;i--) {
            if (u_received.find(in-v_received[i]) != u_received.end()) {
                found = true;
                break;
            }
        }
        if (!found) {
            std::cout<<"Found ans "<<in<<std::endl;
            ans = in;
        }
        v_received.emplace_back(in);
        u_received.emplace(in);
        if (input.get()!='\n'){
            std::cerr<<"Newline expected..."<<std::endl;
            exit(1);
        }
    }

    input.close();

    cookie yum(0,2,v_received[0]+v_received[1]+v_received[2]);

    while (yum.v != ans) {
        if (yum.v < ans) {
            yum.v += v_received[++yum.e];
        } else {
            if (yum.e - yum.b <= 2) {
                yum.v += v_received[++yum.e] - v_received[yum.b++];
            } else {
                yum.v -= v_received[yum.b++];
            }
        }
        if (verb_lvl > 1) {
            std::cout<<"yum from "<<yum.b<<" to "<<yum.e<<" has val "<<yum.v<<std::endl;
        }
        if (yum.e >= v_received.size()) {
            std::cerr<<"No second ans found"<<std::endl;
            exit(1);
        }
    }

    std::cout<<"Found ans: Between "<<yum.b<<" and "<<yum.e;

    u_t min=-1, max=0;
    for (int j=yum.b;j<=yum.e;j++) {
        if (v_received[j] < min)
            min=v_received[j];
        if (v_received[j] > max)
            max=v_received[j];
    }

    std::cout<<" with value "<<(min+max)<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
