#include <iostream>


const int moons_cnt = 4;

struct moon {
    int pos[3];
    int vel[3]={0,0,0};
    moon(int x,int y,int z,int v_x,int v_y,int v_z) {
        pos[0]=x;pos[1]=y;pos[2]=z;vel[0]=v_x;vel[1]=v_y;vel[2]=v_z;
    }
    moon(int x,int y,int z) {
        pos[0]=x;pos[1]=y;pos[2]=z;
    }
};

std::ostream & operator<<(std::ostream&out, const moon &m) {
    out<<'('<<m.pos[0]<<','<<m.pos[1]<<','<<m.pos[2]<<')'<<':'<<'{'<<m.vel[0]<<','<<m.vel[1]<<','<<m.vel[2]<<'}';
    return out;

}

void compare (moon &a,moon&b) {
    for (int i=0;i<3;i++) {
        if (a.pos[i] < b.pos[i]) {
            std::cout<<a<<'~'<<b<<" AT "<<i<<" BECOMES (<) ";
            ++a.vel[i];--b.vel[i];std::cout<<a<<' '<<b<<std::endl;
        } else if (a.pos[i] > b.pos[i]) {
            std::cout<<a<<'~'<<b<<" AT "<<i<<" BECOMES (>) ";
            --a.vel[i];++b.vel[i];
            std::cout<<a<<' '<<b<<std::endl;
        } else std::cout<<a<<'~'<<b<<" DO NOT CHANGE AT "<<i<<std::endl;
    };
}

int abs (int a) {
    if(a<0)return -1*a;
    return a;
}

int energy (const moon a) {
    return (abs(a.pos[0])+abs(a.pos[1])+abs(a.pos[2]))*(abs(a.vel[0])+abs(a.vel[1])+abs(a.vel[2]));
}

int main() {
    moon mymoons[moons_cnt]={moon(103,0,0,102,0,0), moon(-63,0,0,-82,0,0), moon(-63,0,0,-82), moon(-63,0,0,-82)};//{moon(-10,-13,7),moon(1,2,1),moon(-15,-3,13),moon(3,7,-4)};
    const int steps = 200;
    int E=0;
    for (int i=0;i<steps;i++) {
        for(int j=0;j<moons_cnt-1;j++)for(int k=j+1;k<moons_cnt;k++)compare(mymoons[j],mymoons[k]);
        for (int j=0;j<moons_cnt;j++) for (int k=0;k<moons_cnt;k++) mymoons[j].pos[k]+=mymoons[j].vel[k];
        std::cout<<std::endl<<"AT STEP "<<i<<std::endl;
        for (int k=0;k<moons_cnt;k++)std::cout<<mymoons[k]<<std::endl;
    }
    std::cout<<"TOTAL ENERGY AFTER "<<steps<<" STEPS: ";
    for (int k=0;k<moons_cnt;k++){ std::cout<<mymoons[k]<<" HAS "<<energy(mymoons[k])<<" TOTAL ENERGY "<<std::endl; E +=energy(mymoons[k]);}
    std::cout<<"FOR A GRAND TOTAL OF "<<E<<std::endl;
}
