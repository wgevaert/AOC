#include <iostream>
#include <vector>
#include <iomanip>

#define neith 0
#define torch 1
#define gear 2

const unsigned short nothing = -1;

struct c {
    unsigned long typ;
    unsigned short is_visited[3]={nothing,nothing,nothing};
    unsigned short being_visited[3]={nothing,nothing,nothing};
    unsigned short flower_cnt[3]={0,0,0};
//    unsigned short steps[3]={static_cast<unsigned short>(-1),static_cast<unsigned short>(-1),static_cast<unsigned short>(-1)};
};

const int dir_cnt=4,dir[dir_cnt][2]={{1,0},{0,1},{-1,0},{0,-1}};

bool good_typ(unsigned short equipment,unsigned typ){return typ!=equipment;}

struct flower{
    unsigned short equipment;
    long x,y;
    long unsigned steps;
    flower(unsigned short e,long _x,long _y, unsigned long s):equipment(e),x(_x),y(_y),steps(s){};
    bool good_typ(unsigned typ){return (typ==0&&equipment!=neith) || (typ==1&&equipment!=torch) || (typ==2&&equipment!=gear);}
};

char getchar(unsigned short a){return a==0 ? ',' : (a==1 ? 'v' : (a<10 ? '_' : (a<100 ? '=' : (a<1000 ? '$' : (a<10000 ? '%' : '@')))));}

