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

class op {
public:
    char o;
    long val=0;
    op (char O,ll_t v): o(O),val(v) {}
    op (char O): o(O) {}
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

    std::vector<op> ops = {};
    char a=input.peek();
    while (!input.eof()&&(a=='a'||a=='n')) {
        if (a=='a') {
            read_or_die("addx ",input);
            ll_t v;input>>v;
            ops.emplace_back(a,v);
        } else if (a=='n') {
            read_or_die("noop",input);
            ops.emplace_back(a);
        }
        read_or_die("\n",input);
        a=input.peek();
    }
    input.close();

    ll_t X=1, ans=0, rnd=1;
    std::vector<char> ans2={};

    for (auto a:ops) {
        // Round routine (duplicate, edit both positions!)
        if (!((rnd+20)%40)) {
            if(verb_lvl)std::cout<<X<<" at "<<rnd<<std::endl;
            ans += rnd*X;
        }
        ll_t diff = (rnd-1)%40 - X;
        ans2.emplace_back(diff<=1&&diff>=-1 ? '#' : ' ');

        if (a.o == 'a') {
            rnd ++;

            // Round routine (duplicate, edit both positions!)
            if (!((rnd+20)%40)) {
                if(verb_lvl)std::cout<<X<<" at "<<rnd<<std::endl;
                ans += rnd*X;
            }
            ll_t diff = (rnd-1)%40 - X;
            ans2.emplace_back(diff<=1&&diff>=-1 ? '#' : ' ');

            X += a.val;
        }
        rnd++;
    }

    for (u_t a=0;a<ans2.size();a++){std::cout<<ans2[a];if (!((a+1)%40))std::cout<<std::endl;}

    std::cout<<ans<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
