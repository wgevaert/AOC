#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

uint32_t verb_lvl = 0;

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

void debug(std::string msg, uint32_t v) {
    if (verb_lvl > v) {
        std::cout<<msg<<std::flush;
    }
}

struct circ_elt {
    uint64_t val;
    circ_elt* prev=NULL;
    circ_elt(uint64_t v):val(v){};
};

struct circ {
    circ_elt *cur=NULL;

    circ(uint64_t v){cur=new circ_elt(v); cur->prev = cur;};
    ~circ() {circ_elt *next = cur->prev,*tmp;while (next!=cur){tmp=next;next=next->prev;delete tmp;}delete next;};

    void insert(circ_elt* ins, circ_elt* at) {
        circ_elt* next = at->prev;
        at->prev = ins;
        while (ins->prev != NULL)
            ins=ins->prev;
        ins->prev = next;
    }

    circ_elt* take_3 (circ_elt* at) {
        circ_elt* rtr = at->prev;
        at->prev = rtr->prev->prev->prev;
        rtr->prev->prev->prev = NULL;
        return rtr;
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

    circ_elt* units[10], *units2[1000001];

    circ circle(input.get()-'0');
    circ circle2(circle.cur->val);

    units[circle.cur->val] = circle.cur;
    units2[circle2.cur->val] = circle2.cur;

    for (int i=1;i<9;i++) {
        circle.insert(new circ_elt(input.get()-'0'), circle.cur);
        circle.cur = circle.cur->prev;
        units[circle.cur->val] = circle.cur;

        circle2.insert(new circ_elt(circle.cur->val), circle2.cur);
        circle2.cur = circle2.cur->prev;
        units2[circle2.cur->val] = circle2.cur;
    }

    for (int i=10;i<=1000000;i++) {
        circle2.insert(new circ_elt(i), circle2.cur);
        circle2.cur = circle2.cur->prev;
        units2[circle2.cur->val] = circle2.cur;
    }

    // Loop back to start
    circle.cur = circle.cur->prev;
    circle2.cur = circle2.cur->prev;

    if (verb_lvl > 3) {
        for (int i=1;i<=9;i++) {
           std::cout<<i<<':'<<units[i]->val<<std::endl;
        }

        circ_elt* me=circle.cur;
        do {
            std::cout<<me->val<<',';
            me = me->prev;
        } while (me != circle.cur);
    }

    for (int i=0; i<100; i++) {
        circ_elt* taken = circle.take_3(circle.cur);
        uint64_t next_val = circle.cur->val;
        do {
            next_val--;
            if (!next_val)
                next_val = 9;
        } while(taken->val == next_val || taken->prev->val == next_val || taken->prev->prev->val == next_val);
        circle.insert(taken, units[next_val]);
        circle.cur = circle.cur->prev;
    }

    for (int i=0; i<10000000; i++) {
        circ_elt* taken = circle2.take_3(circle2.cur);
        uint64_t next_val = circle2.cur->val;
        do {
            next_val--;
            if (!next_val)
                next_val = 1000000;
        } while(taken->val == next_val || taken->prev->val == next_val || taken->prev->prev->val == next_val);
        circle2.insert(taken, units2[next_val]);
        circle2.cur = circle2.cur->prev;
    }

    std::cout<<"\n";
    circ_elt * i=units[1]->prev;

    do {
        std::cout<<i->val;
        i=i->prev;
    } while (i!= units[1]);

    std::cout<<std::endl<<units2[1]->prev->val*units2[1]->prev->prev->val<<std::endl;

    input.close();

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
