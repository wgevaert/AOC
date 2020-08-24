#include <iostream>
#include <fstream>
#include <vector>

struct particle{
    long long x,y,vx,vy;
    void step_back(){
        x-=vx;y-=vy;
    }
    void step(){
        x+=vx;y+=vy;
    }
};

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    std::vector<particle> particles={};
    while(input_file.get()=='p') {
        particles.emplace_back();
        while(input_file.get()!='<');
        input_file>>particles.back().x;
        if(input_file.get()!=','){std::cout<<", expected"<<std::endl;return 1;}
        input_file>>particles.back().y;
        while(input_file.get()!='<');
        input_file>>particles.back().vx;
        if(input_file.get()!=','){std::cout<<", expected"<<std::endl;return 1;}
        input_file>>particles.back().vy;
        if(input_file.get()!='>'||input_file.get()!='\n'){std::cout<<">\\n expected"<<std::endl;return 1;}
    }
    unsigned long cnt=0,happy_cnt=0;
    long long prev_x_max = 1ll<<62,prev_x_min=-1*(1ll<<62),prev_y_max=1ll<<62,prev_y_min=-1*(1ll<<62);
    while(true){
        long long x_min = 1ll<<62,x_max=-1*(1ll<<62),y_min=1ll<<62,y_max=-1*(1ll<<62);
        for(auto& a:particles){
            a.step();
            if(x_min>a.x)x_min=a.x;
            if(x_max<a.x)x_max=a.x;
            if(y_min>a.y)y_min=a.y;
            if(y_max<a.y)y_max=a.y;
        }
        cnt++;
        if(prev_x_max < x_max || prev_x_min > x_min || prev_y_max < y_max || prev_y_min > y_min){
            for(auto& a:particles)a.step_back();
            if(++happy_cnt>1)break;
            std::cout<<cnt-1<<':'<<std::endl;
            for(unsigned j=prev_y_min;j<=prev_y_max;j++){for(unsigned i=prev_x_min;i<=prev_x_max;i++){
                bool is_particle= false;
                for(auto a:particles)if(a.x==i&&a.y==j){is_particle=true;break;}
                std::cout<<(is_particle ? '#':'.');
            }std::cout<<std::endl;}
            for(auto& a:particles)a.step();
        }
        prev_x_max =x_max;prev_x_min =x_min;prev_y_max =y_max;prev_y_min =y_min;
    }
    return 0;
}
