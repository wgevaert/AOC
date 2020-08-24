#include <iostream>
#include "md5.h"
#include <vector>

struct walker {
    unsigned x,y;
    std::string path;
    walker(unsigned _x,unsigned _y,std::string _path){x=_x;y=_y;path=_path;}
};

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage:{program} {input}"<<std::endl;}
    std::vector<walker> walkers[2];
    walkers[0].emplace_back(0,0,argv[1]);
    unsigned short p=0,q=1,path_length=0,last,substr_start=walkers[0][0].path.size();
    bool answered = false;
    while(walkers[p].size()) {
        for(walker w : walkers[p]) {
            if(w.x==3&&w.y==3){
                if (!answered){std::cout<<w.path.substr(substr_start)<<std::endl;answered=true;}
                last = path_length;
            } else if (w.y > 0 || w.y <3||w.x > 0||w.x <3){
                auto unit = md5(w.path);
                if(unit[0]>'a' && w.y > 0)walkers[q].emplace_back(w.x,w.y-1,w.path + 'U');
                if(unit[1]>'a' && w.y < 3)walkers[q].emplace_back(w.x,w.y+1,w.path + 'D');
                if(unit[2]>'a' && w.x > 0)walkers[q].emplace_back(w.x-1,w.y,w.path + 'L');
                if(unit[3]>'a' && w.x < 3)walkers[q].emplace_back(w.x+1,w.y,w.path + 'R');
            }
        }
        ++path_length;
        walkers[p].clear();
        p=1-p;q=1-q;
    }
    std::cout<<last<<std::endl;
    return 0;
}
