#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

constexpr std::pair<int,int> turn_right(const std::pair<int,int> direction) {
    return std::make_pair(-1*direction.second,direction.first);
}

constexpr std::pair<int,int> turn_left(const std::pair<int,int> direction) {
    return std::make_pair(direction.second,-1*direction.first);
}

constexpr std::pair<int,int> turn_nowhere(const std::pair<int,int> direction) {
    return direction;
}

typedef std::pair<int,int> (*turn_t)(std::pair<int,int>);

struct chart {
    static turn_t turns(const unsigned index){
        static const turn_t a[3]= {turn_left,turn_nowhere,turn_right};
        return a[index];
    }
    unsigned int cur_turn=0,x,y;
    bool is_crashed=false;
    std::pair<int,int> direction;
    bool move(char pos){
        if(pos=='+'){
            direction = chart::turns(cur_turn)(direction);
            cur_turn = (cur_turn+1)%3;
        } else if (pos=='/'){
            direction = std::make_pair(-1*direction.second,-1*direction.first);
        } else if (pos=='\\'){
            direction = std::make_pair(direction.second,direction.first);
        } else if((pos=='-'&&direction.second!=0) || (pos=='|'&&direction.first!=0)){
            std::cout<<"Something is wrong here..."<<std::endl;return false;
        } else if(pos!='-'&&pos!='|'){std::cout<<"I am derailed at "<<x<<','<<y<<':'<<pos<<'.'<<std::endl;return false;}
        x+=direction.first;
        y+=direction.second;
        return true;
    }
    chart(unsigned _x,unsigned _y,int dx,int dy){
        x=_x;y=_y;direction = std::make_pair(dx,dy);
    }
    char display() {
        if(direction.first==1)return '>';
        else if(direction.first==-1)return '<';
        else if(direction.second==1)return 'v';
        else if(direction.second==-1)return '^';
        else return '?';
    }
    bool operator<(chart&b){
        if(is_crashed &&!b.is_crashed)return false;
        if(b.is_crashed&&!is_crashed)return true;
        return y < b.y || (y==b.y && x < b.x);
    }
};

const unsigned system_size = 256;

void print(char system[][system_size],std::vector<chart> charts,unsigned x_max,unsigned y_max) {
    for(unsigned j=0;j<y_max;j++){
        for(unsigned i=0;i<x_max;i++){
            char here=system[i][j];
            for(auto a:charts)
                if(a.x==i&&a.y==j){
                    if(here==system[i][j])here=a.display();
                    else here='X';
                }
            std::cout<<here;
        }
        std::cout<<j<<std::endl;
    }
}


int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    char system[system_size][system_size];
    unsigned x=0,y=0,x_max=0,y_max=0;

    std::vector<chart> charts={};
    for(;;) {
        char a = input_file.get();
        switch(a) {
            case ' ':
            case '+':
            case '|':
            case '-':
            case '\\':
            case '/':
                system[x][y]=a;x++;if(x>x_max)x_max=x;
                break;
            case '<':
                charts.emplace_back(x,y,-1,0);
                system[x][y]='-';x++;if(x>x_max)x_max=x;
                break;
            case '>':
                charts.emplace_back(x,y,1,0);
                system[x][y]='-';x++;if(x>x_max)x_max=x;
                break;
            case '^':
                charts.emplace_back(x,y,0,-1);
                system[x][y]='|';x++;if(x>x_max)x_max=x;
                break;
            case 'v':
                charts.emplace_back(x,y,0,1);
                system[x][y]='|';x++;if(x>x_max)x_max=x;
                break;
            case '\n':
                y=++y_max;x=0;
                break;
            default:if(input_file.eof())break;std::cout<<a<<" not expected"<<std::endl;return 1;
        }
        if(input_file.eof())break;
    }
//    std::cout<<x_max<<' '<<y_max<<std::endl;
//    for(auto a:charts)std::cout<<a.x<<','<<a.y<<std::endl;
    bool answered1=false;
//    std::cout<<std::endl;
//    print(system,charts,x_max,y_max);
    do {
//        std::cout<<std::endl;
//        print(system,charts,x_max,y_max);
        for(unsigned i=0;i<charts.size();i++){
            if(!charts[i].move(system[charts[i].x][charts[i].y])){print(system,charts,x_max,y_max);return 1;}
            for(unsigned j=0;j<charts.size();j++){
                if(i!=j&&charts[j].x==charts[i].x && charts[j].y==charts[i].y){
                    charts[i].is_crashed=charts[j].is_crashed=true;
                    if(!answered1){std::cout<<"answer 1: "<<charts[i].x<<','<<charts[i].y<<std::endl;answered1=true;}
                }
            }
        }
        std::sort(charts.begin(),charts.end());
        while(charts.back().is_crashed)charts.pop_back();
    }while(charts.size()>1);
    std::cout<<"answer 2: "<<charts[0].x<<','<<charts[0].y<<std::endl;
    return 0;
}
