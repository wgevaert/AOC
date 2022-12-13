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

class thingy {
public:
    u_t values=0,popcnt=0;
    thingy(std::string s) {for(auto a:s)if('a'<=a&&a<='g'){popcnt++;values|=(1<<(a-'a'));}}
    thingy(const u_t val) {values=val;for(u_t i=1;i<(1<<7);i<<=1)popcnt+=i&val?1:0;}
    const bool operator< (const thingy& o) const {return this->popcnt < o.popcnt;}
};

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

    ull_t tot1=0,tot=0;
    do {
        // Maps thingie values to numbers they represent
        std::unordered_map<u_t,u_t> mymap={};
        // Maps numbers they represent to thingie values
        u_t invmap[10];
        std::vector<thingy> thingies;
        std::string thing;
        for (int i=0;i<10;i++) {
            std::getline(input,thing,' ');
            thingies.emplace_back(thing);
        }
        read_or_die("| ",input);

        // Determine stuff
        std::sort(thingies.begin(),thingies.end());
        mymap[thingies[0].values] = 1;
        mymap[thingies[1].values] = 7;
        mymap[thingies[2].values] = 4;
        mymap[thingies[9].values] = 8;
        invmap[1] = thingies[0].values;
        invmap[7] = thingies[1].values;
        invmap[4] = thingies[2].values;
        invmap[8] = thingies[9].values;
        for (u_t i=9-3;i<9;i++) {
            if ((thingies[i].values & invmap[1]) == invmap[1]) {
                if ((thingies[i].values & invmap[4]) == invmap[4]) {
                    mymap[thingies[i].values] = 9;
                    invmap[9] = thingies[i].values;
                } else {
                    mymap[thingies[i].values] = 0;
                    invmap[0] = thingies[i].values;
                }
            } else {
                mymap[thingies[i].values] = 6;
                invmap[6]=thingies[i].values;
            }
        }
        for (u_t i=3;i<=5;i++) {
            if ((thingies[i].values & invmap[6]) == thingies[i].values) {
                mymap[thingies[i].values] = 5;
                invmap[5]=thingies[i].values;
            } else if((thingies[i].values & invmap[9]) == thingies[i].values) {
                mymap[thingies[i].values] = 3;
                invmap[3]=thingies[i].values;
            } else {
                mymap[thingies[i].values] = 2;
                invmap[2]=thingies[i].values;
            }
        }

        std::vector<thingy> reqs;
        for(int i=0;i<3;i++) {
            std::getline(input,thing,' ');
            reqs.emplace_back(thing);
        }
        std::getline(input,thing);
        reqs.emplace_back(thing);

        u_t n=0;
        for(auto a:reqs){
            n=n*10+mymap[a.values];
            tot1+=(a.popcnt==2||a.popcnt==3||a.popcnt==4||a.popcnt==7);
        }
        tot+=n;
    } while(input.peek()!='\n'&&!input.eof());
    input.close();

    std::cout<<tot1<<' '<<tot<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
