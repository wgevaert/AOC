#include <iostream>
#include <fstream>
#include <string>

void update(short&dx,short&dy,bool&infected){
    static unsigned infected_cnt=0;
    if(dx==0) {
        if(dy==1){
            dx=(infected ? -1:1);
        } else {
            dx = (infected ? 1 :-1);
        }
        dy=0;
    } else {
        if(dx ==1) {
            dy=(infected ? 1 : -1);
        } else {
            dy = (infected ? -1:1);
        }
        dx = 0;
    }
    if(!infected)std::cout<<++infected_cnt<<' ';
    infected=!infected;
}


int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: {program} {name of input file} {iterations}"<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    const unsigned mid=512,size=2*mid+1;
    static bool cluster[size][size];
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }
    unsigned x_min=mid-12,x_max,x=x_min,y_min=mid-12,y_max=y_min,y=y_min;
    char a;
    while(!input_file.eof()){while((a=input_file.get())!='\n' && !input_file.eof()){if(a=='#')cluster[x][y]=true;else if(a=='.')cluster[x][y]=false;else {std::cout<<"UNKNOWN CHAR "<<a<<std::endl;return 1;}if(x_max<x)x_max=x;x++;}if(!input_file.eof()){x=x_min;y=++y_max;}}
    y_max--;x=(x_min+x_max)/2;y=(y_min+y_max)/2;
    for(unsigned i=y_min-1;i<=y_max+1;i++){std::cout<<std::endl;for(unsigned j=x_min-1;j<=x_max+1;j++)std::cout<<((j==x&&i==y)?'@':(cluster[j][i]?'#':'.'));}

    short dx=0,dy=-1;
    update(dx,dy,cluster[x][y]);
    for(int iter=0;iter<std::stoi(argv[2]);iter++){
        for(unsigned i=y_min-1;i<=y_max+1;i++){std::cout<<std::endl;for(unsigned j=x_min-1;j<=x_max+1;j++)std::cout<<((j==x&&i==y)?'@':(cluster[j][i]?'#':'.'));}
        update(dx,dy,cluster[x+=dx][y+=dy]);
        if(x<x_min)x_min=x;
        else if(x>x_max)x_max=x;
        else if(y<y_min)y_min=y;
        else if(y>y_max)y_max=y;
    }
std::cout<<std::endl;
    return 0;
}
