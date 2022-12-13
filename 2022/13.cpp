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

class list {
public:
    bool is_list=false;
    std::vector<list> childs;
    u_t val=-1;
    list(u_t v):val(v){}
    list(){is_list=true;}
    list(list* l){
        is_list=true;
        childs={l->copy()};
    }
    list copy() {
        if (!is_list)
            return list(val);
        list l;
        for (auto& a:childs) {
            l.childs.push_back(a.copy());
        }
        return l;
    }
    void print() {
        if (is_list) {
            std::cout<<'[';
            for (auto a=0;a<childs.size();a++) {childs[a].print();if(a!=childs.size()-1)std::cout<<',';}
            std::cout<<']';
        } else {
            std::cout<<val;
        }
    }
    ll_t compare(list rhs) {
        if (is_list && !rhs.is_list) {
            return compare(list(&rhs));
        } else if (!is_list && rhs.is_list) {
            return list(this).compare(rhs);
        } else if (!is_list && !rhs.is_list) {
            if (val==rhs.val) return 0;
            if (val < rhs.val) return 1;
            return -1;
        }

        for (auto a=0;a<childs.size()&&a<rhs.childs.size();a++) {
            ll_t c = childs[a].compare(rhs.childs[a]);
            if (c!=0) {
                return c;
            }
        }
        if (childs.size()==rhs.childs.size())return 0;
        if (childs.size()<rhs.childs.size())return 1;
        return -1;
    }
};

list read_list(std::istream& in) {
    if (in.peek()=='[') {
        list r;
        read_or_die("[",in);
        char a = in.peek();
        while(a!=']') {
            r.childs.push_back(read_list(in));
            a=in.peek();
            if (a==',')in.get();
        }
        in.get();
        return r;
    }
    u_t v;
    in>>v;
    return list(v);
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

    std::vector<list> pairs[2], all;

    while(!input.eof()) {
        pairs[0].push_back(read_list(input));
        all.push_back(pairs[0].back());
        read_or_die("\n",input);
        pairs[1].push_back(read_list(input));
        all.push_back(pairs[1].back());
        read_or_die("\n",input);
        input.get();
    }
    input.close();

    u_t ans=0;
    for (u_t i=0;i<pairs[0].size();i++) {
        ans+=(1==pairs[0][i].compare(pairs[1][i]) ? i+1 : 0);
    }
    std::cout<<ans<<std::endl;

    list l;
    l.childs.emplace_back(2);
    all.push_back(list(&l));
    l.childs.clear();
    l.childs.emplace_back(6);
    all.push_back(list(&l));

    std::sort(all.begin(),all.end(),[](list& l, list &r) -> bool {return l.compare(r)==1;});

    u_t ans2 = 1;
    for (u_t i=0;i<all.size();i++) {
        if ( all[i].is_list
           &&all[i].childs.size()==1
           &&all[i].childs[0].is_list
           &&all[i].childs[0].childs.size()==1
           &&(all[i].childs[0].childs[0].val==2||all[i].childs[0].childs[0].val==6)
        ) {
            ans2 *= i+1;
        }
    }

    std::cout<<ans2<<std::endl;
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
