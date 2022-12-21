#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <unordered_map>

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

#define NO_VALUE 9223372036854775807

class monkey {
public:
    ll_t value = NO_VALUE;
    bool can_be_simplified = true;
    std::string name, m1="", m2="";
    char op = '?';
    monkey(std::string n, std::string M1, char o, std::string M2): name(n), m1(M1), m2(M2), op(o) {}
    monkey(std::string n, ll_t v):name(n), value(v) {}
    monkey() {}
    void print() {
        if(value==NO_VALUE) {
            std::cout<<name<<": "<<m1<<' '<<op<<' '<<m2<<std::endl;
        } else {
            std::cout<<name<<": "<<value<<std::endl;
        }
    }
};

ll_t get_monkeyval(std::string M,std::unordered_map<std::string, monkey>& ms) {
    if(verb_lvl) {
        std::cout<<"Calcing for '"<<M<<"'\n"<<std::flush;
    }
    auto A=ms.find(M);
    if (A==ms.end()) {
        std::cerr<<"Could not find "<<M<<" in monkeys."<<std::endl;exit(1);
    }
    monkey m = A->second;
    if (m.op == '?') {
        return m.value;
    }
    ll_t v1=get_monkeyval(m.m1, ms), v2=get_monkeyval(m.m2, ms);
    switch (m.op) {
        case '+':return v1+v2;
        case '-':return v1-v2;
        case '*':return v1*v2;
        case '/':return v1/v2;
    }
    std::cerr<<"Unknown operation: "<<m.op<<std::endl;
    exit(1);
}

ll_t get_monkeyval2(std::unordered_map<std::string, monkey>& ms) {
    monkey m = ms["root"];
    std::string left = m.m1, right = m.m2;
    if (ms[left].can_be_simplified) {
        left = m.m2;
        right = m.m1;
    }
    ll_t ans = get_monkeyval(right, ms);
    while (left != "humn") {
        m = ms[left];
        ll_t otherval;
        if (ms[m.m1].can_be_simplified) {
            left = m.m2;
            otherval = get_monkeyval(m.m1,ms);
            switch(m.op) {
                case '+':ans-=otherval;break;
                case '-':ans = otherval - ans;break;
                case '*':ans /= otherval;break;
                case '/':ans = otherval/ans;break;
                default:std::cerr<<"Unexpected: "<<m.op<<std::endl;exit(1);
            }
        } else {
            left = m.m1;
            otherval = get_monkeyval(m.m2,ms);
            switch(m.op) {
                case '+':ans -= otherval;break;
                case '-':ans += otherval;break;
                case '*':ans /= otherval;break;
                case '/':ans *= otherval;break;
                default:std::cerr<<"Unexpected: "<<m.op<<std::endl;exit(1);
            }
        }
    }
    return ans;
}

// Propagate can_be_simp from humn to root
bool can_be_simp(std::string M, std::unordered_map<std::string, monkey>& ms) {
    auto A=ms.find(M);
    if (A==ms.end()) {
        std::cerr<<"Could not find "<<M<<" in monkeys."<<std::endl;exit(1);
    }
    monkey m = A->second;
    if (M=="humn"){ms[M].can_be_simplified=false;return false;}
    if (m.op == '?') return true;
    if (can_be_simp(m.m1,ms) && can_be_simp(m.m2, ms)) return true;
    ms[M].can_be_simplified = false;
    return false;
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

    std::unordered_map<std::string, monkey> monkeys = {};
    std::string name, m1, m2;
    while(std::isalpha(input.peek())) {
        std::getline(input, name, ':');
        read_or_die(" ",input);
        if (std::isdigit(input.peek())) {
            ll_t v;
            input>>v;
            input.get();
            monkeys[name] = monkey(name, v);
        } else {
            std::getline(input, m1, ' ');
            char o=input.get();
            input.get();
            std::getline(input, m2);
            monkeys[name] = monkey(name, m1, o, m2);
        }
    }
    input.close();
    if(verb_lvl)for(auto a:monkeys){std::cout<<a.first<<' ';a.second.print();}

    std::cout<<get_monkeyval("root", monkeys)<<std::endl;

    can_be_simp("root", monkeys);
    std::cout<<get_monkeyval2(monkeys)<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
