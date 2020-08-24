#include <iostream>
#include <fstream>
#include <vector>

char get_char(unsigned a){
    if(a>2*26)return '?';
    if(a>26)return a-26+'A';
    return a+'a';
}

struct location {
    long first,second;
    bool on_edge=false;
    unsigned size=0;
    location(long _a,long _b){first=_a;second=_b;}
};

unsigned long dist(std::pair<long,long> a,location b){
    return std::abs(a.first-b.first)+std::abs(a.second-b.second);
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage:{program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    std::vector<location> locations={};
    long long x,y,x_min=1ll<<62,y_min=1ll<<62,x_max=-1ll*(1ll<<62),y_max=-1ll*(1ll<<62);
    for(;;) {
        input_file>>x;
        if(input_file.get()!=','||input_file.get()!=' '){if(input_file.eof())break;std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>y;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
        locations.emplace_back(x,y);
        if(x<x_min)x_min=x;if(y<y_min)y_min=y;if(x>x_max)x_max=x;if(y>y_max)y_max=y;
    }
    input_file.close();

    // You might want to change this if necessary, check the images to see if it is indeed.
    const long long margin=10;
    std::ofstream image1("06.1.pgm");
    std::ofstream image2("06.2.pgm");
    if(!image1.good()){std::cout<<"06.1.pgm is bad"<<std::endl;}
    if(!image2.good()){std::cout<<"06.2.pgm is bad"<<std::endl;}

    image1 <<"P2 "<<(x_max-x_min+2*margin+1)<<' '<<(y_max-y_min+2*margin+1)<<'\n'<<55<<std::endl;
    image2 <<"P2 "<<(x_max-x_min+2*margin+1)<<' '<<(y_max-y_min+2*margin+1)<<'\n'<<1<<std::endl;
    unsigned long answer2=0;
    for(long long j=y_min-margin;j<=y_max+margin;j++){
        for(long long i=x_min-margin;i<=x_max+margin;i++){
            unsigned long min=-1;
            unsigned unit=55;
            unsigned long tot_dist=0;
            std::pair<long,long> point(i,j);
            for(unsigned i=0;i<locations.size();i++){
                unsigned long _dist = dist(point,locations[i]);
                tot_dist+=_dist;
                if(min>_dist){min=_dist;unit=i;}else if(min==_dist){unit=55;}
            }
            if(unit!=55){
                if(j==y_min-margin||j==y_max+margin||i==x_min-margin||i==x_max+margin){
                    locations[unit].on_edge=true;
                } else if(!locations[unit].on_edge) {
                    locations[unit].size++;
                }
            }
            if(tot_dist<10000)answer2++;
            image1<<(unit==55 ? 53 : unit)<<' ';
            image2<<(tot_dist < 10000)<<' ';
        }
        image1<<std::endl;
        image2<<std::endl;
    }
    unsigned max_size=0;
    for(auto a:locations)if(!a.on_edge && a.size>max_size)max_size=a.size;
    std::cout<<" answer 1 is "<<max_size<<"\n answer 2 is "<<answer2<<std::endl;

    return 0;
}
