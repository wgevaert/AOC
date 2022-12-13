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

class packet {
public:
    u_t v,v_t,o; //version&v total&operator
    ull_t val=0;
    std::vector<packet> childs = {};
    packet(u_t V,u_t O){v=V;v_t=V;o=O;}
    void add(packet p) {
        childs.push_back(p);
        v_t+=p.v_t;
    }
    ull_t getVal() {
        if (o==0) {
            ull_t V=0;
            for (auto c:childs) V+=c.getVal();
            return V;
        }
        if (o==1) {
            ull_t V=1;
            for (auto c:childs) V*=c.getVal();
            return V;
        }
        if (o==2) {
            ull_t min=-1;
            for(auto c:childs){ull_t VV=c.getVal();if(VV<min)min=VV;}
            return min;
        }
        if (o==3) {
            ull_t max=0;
            for(auto c:childs){ull_t VV=c.getVal();if(VV>max)max=VV;}
            return max;
        }
        if (o==4) return val;
        if (o==5) return childs[0].getVal()>childs[1].getVal();
        if (o==6) return childs[0].getVal()<childs[1].getVal();
        if (o==7) return childs[0].getVal()==childs[1].getVal();
        return -1;
    }
};

packet read_ptree(const std::vector<bool>&bits,u_t& pos) {
    if(verb_lvl>10)std::cout<<'('<<bits.size()<<')';
    u_t v=0,o=0;
    for (u_t i=0;i<3;i++)v=(v<<1)+bits[pos+i];pos+=3;
    for (u_t i=0;i<3;i++)o=(o<<1)+bits[pos+i];pos+=3;
    if(verb_lvl>5){std::cout<<"\nNow remaining at pos "<<pos<<" with v "<<v<<",o "<<o<<": ";for(u_t i=pos;i<bits.size();i++)std::cout<<(bits[i]?'1':'0');std::cout<<std::endl;}
    packet rtr(v,o);
    bool cnt;
    switch (o) {
        case 4:
            do {
                cnt = bits[pos];
                for (u_t i=1;i<5;i++)
                    rtr.val=(rtr.val<<1)+(bits[pos+i]?1:0);
                pos +=5;
            } while(cnt);
            break;
        default:
            pos++;
            if(bits[pos-1]) {
                u_t subp = 0;
                for(u_t i=0;i<11;i++)subp=(subp<<1)+(bits[pos+i]?1:0);
                pos+=11;
                if(verb_lvl>3)std::cout<<"Reading "<<subp<<" subpackets"<<std::endl;
                for (u_t i=0;i<subp;i++) {
                    if(verb_lvl>8)std::cout<<pos<<','<<subp<<','<<i<<'_';
                    rtr.add(read_ptree(bits,pos));
                }
            } else {
                u_t target_pos=0;
                for(u_t i=0;i<15;i++)target_pos=(target_pos<<1)+(bits[pos+i]?1:0);
                pos+=15;
                if(verb_lvl>3)std::cout<<"Reading "<<target_pos<<" bits of subpackets"<<std::endl;
                target_pos+=pos;
                while(pos<target_pos) {
                    if(verb_lvl>8)std::cout<<pos<<'.';
                    rtr.add(read_ptree(bits,pos));
                }
            }
            break;
    }
    if(verb_lvl>5)std::cout<<"returning"<<std::endl;
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

    std::vector<bool> bits={};
    char a;u_t A;
    while((a=input.get())!='\n') {
        if (a<='9'&&a>='0')A=a-'0';else if(a>='A'&&a<='Z')A=a-'A'+10;else{std::cerr<<a<<static_cast<int>(a)<<" unexpected<<std"<<std::endl; exit(1);}
        for(u_t i=8;i!=0;i>>=1){
            bits.push_back(A&i);
        }
    }
    input.close();
    if(verb_lvl>4){for(auto a:bits)std::cout<<(a?'1':'0');std::cout<<std::endl;}
    u_t pos=0;
    packet tree=read_ptree(bits,pos);
    std::cout<<tree.v_t<<std::endl;
    std::cout<<tree.getVal()<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
