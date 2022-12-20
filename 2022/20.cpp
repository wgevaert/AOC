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

const bool part2=false;

class dll_ele {
public:
    ll_t val;
    dll_ele *front, *back;
    dll_ele(ll_t v):val(v){front=back=this;}
    void add_to_front(ll_t v) {
        dll_ele* old_front=front;
        old_front->back = front = new dll_ele(v);
        front->back = this;
        front->front = old_front;
    }
    void move_back() {
        back->front = front;
        front->back = back;
        back = back->back;
        front = back->front;
        back->front = this;
        front->back = this;
    }
    void move_front() {
        back->front = front;
        front->back = back;
        front = front->front;
        back = front->back;
        back->front = this;
        front->back = this;
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

    const ll_t p2const = 811589153;
    ll_t v, pos0;
    input>>v;
    if (part2)v*=p2const;
    dll_ele* mylist= new dll_ele(v);
    std::vector<dll_ele*> ref={mylist};
    read_or_die("\n",input);
    while (input.peek()=='-'||std::isdigit(input.peek())) {
        input>>v;
        if (part2)v*=p2const;
        if (v==0)pos0=ref.size();
        mylist->add_to_front(v);
        mylist = mylist->front;
        ref.emplace_back(mylist);
        read_or_die("\n",input);
    }
    input.close();

    if(verb_lvl>1){auto a=mylist;do{std::cout<<a->val<<' ';a=a->front;}while(a!=mylist);std::cout<<std::endl;}
    for (int i=0;i<1+9*part2;i++)
    for (auto a:ref) {
        ll_t A=a->val;
        if (A<=0) {
            // There's a -1 because the element we are moving is not counted.
            A = -1*((-1*A)%static_cast<ll_t>(ref.size()-1));
            for (auto b=0;b<-1*A;b++)a->move_back();
        } else {
            // There's a -1 because the element we are moving is not counted.
            A = A%(ref.size()-1);
            for (auto b=0;b<A;b++)a->move_front();
        }
        if(verb_lvl>1)std::cout<<"Moved "<<a->val<<" between "<<a->back->val<<" and "<<a->front->val<<std::endl;
    }

    ll_t ans=0;
    mylist=ref[pos0];
    for(auto b=0;b<1000;b++)mylist=mylist->front;
    if(verb_lvl)std::cout<<"1000 has "<<mylist->val<<std::endl;
    ans +=mylist->val;
    for(auto b=0;b<1000;b++)mylist=mylist->front;
    if(verb_lvl)std::cout<<"2000 has "<<mylist->val<<std::endl;
    ans +=mylist->val;
    for(auto b=0;b<1000;b++)mylist=mylist->front;
    if(verb_lvl)std::cout<<"3000 has "<<mylist->val<<std::endl;
    ans +=mylist->val;

    std::cout<<ans<<std::endl;
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
