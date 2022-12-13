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
                    parsed_pattern += "?("+std::to_string(static_cast<int>(b))+")";
                else
                    parsed_pattern += b;
            }
            std::cerr<<'\''<<parsed_pattern<<'\''<<" expected"<<std::endl;
            exit(1);
        }
    }
}

const u_t uinf=-1;

// global vars, how risky.
u_t expll,explr;

// TODO: rewrite to just a struct with "depth" and "value"
class stupidNumber {
public:
    stupidNumber *left=NULL,*right=NULL;
    u_t val=uinf;
    stupidNumber(u_t v){val=v;left=right=NULL;}
    stupidNumber(stupidNumber* l, stupidNumber* r){left=l;right=r;}
    ~stupidNumber() {if(left!=NULL)delete left;if(right!=NULL)delete right;if(verb_lvl>6)std::cout<<"Deleting "<<val<<std::endl;}
    stupidNumber* getCopy() {
        if (val!=uinf)return new stupidNumber(val);
        return new stupidNumber(left->getCopy(),right->getCopy());
    }
    bool split() {
        if (val != uinf && val > 9) {
            if (verb_lvl>6)std::cout<<"Trying to split "<<val<<std::endl;
            left = new stupidNumber(val/2);
            right = new stupidNumber((val+1)/2);
            val = uinf;

            if(verb_lvl>6)std::cout<<"splitting"<<std::endl;
            return true;
        }
        if (val != uinf)return false;
        if(left->split())return true;
        if(right->split())return true;
        return false;
    }
    bool explode(u_t d) {
        if (val!=uinf)return false;
        if(d>=4) {
            // Assume we're never deeper than this.
            if (left->val==uinf||right->val==uinf) {
                std::cerr<<"Oh noes!"<<std::endl;exit(1);
            }
            explr=left->val;
            expll=right->val;
            val=0;
            delete left;left=NULL;
            delete right;right=NULL;

            if(verb_lvl>7)std::cout<<"Exploded"<<std::endl;
            return true;
        }
        if (left->explode(d+1)) {
            right->addToLeft();
            return true;
        }
        if (right->explode(d+1)) {
            left->addToRight();
            return true;
        }
        return false;
    }
    void addToLeft() {
        if (expll==uinf){
            if(verb_lvl>5)std::cout<<"Aborting ATL"<<std::endl;
            return;
        }
        if (val!=uinf){
            if(verb_lvl>4)std::cout<<"Adding lvalue "<<expll<<" to "<<val<<std::endl;
            val+=expll;
            expll=uinf;
        } else left->addToLeft();
    }
    void addToRight() {
        if (explr==uinf){
            if(verb_lvl>5)std::cout<<"Aborting ATR"<<std::endl;
            return;
        }
        if (val!=uinf){
            if(verb_lvl>4)std::cout<<"Adding rvalue "<<explr<<" to "<<val<<std::endl;
            val+=explr;
            explr=uinf;
        } else right->addToRight();
    }
    ull_t getM() {
        if (val!=uinf)return val;
        return left->getM()*3+right->getM()*2;
    }
    void print() {
        if (val!=uinf){std::cout<<val;return;}
        std::cout<<'[';left->print();std::cout<<',';right->print();std::cout<<']';
    }
};

stupidNumber* readN(std::istream& in) {
    char a=in.peek();
    if (a=='[') {
        read_or_die("[",in);
        stupidNumber* l=readN(in);
        read_or_die(",",in);
        stupidNumber* r=readN(in);
        read_or_die("]",in);
        auto rtr= new stupidNumber(l,r);
        if(verb_lvl>7)std::cout<<"Stored "<<l<<' '<<r<<" in "<<rtr<<std::endl;
        return rtr;
    }
    u_t val = in.get()-'0';
    auto rtr= new stupidNumber(val);
    if(verb_lvl>7)std::cout<<"Stored "<<val<<" in "<<rtr<<std::endl;
    return rtr;
}

stupidNumber* addAndSimplify(stupidNumber* n, stupidNumber* m) {
    stupidNumber* rtr=new stupidNumber(n,m);
    while (rtr->explode(0)||rtr->split())if(verb_lvl>4){rtr->print();std::cout<<std::endl;}
    if(verb_lvl>2)std::cout<<"Done simplifying"<<std::endl;
    if(verb_lvl>2){rtr->print();std::cout<<std::endl;}
    return rtr;
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

    stupidNumber* n=readN(input),*m,*result1;
    std::vector<stupidNumber*>ins={n};
    result1=n->getCopy();
    read_or_die("\n",input);

    if(verb_lvl > 2){n->print();std::cout<<std::endl;}
    if(verb_lvl)std::cout<<"WOEI!"<<std::endl;
    do {
        if(verb_lvl)std::cout<<"WOEI!"<<std::endl;
        m=readN(input);
        ins.push_back(m);
        read_or_die("\n",input);
        if(verb_lvl > 2){m->print();std::cout<<std::endl;}
        if(verb_lvl)std::cout<<"WOEI!"<<std::endl;
        result1=addAndSimplify(result1,m->getCopy());
        if(verb_lvl)std::cout<<"WOEI!"<<std::endl;
    } while(input.peek()!='\n'&&!input.eof());
    std::cout<<result1->getM()<<std::endl;
    delete result1;
    input.close();

    ull_t max=0;
    for (size_t i=1;i<ins.size();i++)for(size_t j=0;j<i;j++){
        stupidNumber*m=addAndSimplify(ins[i]->getCopy(),ins[j]->getCopy());ull_t mag=m->getM();if(mag>max)max=mag;delete m;
        m=addAndSimplify(ins[j]->getCopy(),ins[i]->getCopy());mag=m->getM();if(mag>max)max=mag;delete m;
    }
    std::cout<<max<<std::endl;

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
