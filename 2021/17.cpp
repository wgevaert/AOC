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

const ll_t tr(const ll_t in,const ll_t in2) {if(in<in2)return tr(in2,in);return (in-in2+1)*(in+in2)/2;}// Sum of in2,...,in

const ll_t tr(const ll_t in) {return tr(in,0);}//triangle numbers;

ull_t find_y_hits_between(ll_t smin,ull_t smax,ll_t ymin,ll_t ymax,ll_t yvmax) {
    // stupid but works; TODO: caching results or something clever.
    ull_t r=0;
    for (ll_t vy=yvmax;vy>=ymin;vy--){
        ll_t s=0;ll_t Vy=vy,py=0;
        if(vy>0){s=2*vy+1;Vy=-1*vy-1;}
        if(verb_lvl>6)std::cout<<"trying "<<vy<<" starting with "<<s<<std::endl;
        do {
            if(verb_lvl>5)std::cout<<py<<" should be between "<<ymax<<' '<<ymin<<" when "<<s<<" is between "<<smax<<' '<<smin<<". Vy is "<<Vy<<std::endl;
            if(s>=smin&&py<=ymax&&py>=ymin){
                if(verb_lvl>2)std::cout<<"Found y hit for vel "<<vy<<" at step "<<s<<std::endl;
                r++;break;
            }
            py+=Vy;Vy--;s++;
        } while(s<=smax&&py>=ymin);
    }
    if(verb_lvl>1)std::cout<<"Found "<<r<<" steps between "<<smin<<" and "<<smax<<" steps"<<std::endl;
    return r;
}


int real_main(int argc, char** argv) {
    if (verb_lvl > 0) {
        std::cout<<"Running in verbosity mode "<<verb_lvl<<std::endl;
    }

    // Don't wanna do IO
    //const ll_t xmin=20,xmax=30, ymin=-10,ymax=-5;
    const ll_t xmin=253,xmax=280,ymin=-73,ymax=-46;
    const ll_t yvmax = -1*ymin-1, step_max=2*yvmax+5;

    // for part 1, assume there is a working x velocity.
    std::cout<<tr(yvmax)<<std::endl;

    static ull_t step=0, px=0, tip_speed=1, total = 0;

    for (ll_t vx=1;vx<=xmax;vx++) {
        px+=vx;step++;
        while(px > xmax){
            px -= tip_speed++;step--;
        }
        if (px >= xmin){
            ll_t smin=step,smax=step;
            if (tip_speed==1)smax=step_max;
            ll_t ts=tip_speed,Px=px;
            while (Px>=xmin){
                smin--;Px-=ts;ts++;
            }smin++;
            if(verb_lvl>3)std::cout<<"For x vel "<<vx<<" step should be between "<<smin<<" and "<<smax<<std::endl;
            total += find_y_hits_between(smin, smax, ymin, ymax, yvmax);
        }
    }

    std::cout<<total<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    if (argc>=2)verb_lvl=std::stoi(argv[1]);
    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
