#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

class board {
    u_t content[5][5];
    bool called[5][5];
    bool won=false;
    std::unordered_map<u_t,std::pair<u_t,u_t>> numbers;
public:
    board(std::istream& in) {
        u_t i;
        for(int a=0;a<5;a++)for(int b=0;b<5;b++){
          in>>i;
          content[a][b]=i;
          called[a][b]=false;
          numbers[i]=std::pair<u_t,u_t>(a,b);
        }in.get();in.get();
    }
    
    bool roll(u_t n) {
        auto pos = numbers.find(n);
        if (pos == numbers.end()) {
            return false;
        }
        called[pos->second.first][pos->second.second]=true;
        bool has_bingo = true;
        for (int i=0;i<5;i++)if(!called[pos->second.first][i]){has_bingo=false;break;}
        if (has_bingo&&!won){won=true;return true;}
        has_bingo = true;
        for (int i=0;i<5;i++)if(!called[i][pos->second.second]){has_bingo=false;break;}
        if(has_bingo&&!won){won=true;return true;}
        return false;
    }
    u_t sum() {
        u_t rtr=0;
        for(int a=0;a<5;a++)for(int b=0;b<5;b++)if(!called[a][b])rtr+=content[a][b];return rtr;
    }

    void print() {
        for (u_t a=0;a<5;a++){for(u_t b=0;b<5;b++){std::cout<<content[a][b]<<(called[a][b]?'@':'_')<<' ';}std::cout<<std::endl;}
    }
};

void print(std::vector<u_t> v) {
    for(auto a:v)std::cout<<a<<',';std::cout<<std::endl;
}

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

    u_t in;
    std::vector<u_t> rolls = {};
    do {
        input>>in;
        rolls.push_back(in);
    } while(input.get()!='\n');
    if(verb_lvl>2)print(rolls);

    std::vector<board> boards = {};
    while(!input.eof()) {
        boards.emplace_back(input);
    }
    input.close();

    u_t round = 0;
    for (auto& a:rolls) {
        round++;
        u_t board=0;
        if (verb_lvl > 2)std::cout<<"Round "<<round<<" (picked number "<<a<<"):\n";
        for (auto& b:boards) {
            board++;
            if (b.roll(a)) {
                std::cout<<"Board "<<board<<" won with score "<<(a*b.sum())<<" at round "<<round<<std::endl;
            }
            if (verb_lvl > 3){std::cout<<"Board "<<board<<":\n";b.print();std::cout<<std::endl;}
        }
    }
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