int main(int argc, char** argv) {
    if(argc<3){std::cout<<"usage: {program} {depth} {X} {Y}"<<std::endl; return 0;}
    unsigned depth = std::stoul(argv[1]), X = std::stoul(argv[2]), Y= std::stoul(argv[3]), ans1=0;

    const unsigned cave_size_X=67, cave_size_Y=750, mod=20183, X_mul=16807, Y_mul=48271;
    c cave[cave_size_X][cave_size_Y];
    cave[0][0].typ=0;
    for(unsigned i=0;i<cave_size_X;i++)for(unsigned j=0;j<cave_size_Y;j++)if(j==0)cave[i][j].typ=(i*X_mul+depth)%mod;else if(i==0)cave[i][j].typ=(j*Y_mul+depth)%mod;else /*if(i==X&&j==Y)cave[i][j].typ=0; else*/ cave[i][j].typ=(cave[i-1][j].typ*cave[i][j-1].typ+depth)%mod;
    for(unsigned i=0;i<cave_size_X;i++)for(unsigned j=0;j<cave_size_Y;j++)cave[i][j].typ %=3;
    cave[X][Y].typ=0;
    for(unsigned i=0;i<=X;i++)for(unsigned j=0;j<=Y;j++)ans1+=cave[i][j].typ;
    for(unsigned j=0;j<=Y+5;j++){for(int t=0;t<3;t++){for(unsigned i=0;i<=X+5;i++)std::cout<<(cave[i][j].typ==t ? ' ' : '#');std::cout<<' ';}std::cout<<std::endl;}
    for(unsigned j=0;j<=Y+5;j++){for(unsigned i=0;i<=X+5;i++)switch((cave[i][j].typ)%3){case 0:std::cout<<'.';break;case 1:std::cout<<'=';break;case 2:std::cout<<'|';}std::cout<<std::endl;}

    std::cout<<ans1<<std::endl;

    const unsigned short switch_cnt = 6;
    std::vector<flower> flowers[2];
    std::vector<flower> switching_flowers[switch_cnt];
    flowers[0].emplace_back(torch,0,0,0);
    unsigned short p=0,q=1,steps=0,waiting=0;
    bool flowing = true;
    cave[0][0].is_visited[torch]=0;
    do {
        flowing = false;
        for(auto f:switching_flowers[steps%switch_cnt]) {
            if (nothing == cave[f.x][f.y].is_visited[f.equipment]) {
                flowers[q].emplace_back(f);
                cave[f.x][f.y].is_visited[f.equipment] = steps + 1;
                cave[f.x][f.y].flower_cnt[f.equipment]++;
                waiting=switch_cnt+1;
//std::cout<<"switching climber found at "<<flowers[q].back().x<<','<<flowers[q].back().y<<" with equipment "<<flowers[q].back().equipment<<" and "<<flowers[q].back().steps<<" steps"<<std::endl;
            }
        }
        switching_flowers[steps%switch_cnt].clear();
        for(auto f:flowers[p]){
//std::cout<<"looking at flower at "<<f.x<<','<<f.y<<" with equipment "<<f.equipment<<" and "<<f.steps<<'='<<cave[f.x][f.y].is_visited[f.equipment]<<" steps"<<std::endl;
            for(int i=0;i<3;i++)
                if(i!=f.equipment && nothing==cave[f.x][f.y].is_visited[i] && nothing==cave[f.x][f.y].being_visited[i] && good_typ(i,cave[f.x][f.y].typ)) {
                    switching_flowers[steps%switch_cnt].emplace_back(i,f.x,f.y,f.steps+7);
                    flowing = true;
                    cave[f.x][f.y].being_visited[i]=steps+1;//true;
                }
            for(int i=0;i<dir_cnt;i++) {
                long new_x = f.x+dir[i][0],new_y=f.y+dir[i][1];
                if(new_x>=0 && new_x<cave_size_X && new_y>=0 && new_y<cave_size_Y) {
//std::cout<<new_x<<','<<new_y<<':';
                    if (nothing==cave[new_x][new_y].is_visited[f.equipment]) {
//std::cout<<" not visited yet,";
                        if (f.good_typ(cave[new_x][new_y].typ)) {
//std::cout<<" and can go there!"<<std::endl;
                            flowers[q].emplace_back(f.equipment,new_x,new_y,f.steps+1);
                            flowing = true;
                            cave[new_x][new_y].is_visited[f.equipment] = steps+1;//true;
                            cave[new_x][new_y].flower_cnt[f.equipment]++;
                        }
//else {std::cout<<" but cannot go there"<<std::endl;}
                    }
//else {std::cout<<" already visited"<<std::endl;}
                }
            }
        }
//std::cout<<steps<<std::endl;
//    for(int j=0;j<=Y;j++){for(int i=0;i<=X;i++){for(int e=0;e<3;e++){if(cave[i][j].is_visited[e]==nothing)std::cout<<"...";else std::cout<<std::setfill('.')<<std::setw(3)<<cave[i][j].is_visited[e];}std::cout<<cave[i][j].typ<<' ';}std::cout<<std::endl;}std::cout<<std::endl;
//    for(int j=0;j<=Y;j++){for(int i=0;i<=X;i++){for(int e=0;e<3;e++){std::cout<<std::setfill('.')<<std::setw(3)<<cave[i][j].flower_cnt[e];}std::cout<<cave[i][j].typ<<' ';}std::cout<<std::endl;}std::cout<<std::endl;
//if(!(steps%100)) {for(int j=0;j<=Y;j++){for(int i=0;i<cave_size_X;i++){for(int e=0;e<3;e++){std::cout<<getchar(cave[i][j].flower_cnt[e]);cave[i][j].flower_cnt[e]=0;}std::cout<<' ';}std::cout<<std::setfill(' ')<<std::setw(3)<<j;}std::cout<<std::endl;}//if(cave[i][j].is_visited[e]==nothing)std::cout<<"ooooo";else std::cout<<std::setw(5)<<std::setfill('o')<<cave[i][j].is_visited[e];cave[i][j].flower_cnt[e]=0;}std::cout<<' ';}std::cout<<std::endl;}std::cout<<std::endl;}
//if(!(steps%1)) {for(int j=0;j<=Y;j++){for(int i=0;i<=X;i++){for(int e=0;e<3;e++){if(cave[i][j].is_visited[e]==nothing)std::cout<<"ooooo";else std::cout<<std::setw(5)<<std::setfill('o')<<cave[i][j].is_visited[e];cave[i][j].flower_cnt[e]=0;}std::cout<<' ';}std::cout<<std::endl;}std::cout<<std::endl;}
        flowers[p].clear();
        p=1-p;q=1-q;steps++;waiting--;
    }while(nothing==cave[X][Y].is_visited[torch]);//flowing);// && cave[X][Y].steps[torch] == static_cast<unsigned short>(-1));
    std::cout<<steps<<' '<<cave[X][Y].is_visited[torch]<<' '<<X<<','<<Y<<std::endl;
    for(int j=0;j<=Y;j++){for(int i=0;i<=X;i++){for(int e=0;e<3;e++){if(cave[i][j].is_visited[e]==nothing)std::cout<<"....";else std::cout<<std::setfill('.')<<std::setw(4)<<cave[i][j].is_visited[e];}std::cout<<cave[i][j].typ<<' ';}std::cout<<std::endl;}std::cout<<std::endl;
    return 0;
}
