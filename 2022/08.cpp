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

class tree {
public:
    char height;
    bool visible = false;
    ull_t score=1;
    tree(char h){height=h;}
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

    std::vector<std::vector<tree>> forest;
    std::string line;
    std::vector<tree> newline;

    std::getline(input, line);
    while(line.size()) {
        newline.clear();
        for (auto a:line)newline.emplace_back(a);
        forest.push_back(newline);
        std::getline(input, line);
    }
    std::cout<<"DONE READING"<<std::endl;
    input.close();

    if(verb_lvl) {
        for (u_t i=0;i<forest[0].size();i++){std::cout<<(i>10?static_cast<char>(i/10+'0'):' ');}std::cout<<std::endl;
        for (u_t i=0;i<forest[0].size();i++)std::cout<<static_cast<char>(i%10+'0');std::cout<<'\n'<<std::endl;
        for(u_t a=0;a<forest.size();a++){for(auto b:forest[a])std::cout<<b.height;std::cout<<' '<<a<<std::endl;}
    }

    for (u_t i=0;i<forest.size();i++) {
        char curh='0'-1;
        u_t curp=0;
        do {
            if (forest[i][curp].height > curh) {
                forest[i][curp].visible=true;
                curh = forest[i][curp].height;
            }
            curp++;
        } while (curh!='9'&&curp<forest[i].size());
    }
    for (u_t i=0;i<forest.size();i++) {
        char curh='0'-1;
        u_t curp=forest[i].size()-1;
        do {
            if (forest[i][curp].height > curh) {
                forest[i][curp].visible=true;
                curh = forest[i][curp].height;
            }
            curp--;
        } while (curh!='9'&&curp+1!=0);
    }
    // These bounds don't make sense for non-square forests...
    for (u_t i=forest.size()-1;i+1!=0;i--) {
        char curh='0'-1;
        u_t curp=0;
        do {
            if (forest[curp][i].height > curh) {
                forest[curp][i].visible=true;
                curh = forest[curp][i].height;
            }
            curp++;
        } while (curh!='9'&&curp<forest[i].size());
    }
    for (u_t i=forest.size()-1;i+1!=0;i--) {
        char curh='0'-1;
        u_t curp=forest[i].size()-1;
        do {
            if (forest[curp][i].height > curh) {
                forest[curp][i].visible=true;
                curh = forest[curp][i].height;
            }
            curp--;
        } while (curh!='9'&&curp+1!=0);
    }

    ull_t ans=0;
    for (auto a:forest){for (auto b:a){
        if (b.visible) {
            if(verb_lvl>5)std::cout<<b.height;
            ans+=b.visible;
        } else if (verb_lvl > 5){
            std::cout<<static_cast<char>(b.height-'0'+'a');
        }
    }if(verb_lvl > 5)std::cout<<std::endl;}
    std::cout<<ans<<std::endl;

    u_t ans2=0;

    for(u_t i=1;i<forest.size()-1;i++)for (u_t j=1;j<forest[i].size()-1;j++) {
        char h=forest[i][j].height;
        std::cout<<"HEIGHT "<<h<<std::endl;
        u_t s=1;
        for (u_t k=i+1;k+1<forest.size()&&forest[k][j].height<h;k++)s++;
        forest[i][j].score *= s;
        s=1;
        for (u_t k=i-1;k!=0&&forest[k][j].height<h;k--)s++;
        forest[i][j].score *= s;
        s=1;
        for (u_t k=j+1;k+1<forest[i].size()&&forest[i][k].height<h;k++)s++;
        forest[i][j].score *= s;
        s=1;
        for (u_t k=j-1;k!=0&&forest[i][k].height<h;k--)s++;
        forest[i][j].score *= s;

        std::cout<<"CALCULATED SCORE "<<forest[i][j].score<<" AT "<<i<<','<<j<<std::endl;
        if (ans2<forest[i][j].score)ans2=forest[i][j].score;
    }
    std::cout<<ans2<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
