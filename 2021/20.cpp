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

const size_t field_size = 2000;

template <typename T>
class field {
    ull_t cx=field_size/2,cy=field_size/2,xmin=cx,xmax=cx,ymin=cy,ymax=cy;
    T thefield[field_size][field_size];
public:
    const T empty;
    field(T e): empty(e) {for (auto i=0;i<field_size;i++)for (auto j=0;j<field_size;j++)thefield[j][i]=empty;}
    void set(T val, ll_t x, ll_t y) {
        x+=cx;y+=cy;
        if (x>=field_size||x<0||y>=field_size||y<0) {
            std::cerr<<x<<','<<y<<" does not fit inside "<<field_size<<" field."<<std::endl;
            exit(1);
        }
        if (val != empty) {
            if (x<xmin)xmin=x;
            if (x>xmax)xmax=x;
            if (y<ymin)ymin=y;
            if (y>ymax)ymax=y;
        }
        thefield[y][x]=val;
    }
    T get(ll_t x,ll_t y) {
        x+=cx;y+=cy;
        if (x<=xmax&&x>=xmin&&y<=ymax&&y>=ymin)return thefield[y][x];
        return empty;
    }
    ll_t get_xmin() {
        return static_cast<ll_t>(xmin)-static_cast<ll_t>(cx);
    }
    ll_t get_xmax() {
        return static_cast<ll_t>(xmax)-static_cast<ll_t>(cx);
    }
    ll_t get_ymin() {
        return static_cast<ll_t>(ymin)-static_cast<ll_t>(cy);
    }
    ll_t get_ymax() {
        return static_cast<ll_t>(ymax)-static_cast<ll_t>(cy);
    }
    void print(ll_t margin) {
        for (ll_t y=get_ymin()-margin;y<=get_ymax()+margin;y++) {
            for (ll_t x=get_xmin()-margin;x<=get_xmax()+margin;x++)
                std::cout<<get(x,y);
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
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

    std::string algo;
    std::getline(input, algo);

    if (algo[0]!='#'||algo[511]!='.') {
        std::cerr<<"Assuming blinking, to do no blinking, edit source"<<std::endl;
        return 1;
    }
    field<char> fields[2] = {field<char>('.'), field<char>('#')};
    u_t cur = 0;

    ll_t x,y=0;
    read_or_die("\n",input);
    char a = input.get();
    do {
        x=0;
        do {
            fields[cur].set(a,x,y);
            x++;
            a = input.get();
       } while (!input.eof()&&a!='\n');
       y++;
       a=input.get();
    } while(a!='\n' && !input.eof());
    input.close();

    const ll_t rounds=50;
    for (auto a=0;a<rounds;a++) {
        for (auto y=fields[cur].get_ymin()-1;y<=fields[cur].get_ymax()+1;y++)for (auto x=fields[cur].get_xmin()-1;x<=fields[cur].get_xmax()+1;x++) {
            u_t val = 0;
            for (int i=-1;i<=1;i++)for(int j=-1;j<=1;j++)val = (val<<1)|(fields[cur].get(x+j,y+i)=='.' ? 0 : 1); 
            fields[(cur+1)%2].set(algo[val], x, y);
        }
        fields[(cur+1)%2].print(2);
        cur = (cur+1)%2;
    }

    ull_t ans1 = 0;
    for (auto y=fields[cur].get_ymin();y<=fields[cur].get_ymax();y++)for (auto x=fields[cur].get_xmin();x<=fields[cur].get_xmax();x++)ans1+=fields[cur].get(x,y)!=fields[cur].empty;

    std::cout<<ans1<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
