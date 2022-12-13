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

ull_t get_score(char c) {
    switch (c) {
        case ')':return 3;
        case ']':return 57;
        case '}':return 1197;
        case '>':return 25137;
    }
    return 0;
}

ull_t get_score2(char c) {
    switch (c) {
        case '(':return 1;
        case '[':return 2;
        case '{':return 3;
        case '<':return 4;
    }
    return 0;
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

    std::vector<std::string> lines;
    std::string line;
    std::getline(input,line);
    do {
        lines.push_back(line);

        std::getline(input,line);
    } while(line != "");
    input.close();

    ull_t s=0;
    std::vector<ull_t> s2 = {};
    for (auto l:lines) {
        std::vector<char> stack = {};
        bool corr=false;
        for (auto c:l) {
            if (c=='{'||c=='['||c=='('||c=='<') {
                stack.emplace_back(c);
            } else {
                char o;
                if (!stack.size()) {
                    o='m';
                } else {
                    o=stack[stack.size()-1];
                    stack.pop_back();
                }
                if (!(
                  (o=='('&&c==')')||
                  (o=='['&&c==']')||
                  (o=='{'&&c=='}')||
                  (o=='<'&&c=='>')
                )) {
                    corr=true;
                    s+=get_score(c);
                    break;
                }
            }
        }
        if (!corr) {
            if (verb_lvl)std::cout<<"Line "<<l<<" is not corrupted"<<std::endl;
            ull_t sc=0;
            for (auto a=stack.rbegin();a!=stack.rend();a++) {
                if(verb_lvl>2)std::cout<<get_score2(*a)<<" for "<<*a<<std::endl;
                sc = sc * 5 + get_score2(*a);
            }
            if(verb_lvl>1)std::cout<<"Got score "<<sc<<std::endl;
            s2.emplace_back(sc);
        }
    }
    std::cout<<s<<std::endl;

    std::sort(s2.begin(),s2.end());
    std::cout<<s2[s2.size()/2]<<std::endl;
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
