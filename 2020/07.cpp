#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <cstring>
#include <string>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;
const ull_t ull_max = static_cast<ull_t>(-1);

struct rule{
    std::vector<std::pair<u_t,u_t>> contains = {};
    std::vector<u_t> contained_in = {};
    bool is_parent=false;
    ull_t childs=ull_max;

    rule(){};
    rule(u_t a, u_t b): contains{{a,b}}{};
    rule(u_t a): contained_in{a}{};
};


u_t get_index(const std::string color) {
    static std::unordered_map<std::string, u_t> dict;

    auto pos = dict.find(color);
    if (pos == dict.end()) {
       u_t new_index = dict.size();
       dict[color] = new_index;
       return new_index;
    }
    return pos->second;
}

u_t find_parents(const u_t base, std::vector<rule>& rules) {
    static u_t ans=0;

    for (auto a:rules[base].contained_in){
        if (!rules[a].is_parent) {
            ans++;
            rules[a].is_parent = true;
            find_parents(a, rules);
        }
    }

    return ans;
}

ull_t find_childs(const u_t base, std::vector<rule>& rules) {
    if (rules[base].childs != ull_max)
        return rules[base].childs;
    rules[base].childs = 0;
    for (auto a : rules[base].contains) {
        if (verb_lvl > 3)
            std::cout<<"Recursing into "<<a.first<<" of which we have "<<a.second<<std::endl;

        rules[base].childs += a.second * (find_childs(a.first, rules) + 1);

        if (verb_lvl > 3)
            std::cout<<base<<" now has "<<rules[base].childs<<" childs."<<std::endl;
    }
    return rules[base].childs;
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

    std::vector<rule> rules = {};
    std::string line, color_1, color_2;

    while (!input.eof()) {
        // I don't like input handling...
        std::getline(input, line);
        if (verb_lvl > 3)
            std::cout<<"Read "<<line<<std::endl;

        if (line.size()) {
            u_t split_1 = line.find(" bags contain ");
            if (verb_lvl > 5)
                std::cout<<"Split found at "<<split_1<<std::endl;

            color_1 = line.substr(0, split_1);

            if (verb_lvl > 2)
                std::cout<<"Color is '"<<color_1<<"'"<<std::endl;

            line = line.substr(split_1 + strlen(" bags contain "));

            if (line[0]!='n'||line[1]!='o') {
                while ((split_1 = line.find(" bag")) + 1 != 0) {
                    u_t cnt = 0;
                    while (line[cnt]<='9'&&line[cnt]>='0')
                        cnt++;
                    color_2 = line.substr(cnt+1, split_1-(cnt+1));

                    if(verb_lvl > 5)
                        std::cout<<'"'<<line.substr(0, cnt)<<'"'<<" of \""<<color_2<<'"'<<std::endl;

                    cnt = std::stoul(line.substr(0,cnt));

                    u_t index_1=get_index(color_1), index_2=get_index(color_2);
                    if (index_1 == rules.size()) {
                        rules.emplace_back(index_2, cnt);
                    } else {
                        rules[index_1].contains.emplace_back(index_2, cnt);
                    }
                    if (index_2 == rules.size()) {
                        rules.emplace_back(index_1);
                    } else {
                        rules[index_2].contained_in.emplace_back(index_1);
                    }
                    split_1 = line.find(", ");
                    if (split_1 + 1 == 0) {
                        break;
                    } else {
                        line = line.substr(split_1 + strlen(", "));
                    }
                    if (verb_lvl > 5)
                        std::cout<<"Read "<<line<<std::endl;
                }
            }
        }
    }
    input.close();

    if (verb_lvl > 1) {
        std::cout<<"Found the following rules:\n";
        for (int i=0;i<rules.size();i++) {
            std::cout<<i<<" contains ";
            for (auto b:rules[i].contains)
                std::cout<<b.second<<" of "<<b.first<<", ";
            std::cout<<"and is contained in ";
            for (auto b:rules[i].contained_in)
                std::cout<<b<<", ";
            std::cout<<std::endl;
        }
    }

    std::cout <<
        find_parents(get_index("shiny gold"), rules) << ' ' <<
        find_childs(get_index("shiny gold"), rules)<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
