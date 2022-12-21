#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

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
    dll_ele(){front=back=this;}
    void add_to_front(dll_ele* e) {
        e->front = front;
        front->back = e;
        front = e;
        e->back = this;
    }
    void loosen() {
        back->front = front;
        front->back = back;
    }
    void fasten() {
        front->back = this;
        back->front = this;
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
    ll_t v, pos0, size=1;
    input>>v;
    if (part2)v*=p2const;
    dll_ele ref[5000];
    ref[0].val = v;

    dll_ele* mylist= &ref[0];
    read_or_die("\n",input);
    while (input.peek()=='-'||std::isdigit(input.peek())) {
        input>>v;
        if (part2)v*=p2const;
        if (v==0)pos0=size;
        if (size>=5000){std::cerr<<"Too many numbers!"<<std::endl;return 1;}
        ref[size].val = v;
        mylist->add_to_front(&ref[size]);
        size++;
        mylist = mylist->front;
        read_or_die("\n",input);
    }
    input.close();

    if(verb_lvl>1){auto a=mylist;do{std::cout<<a->val<<' ';a=a->front;}while(a!=mylist);std::cout<<std::endl;}
    for (int i=0;i<1+9*part2;i++)
    for (u_t j=0;j<size;j++) {
        ll_t A=ref[j].val;
        ref[j].loosen();
        if (A<=0) {
            // There's a -1 because the element we are moving is not counted.
            A = -1*((-1*A)%(size-1));
            for (auto b=0;b<-1*A;b++)ref[j].back=ref[j].back->back;
            ref[j].front = ref[j].back->front;
        } else {
            // There's a -1 because the element we are moving is not counted.
            A = A%(size-1);
            for (auto b=0;b<A;b++)ref[j].front=ref[j].front->front;
            ref[j].back=ref[j].front->back;
        }
        ref[j].fasten();
        if(verb_lvl>1)std::cout<<"Moved "<<ref[j].val<<" between "<<ref[j].back->val<<" and "<<ref[j].front->val<<std::endl;
    }

    ll_t ans=0;
    mylist=&ref[pos0];
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
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
