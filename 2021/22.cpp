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

const u_t dim=3;

class cuboid {
public:
    ll_t min[dim],max[dim];
    void print() const {
        for (u_t i=0;i<dim;i++)std::cout<<static_cast<char>('z'-dim+i+1)<<'='<<min[i]<<".."<<max[i]<<(i==dim-1?"":",");
    }
    bool has_overlap(const cuboid& rhs) {
        if(verb_lvl>10){std::cout<<"Testing overlap of ";print();std::cout<<" and ";rhs.print();std::cout<<std::endl;}
        for (u_t i=0;i<dim;i++) {
            if ((min[i]>rhs.max[i])||(max[i]<rhs.min[i])){
                if(verb_lvl>11)std::cout<<"false"<<std::endl;
                return false;
            }
        }
        if(verb_lvl>11)std::cout<<"true"<<std::endl;
        return true;
    }
    ull_t volume() {
        ull_t r=1;
        for (short i=0;i<dim;i++) {
            if (max[i]<min[i])return 0;
            r *= static_cast<ull_t>(max[i]-min[i]+1);
        }
        return r;
    }
    ull_t intersected_volume(cuboid& c) {
        cuboid C;
        for (u_t i=0;i<dim;i++) {
            if (c.max[i]<max[i])C.max[i]=c.max[i];else C.max[i]=max[i];
            if (c.min[i]>min[i])C.min[i]=c.min[i];else C.min[i]=min[i];
        }
        return C.volume();
    }
};

class reactor {
public:
    std::vector<cuboid> ons = {};
    void intersect(cuboid& o) {
        std::vector<cuboid> new_ons;
        for (auto& c:ons) {
            if (c.has_overlap(o)) {
                for (u_t i=0;i<dim;i++) {
                    //c='11..13' o='9..11' => newc='12..13' c='11..11' o='9..11'
                    //c='10..11' o='9..11' => c='10..11' o='9..11' (just remove c)
                    if (o.max[i]>=c.min[i] && o.max[i]<c.max[i]) {
                        cuboid newc;
                        for (u_t j=0;j<dim;j++){newc.min[j]=c.min[j];newc.max[j]=c.max[j];}
                        newc.min[i]=o.max[i]+1;
                        if(verb_lvl>6){std::cout<<"Splitting ";c.print();std::cout<<" into ";newc.print();std::cout<<" and ";}
                        c.max[i]=o.max[i];
                        if(verb_lvl>6){c.print();std::cout<<std::endl;}
                        new_ons.push_back(newc);
                    }
                    else if (verb_lvl>11)std::cout<<"No max split"<<std::endl;

                    //c='9..11' o='11..13' => newc='9..10' c='11..11' o='11..13'
                    if (o.min[i]>c.min[i]&&o.min[i]<=c.max[i]) {
                        cuboid newc;
                        for (u_t j=0;j<dim;j++){newc.min[j]=c.min[j];newc.max[j]=c.max[j];}
                        newc.max[i]=o.min[i]-1;
                        if(verb_lvl>6){std::cout<<"Splitting ";c.print();std::cout<<" into ";newc.print();std::cout<<" and ";}
                        c.min[i]=o.min[i];
                        if(verb_lvl>6){c.print();std::cout<<std::endl;}
                        new_ons.push_back(newc);
                    }
                    else if(verb_lvl>11)std::cout<<"No min split"<<std::endl;
                }
            } else {
                new_ons.push_back(c);
            }
        }
        ons.clear();
        ons = new_ons;
    }
    void turn_on(cuboid& o) {
        intersect(o);
        ons.push_back(o);
    }
    void turn_off(cuboid& o) {
        intersect(o);
    }
    void print() {
        std::cout<<"Reactor now contains:\n";for(auto a:ons){a.print();std::cout<<std::endl;}std::cout<<std::endl;
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

    reactor r;
    cuboid c;

    while(input.get()=='o') {
        bool on;
        if (input.get()=='n') {
            on=true;
        } else if (input.get()=='f') {
            on=false;
        }
        read_or_die(" ",input);
        for (u_t i=0;i<dim;i++) {
            input.get();//'x', 'y', 'z', etc.
            read_or_die("=",input);
            input>>c.min[i];
            read_or_die("..",input);
            input>>c.max[i];
            input.get();//',' or '\n';
        }
        if (on) {
            r.turn_on(c);
        } else {
            r.turn_off(c);
        }
        if(verb_lvl>12)r.print();
    }
    input.close();

    ull_t ans2 = 0, ans1=0;
    cuboid small;
    for (u_t i=0;i<dim;i++){small.min[i]=-50;small.max[i]=50;}
    for (auto& a:r.ons){
        ans2+=a.volume();
        ans1+=a.intersected_volume(small);
    }

    std::cout<<ans1<<'\n'<<ans2<<std::endl;
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
