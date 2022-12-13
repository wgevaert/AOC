#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;
bool p2 = false; // Change for part 2

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

class cave {
    std::vector<size_t> n;//neighbours
public:
    bool is_big;
    u_t visited=0;

    cave(bool b){is_big=b;}
    void add_n(size_t nr){n.emplace_back(nr);}
    std::vector<size_t>& get_n(){return n;}
};

class searcher {
public:
    size_t pos;
    bool cheated=false;
    searcher(size_t p,bool c){pos=p;cheated=c;}
};

ull_t do_search(searcher s, std::vector<cave>& cs, const size_t end, const size_t start) {
    static u_t d=0;
    d++;
    if (s.pos==end) {d--;cs[s.pos].visited=false;return 1;}
    ull_t res = 0;

    if(verb_lvl>10){std::cout<<"At (depth "<<d<<") "<<s.pos<<":";for (auto c:cs[s.pos].get_n())if (cs[c].is_big||!cs[c].visited)std::cout<<c<<',';std::cout<<s.cheated<<std::endl;}

    for (auto c:cs[s.pos].get_n()) {
        if (cs[c].is_big || !cs[c].visited) {
            // Search it!
            cs[c].visited++;
            res += do_search(searcher(c,s.cheated), cs, end, start);
        } else if (p2 && !cs[c].is_big && !s.cheated && c!=start && c!=end) {
            // Let's cheat!
            cs[c].visited++;
            res += do_search(searcher(c,true), cs, end, start);
        }
    }
    cs[s.pos].visited--;
    d--;
    return res;
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

    u_t start,end;
    std::string line;
    std::getline(input,line,'-');
    std::unordered_map<std::string,u_t>nodes={};
    const size_t maxn=20;
    std::vector<cave>caves = {};
    static bool adjmat[maxn][maxn];
    while(line != "") {
        u_t n1,n2;
        auto n=nodes.find(line);
        if (n==nodes.end()) {
            n1 = caves.size();
            nodes[line]=caves.size();
            caves.emplace_back(line[0]>='A'&&line[0]<='Z');
        } else {
            n1 = n->second;
        }
        std::getline(input,line);
        n=nodes.find(line);
        if (n==nodes.end()) {
            n2 = caves.size();
            nodes[line]=caves.size();
            caves.emplace_back(line[0]>='A'&&line[0]<='Z');
        } else {
            n2 = n->second;
        }
        if (n1>=maxn||n2>=maxn) {
            std::cerr<<maxn<<" not big enough"<<std::endl;exit(1);
        }
        caves[n1].add_n(n2);
        caves[n2].add_n(n1);
        adjmat[n1][n2]=adjmat[n2][n1]=true;
        std::getline(input,line,'-');
    }
    input.close();

    start = nodes["start"];
    end = nodes["end"];

    searcher s(start,false);
    caves[start].visited=1;
    std::cout<<do_search(s, caves, end, start)<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
