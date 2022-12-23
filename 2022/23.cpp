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

const size_t field_size = 2000;

class elf {
public:
     ll_t x,y, pdx, pdy;
     elf(ll_t X, ll_t Y):x(X),y(Y){}
};

void print_char(const char a, std::ostream& o) {
    if (a=='#') {
        o<<"5 ";
    } else {
        o<<std::to_string(static_cast<int>(a-'0'))<<' ';
    }
}

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

    std::vector<elf> elves = {};

    ll_t x,y=0;
    char a = input.get();
    do {
        x=0;
        do {
            if (a == '#') elves.emplace_back(x,y);
            x++;
            a = input.get();
       } while (!input.eof()&&a!='\n');
       y++;
       a=input.get();
    } while(a!='\n' && !input.eof());
    input.close();

    int dxs[4] = {0,0,-1,1}, dys[4] = {-1,1,0,0}, adx1[4]={1,1,-1,1}, ady1[4]={-1,1,1,1}, adx2[4]={-1,-1,-1,1}, ady2[4]={-1,1,-1,-1}, curd=0;

    bool moving = true;
    u_t round=0;
    const ll_t x_max=130,x_min=-16,y_max=127,y_min=-16;
    const bool make_pic=false;
    while(moving) {
        round++;
        moving=false;
        field<char> thefield('0');
        for (auto& e:elves) {
            thefield.set('#',e.x,e.y);
        }
        if (round == 11) std::cout<<((thefield.get_xmax()-thefield.get_xmin()+1)*(thefield.get_ymax()-thefield.get_ymin()+1)-elves.size())<<std::endl;
        for (auto& e:elves) {
            e.pdx=e.pdy=0;// Don't move if you can't move;
            if (thefield.get(e.x,e.y+1) != '#' &&
                thefield.get(e.x+1,e.y+1) != '#' &&
                thefield.get(e.x+1,e.y) != '#' &&
                thefield.get(e.x+1,e.y-1) != '#' &&
                thefield.get(e.x,e.y-1) != '#' &&
                thefield.get(e.x-1,e.y-1) != '#' &&
                thefield.get(e.x-1,e.y) != '#' &&
                thefield.get(e.x-1,e.y+1) != '#'
            ) {
                continue;
            }
            moving=true;
            for (u_t d=0;d<4;d++) {
                char a = thefield.get(e.x+dxs[(curd+d)%4], e.y+dys[(curd+d)%4]);
                char b = thefield.get(e.x+adx1[(curd+d)%4], e.y+ady1[(curd+d)%4]);
                char c = thefield.get(e.x+adx2[(curd+d)%4], e.y+ady2[(curd+d)%4]);
                if (a != '#' && b != '#' && c != '#') {
                    e.pdx = dxs[(curd+d)%4];
                    e.pdy = dys[(curd+d)%4];
                    thefield.set(a+1,e.x+e.pdx,e.y+e.pdy);
                    break;
                }
            }
        }
        if (make_pic) {
            std::string filename = std::to_string(round);
            filename = "23-"+std::string(3-filename.size(),'0')+filename+".pgm";
            std::ofstream file(filename);
            file<<"P2 "<<(x_max-x_min+1)<<' '<<(y_max-y_min+1)<<" 5 ";
            for (auto a=y_max;a>=y_min;a--)for(auto b=x_min;b<=x_max;b++)print_char(thefield.get(b,a),file);
        }
        if(verb_lvl)thefield.print(1);
        for (auto& e:elves) {
            if (thefield.get(e.x+e.pdx,e.y+e.pdy) == '1') {
                e.x+=e.pdx;
                e.y+=e.pdy;
            }
        }
        curd++;
    }

    std::cout<<round<<std::endl;
    

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
