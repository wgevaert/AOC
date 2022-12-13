#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <unordered_map>
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

    char a[2]={'#','#'},first,last;
    first = a[0] = input.get();
    a[1] = input.get();
    u_t c=1;
    std::unordered_map<std::string, ull_t> pair_cnt[2] = {{},{}};
    std::unordered_map<std::string, char> mappings = {};

    do {
        std::string key = std::string(1, a[(c+1)%2]) + std::string(1, a[c]);
        if (verb_lvl>3)std::cout<<key<<' ';
        if (pair_cnt[0].find(key)==pair_cnt[0].end()) {
            pair_cnt[0][key]=1;
        } else {
            pair_cnt[0][key]+=1;
        }
        c=(c+1)%2;
        a[c] = input.get();
    } while (a[c]!='\n');
last = a[(c+1)%2];
input.get();

    while(true) {
        std::string key;
        std::getline(input,key,' ');
        if(input.eof())break;
        read_or_die("-> ",input);
        mappings[key]=input.get();
        read_or_die("\n",input);
    }
    input.close();

    const u_t steps=40;c=0;
    for (u_t i=0;i<steps;i++) {
        u_t tot=0;
        if(verb_lvl>3)std::cout<<"Step "<<i<<std::endl;
        for (auto a:pair_cnt[c]) {
            if(verb_lvl>9)std::cout<<"We have "<<a.second<<" of "<<a.first<<std::endl;
tot+=a.second;
            std::string newl=std::string(1,a.first[0])+std::string(1,mappings[a.first]),newr=std::string(1,mappings[a.first])+std::string(1,a.first[1]);
            if(verb_lvl>8)std::cout<<"Made "<<newl<<" and "<<newr<<" from "<<a.first<<std::endl;

            if (pair_cnt[(c+1)%2].find(newl)==pair_cnt[(c+1)%2].end()) {
                pair_cnt[(c+1)%2][newl]=a.second;
            } else {
                pair_cnt[(c+1)%2][newl]+=a.second;
            }
            if (pair_cnt[(c+1)%2].find(newr)==pair_cnt[(c+1)%2].end()) {
                pair_cnt[(c+1)%2][newr]=a.second;
            } else {
                pair_cnt[(c+1)%2][newr]+=a.second;
            }
        }
if(verb_lvl>9)std::cout<<tot<<" total"<<std::endl;
        pair_cnt[c].clear();
        c = (c+1)%2;
    }
    static ull_t cnt[26],min=-1,max=0,tot=0;
    for (auto a:pair_cnt[c]) {
        tot+=a.second;
        cnt[a.first[0]-'A']+=a.second;
        cnt[a.first[1]-'A']+=a.second;
    }
    cnt[first-'A']--;
    cnt[last-'A']--;
    for (u_t i=0;i<26;i++) {
        if(cnt[i]!=0&&cnt[i]<min)min=cnt[i];
        if(cnt[i]>max){std::cout<<static_cast<char>('A'+i)<<cnt[i]<<std::endl;max=cnt[i];}
    }

    std::cout<<max/2<<'-'<<min/2<<'='<<((max-min)/2+1)<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
