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
bool part2 = true;

ull_t modulus = 2*3*5*7*11*13*17*19;

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

class monkey {
    std::vector<ull_t> items[2];
    short curi=0;
    ull_t m=1,a=0;
    bool s=false;//to square or not to square
public:
    ull_t i_cnt=0;
    const ull_t d,iftrue,iffalse;
    std::vector<ull_t>& get_cur_items(){return items[curi];}
    void add_item(ull_t i){
        items[(curi+0/*1*/)%2].emplace_back(i);
    }
    void clear(){items[curi].clear();}
    void set_items(std::vector<ull_t>& i){for (auto a:i)items[curi].emplace_back(a);}
    monkey(ull_t M, ull_t A, bool S, ull_t D, ull_t ifTrue, ull_t ifFalse) : m(M),a(A),s(S),d(D),iftrue(ifTrue),iffalse(ifFalse){}
    bool test(ull_t in) {return !(in%d);}
    void fin(){}//items[curi].clear();curi=(curi+1)%2;}
    ull_t operation(ull_t i) {
        if (s) {
            i = i*i;
        }
        i *= m;
        i += a;
        i_cnt++;
        if (part2) {
            i %= modulus;
            return i;
        }
        return i/3;
    }
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

    const u_t rounds = part2 ? 10000 : 20;
    std::vector<monkey> monkeys = {};

    for (u_t unused=0;unused<=7;unused++) {
        read_or_die("Monkey ",input);
        ull_t i;
        input >> i;
        if (i!=unused) {
            std::cerr<<"Wrong! "<<i<<"!="<<unused<<std::endl;
            return 1;
        }
        read_or_die(":\n  Starting items: ",input);
        std::vector<ull_t> items = {};
        while(true) {
            input>>i;
            items.emplace_back(i);
            if (input.get()!=',')break;
            input.get();//space
        }
        read_or_die("  Operation: new = old ",input);
        char o=input.get();
        input.get();//space
        ull_t m=1,a=0, d, itr, ifa;
        if (input.peek() == 'o') {
            if (o!='*'){std::cerr<<"Weird..."<<std::endl;return 1;}
            read_or_die("old",input);
            o='s';
        } else {
            if (o=='*') {
                input>>m;
            } else {
                input>>a;
            }
        }
        read_or_die("\n  Test: divisible by ",input);
        input>>d;
        read_or_die("\n    If true: throw to monkey ",input);
        input>>itr;
        read_or_die("\n    If false: throw to monkey ",input);
        input>>ifa;
        read_or_die("\n",input);
        monkeys.emplace_back(m,a,o=='s',d,itr,ifa);
        monkeys.back().set_items(items);
        if (input.peek()=='\n')input.get();else break;
    }
    input.close();

    for (auto a=0;a<rounds;a++) {
        for (auto& m:monkeys) {
            for (auto i:m.get_cur_items()) {
                if(verb_lvl>2)std::cout<<"Inspecting "<<i<<" gives ";
                i = m.operation(i);
                if(verb_lvl>2)std::cout<<i<<std::endl;
                if (m.test(i)) {
                    if (verb_lvl > 1)std::cout<<"Success! Giving to "<<m.iftrue<<std::endl;
                    monkeys[m.iftrue].add_item(i);
                } else {
                    if (verb_lvl > 1)std::cout<<"Failure! Giving to "<<m.iffalse<<std::endl;
                    monkeys[m.iffalse].add_item(i);
                }
            }
            m.clear();
        }
        for (auto& m:monkeys)m.fin();
        if (verb_lvl > 1)for (auto m=0;m<monkeys.size();m++){std::cout<<"Monkey "<<m<<" has ";for (auto a:monkeys[m].get_cur_items())std::cout<<a<<' ';std::cout<<'|'<<std::endl;}
    }

    ull_t max=0,max1=0;
    for (auto m:monkeys) {
        std::cout<<m.i_cnt<<' ';
        if (m.i_cnt>max){max1=max;max=m.i_cnt;}else if(m.i_cnt>max1)max1=m.i_cnt;
    }std::cout<<std::endl;
    std::cout<<max<<' '<<max1<<' '<<(max*max1)<<std::endl;

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
