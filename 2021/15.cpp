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

class tile {
public:
    u_t risk;
    ull_t tot_risk=-1;
    bool in_path=false;

    tile(u_t r){risk=r;}
};

class flower {
public:
    u_t x,y;//pos
    ull_t r;//risk
    flower(u_t X,u_t Y,ull_t R){x=X;y=Y;r=R;}
};//something that flows, not something that grows.

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

std::string intToString(ull_t i, u_t mod) {
    u_t a=10,b=1;
    i=i%mod;
    std::string r=std::to_string(i);
    while(a<mod) {
        if(r.size()<=b)r="0"+r;
        a*=10;
        b+=1;
    }
    return r;
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

    std::vector<tile> tiles[5*101] = {};
    u_t y=0;
    char a;
    bool p2=false;
    do {
        a=input.get();
        do {
            tiles[y].emplace_back(a-'0');
            a=input.get();
        }while(a!='\n'&&!input.eof());
        y++;
        if (y>=101){std::cerr<<"101 is too small"<<std::endl;exit(1);}
    }while(input.peek()!='\n'&&!input.eof());
    input.close();

    if (p2) {
        // Dupe to right
        for(auto& a:tiles){size_t s=a.size();for(u_t t=0;t<4;t++)for(u_t r=0;r<s;r++)a.emplace_back((a[r].risk+t)%9+1);std::cout<<a.size()<<std::endl;}
        // Dupe down
        for (u_t t=1;t<=4;t++)for(u_t r=0;r<y;r++){tiles[t*y+r]=tiles[r];for(auto& a:tiles[t*y+r])a.risk=(a.risk+t+8)%9+1;std::cout<<tiles[t*y+r].size()<<":D"<<t<<','<<r<<std::endl;}
        y*=5;
    }
    if (verb_lvl)for (auto a:tiles){for(auto b:a)std::cout<<b.risk<<' ';std::cout<<std::endl;}

    std::vector<flower> fs[2] = {{flower(0,0,0)},{}};
    u_t c=0;
    ull_t max=-1;

    // Note: There _might_ be a field like this:
    // 59999
    // 19111
    // 11191
    // 99991
    // So we do have to check all directions.
    const std::vector<std::pair<int,int>>ds={{1,0},{-1,0},{0,1},{0,-1}};

    while(fs[c].size()) {
        if(verb_lvl>3)std::cout<<fs[c].size()<<std::endl;
        if(verb_lvl>4)for(auto a:tiles){for(auto b:a)std::cout<<(b.tot_risk==max?"  ":intToString(b.tot_risk,100));std::cout<<std::endl;}
        for (auto a:fs[c]) {
            if (a.r<=tiles[a.y][a.x].tot_risk) {
                for (auto d:ds) {
                    flower newtile(a.x+d.first,a.y+d.second,a.r);
                    if(verb_lvl>10)std::cout<<newtile.x<<' '<<newtile.y<<" testing with risk "<<newtile.r<<" from direction "<<d.first<<' '<<d.second<<" with origin "<<a.x<<' '<<a.y<<std::endl;
                    if (//check bounds
                        newtile.x+1!=0
                        &&newtile.y+1!=0
                        &&newtile.y<y
                        &&newtile.x<tiles[newtile.y].size()
                        //check risk
                        &&newtile.r<tiles[newtile.y][newtile.x].tot_risk-tiles[newtile.y][newtile.x].risk
                    ) {
                        newtile.r+=tiles[newtile.y][newtile.x].risk;
                        if(verb_lvl>10)std::cout<<newtile.x<<' '<<newtile.y<<" added with risk "<<newtile.r<<" from direction "<<d.first<<' '<<d.second<<" with origin "<<a.x<<' '<<a.y<<' '<<a.r<<" and current risk "<<tiles[newtile.y][newtile.x].risk<<std::endl;
                        tiles[newtile.y][newtile.x].tot_risk=newtile.r;
                        fs[(c+1)%2].push_back(newtile);
                    }
                }
            }
        }
        fs[c].clear();
        c=(c+1)%2;
        if(verb_lvl>3)std::cout<<fs[c].size()<<std::endl;
    }
    if(verb_lvl>3){
        size_t Y=y-1,X=tiles[y-1].size()-1, r=tiles[Y][X].tot_risk;
        while((Y!=0||X!=0)&&!tiles[Y][X].in_path) {
            r -= tiles[Y][X].risk;
            tiles[Y][X].in_path = true;
            for (auto a:ds) {
                flower n(X+a.first,Y+a.second,r);
                if (n.x+1!=0&&n.y+1!=0&&n.y<y&&n.x<tiles[n.y].size()&&tiles[n.y][n.x].tot_risk==r) {
                    if(verb_lvl>4)std::cout<<"Updated x,y from "<<X<<','<<Y<<" to "<<n.x<<','<<n.y<<std::endl;
                    X=n.x;Y=n.y;
                }
            }
        }
        for (auto a:tiles){for(auto b:a)std::cout<<(b.in_path?intToString(b.risk,10):" ");std::cout<<std::endl;}
    }

    std::cout<<"Ans:"<<tiles[y-1].rbegin()->tot_risk<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
