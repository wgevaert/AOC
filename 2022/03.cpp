#include <iostream>
#include <fstream>
#include <chrono>
#include <string>/*
#include <vector>
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

ull_t get_prio(char a) {
    if ('a'<=a&&a<='z') {
        return a-'a'+1;
    }
    return a-'A'+27;
}

char get_char(u_t prio) {
    if (prio <=26)return 'a'+prio-1;
    return 'A'+prio-27;
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

    u_t cnt[52], line_cnt=0, cnt2[52];
    for (auto& a:cnt2)a=0;
    bool seen[52];
    ull_t tot=0,tot2=0;
    std::string in;
    std::getline(input,in);
    while(!input.eof()&&in !="") {
        for(u_t i=0;i<52;i++){cnt[i]=0;seen[i]=false;}
        for (u_t i=0;i<in.size()/2;i++) {
            cnt[get_prio(in[i])-1]++;
            if(verb_lvl>3)std::cout<<"In first compartment: "<<in[i]<<' '<<get_prio(in[i])<<std::endl;
        }
        for (auto a:in){
            u_t p=get_prio(a)-1;
            if(!seen[p]){
                seen[p]=true;
                cnt2[p]++;
                if(verb_lvl>3)std::cout<<"Have seen "<<a<<" in "<<in<<" for a total of "<<cnt2[p]<<std::endl;
            }
        }
        for (u_t i=in.size()/2;i<in.size();i++) {
            if (cnt[get_prio(in[i])-1]) {
                tot+=get_prio(in[i]);
                if (verb_lvl>1)std::cout<<"Found "<<in[i]<<" with prio "<<get_prio(in[i])<<" in "<<in<<std::endl;
                break;
            }
        }
        line_cnt++;
        if (!(line_cnt%3)) {
            if (verb_lvl>3)std::cout<<"Resetting etc."<<std::endl;
            for (u_t i=0;i<52;i++){
                if(cnt2[i]==3){
                    tot2+=i+1;
                    if (verb_lvl>4)std::cout<<"Found badge "<<get_char(i+1)<<std::endl;
                } else if(verb_lvl>5)std::cout<<" No badge at "<<get_char(i+1)<<": "<<cnt2[i]<<std::endl;
                cnt2[i]=0;
            }
        }
        std::getline(input,in);
    }
    std::cout<<tot<<' '<<tot2<<std::endl;
    

    // do things with input.

    input.close();

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
