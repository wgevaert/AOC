#include <iostream>
#include <fstream>
#include <vector>


struct field_elt {
    char c='#';
    bool seen=false;
    unsigned short steps=0;
};

struct field {
private:
    static const long base=512, field_size = 2*base +1;
    field_elt f[field_size][field_size];
    unsigned x_min=base,x_max=base+1,y_min=base,y_max=base+1;
    unsigned real(int a){return base+2*a;}
    friend std::ostream& operator<<(std::ostream& out,field& f);
public:
    field(){for(int i=0;i<field_size;i++)for(int j=0;j<field_size;j++)f[i][j].c='#';f[base][base].c='X';f[base][base].seen=true;}
    void make_door(int x,int y,int new_x,int new_y){
        if(f[real(x)][real(y)].c=='#'){std::cout<<"YOU CAN'T MAKE DOORS LIKE THAT! from "<<x<<','<<y<<" to "<<new_x<<','<<new_y<<std::endl;exit(1);}
        unsigned r_n_x=real(new_x),r_n_y=real(new_y);
        f[r_n_x][r_n_y].c='.';
        f[base+x+new_x][base+y+new_y].c=' ';//Doors are just spaces, that's easier
        if(r_n_x<x_min)x_min=r_n_x;
        if(r_n_x>=x_max)x_max=r_n_x+1;
        if(r_n_y<y_min)y_min=r_n_y;
        if(r_n_y>=y_max)y_max=r_n_y+1;
    }
    std::pair<unsigned,unsigned> do_BFW() {
        std::vector<std::pair<int,int>> flowers[2]={{std::make_pair(0,0)},{}};
        std::pair<int,int>directions[4]={std::make_pair(-1,0),std::make_pair(0,-1),std::make_pair(1,0),std::make_pair(0,1)};
        unsigned p=0,q=1,ans1=0,ans2=0;
        bool flowing;
        do {
            flowing = false;
            for(auto a:flowers[p]) {
                for(auto b:directions){
                    int n_x=a.first+b.first,n_y=a.second+b.second;
                    if (f[base+a.first+n_x][base+a.second+n_y].c==' ' && !f[real(n_x)][real(n_y)].seen){
                        flowers[q].emplace_back(n_x,n_y);
                        f[real(n_x)][real(n_y)].seen=true;
                        f[real(n_x)][real(n_y)].steps=ans1+1;
                        flowing = true;
                    }
                }
            }
            p=1-p;q=1-q;
            ans1++;
        }while(flowing);
        for(unsigned i=x_min;i<x_max;i+=2)for(unsigned j=y_min;j<y_max;j+=2)if(f[i][j].steps >=1000)ans2++;
        return {ans1-1,ans2};
    }
};

std::ostream& operator<<(std::ostream& out,field& f) {
    for(unsigned j=f.y_min-1;j<=f.y_max;j++){
        for (unsigned i=f.x_min-1;i<=f.x_max;i++)
            out<<(f.f[i][j].steps ? static_cast<char>('a'+(f.f[i][j].steps-1)%26): (f.f[i][j].seen ? 'O' : f.f[i][j].c));
        out<<'\n';
    }
    return out;
}

std::ostream& operator<<(std::ostream& out,std::pair<unsigned,unsigned> p) {
    out<<p.first<<','<<p.second;
    return out;
}

struct walker{
    int x,y;
    void walk(char a, field& f){
        switch(a) {
            case 'N':f.make_door(x,y,x,y-1);y--;break;
            case 'S':f.make_door(x,y,x,y+1);y++;break;
            case 'E':f.make_door(x,y,x+1,y);x++;break;
            case 'W':f.make_door(x,y,x-1,y);x--;break;
            default:std::cout<<"BAD THINGS HAVE HAPPENED"<<std::endl;exit(1);
        }
    }
    walker(int _x,int _y):x(_x),y(_y){};
};

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<walker> walkers={};
    walkers.emplace_back(0,0);
    field facility;
    bool done=false;
    if(input_file.get()!='^'){std::cout<<"^ expected, probably a wrong file?"<<std::endl;return 1;}
    while(!done) {
        char a;
        switch(a=input_file.get()) {
            case 'N':
            case 'S':
            case 'E':
            case 'W':
                walkers.back().walk(a,facility);
                break;
            case '(':
                walkers.emplace_back(walkers.back().x,walkers.back().y);
                break;
            case '|':
                walkers.pop_back();
                walkers.emplace_back(walkers.back().x,walkers.back().y);
                break;
            case ')':
                walkers.pop_back();
                break;
            case '$':
                done=true;
                break;
            default:std::cout<<a<<" was unexpected"<<std::endl;return 1;
        }
    }
    std::cout<<facility<<std::endl;
    std::cout<<facility.do_BFW()<<std::endl;
    return 0;
}
