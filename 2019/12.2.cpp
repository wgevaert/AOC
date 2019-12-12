#include <iostream>

struct moon {
    int pos[3];
    int vel[3]={0,0,0};
    moon(int x,int y,int z,int v_x,int v_y,int v_z) {
        pos[0]=x;pos[1]=y;pos[2]=z;vel[0]=v_x;vel[1]=v_y;vel[2]=v_z;
    }
    moon(int x,int y,int z) {
        pos[0]=x;pos[1]=y;pos[2]=z;
    }
    bool operator ==(const moon m) {return pos[0]==m.pos[0]&&pos[1]==m.pos[1]&&pos[2]==m.pos[2]&&vel[0]==m.vel[0]&&vel[1]==m.vel[1]&&vel[2]==m.vel[2];
    }
};

std::ostream & operator<<(std::ostream&out, const moon &m) {
    out<<"pos=<x= "<<m.pos[0]<<", y= "<<m.pos[1]<<", z= "<<m.pos[2]<< ">, vel=<x= "<<m.vel[0]<<", y= "<<m.vel[1]<<", z= "<<m.vel[2]<<'>';
    return out;

}

void compare (moon &a,moon&b) {
    for (int i=0;i<3;i++) {
        if (a.pos[i] < b.pos[i]) {
            //std::cout<<a<<'~'<<b<<" AT "<<i<<" BECOMES (<) ";
            ++a.vel[i];--b.vel[i];//std::cout<<a<<' '<<b<<std::endl;
        } else if (a.pos[i] > b.pos[i]) {
            //std::cout<<a<<'~'<<b<<" AT "<<i<<" BECOMES (>) ";
            --a.vel[i];++b.vel[i];
            //std::cout<<a<<' '<<b<<std::endl;
        } else {}//std::cout<<a<<'~'<<b<<" DO NOT CHANGE AT "<<i<<std::endl;
    };
}

int main() {
/* Previous Brute force position:
90380000000:
pos=<x= 474, y= 474, z= -87>, vel=<x= -6, y= 11, z= 7>
pos=<x= -530, y= -73, z= 14>, vel=<x= -35, y= -18, z= 9>
pos=<x= -291, y= -509, z= 47>, vel=<x= 7, y= -22, z= -4>
pos=<x= 326, y= 101, z= 43>, vel=<x= 34, y= 29, z= -12>
*/
    const moon start_pos[4]={moon(7/*-10*/,0,0/*-13,7*/),moon(1/*1*/,0,0/*2,1*/),moon(13/*-15*/,0,0/*-3,13*/),moon(-4/*3*/,0,0/*7,-4*/)};//{moon(-1,0,2),moon(2,-10,-7),moon(4,-8,8),moon(3,5,-1)};
    moon mymoons[4]={moon(7/*-10*/,0,0/*-13,7*/),moon(1/*1*/,0,0/*2,1*/),moon(13/*-15*/,0,0/*-3,13*/),moon(-4/*3*/,0,0/*7,-4*/)};//{moon(-10,0,0/*-13,7*/),moon(1,0,0/*2,1*/),moon(-15,0,0/*-3,13*/),moon(3,0,0/*7,-4*/)};
    long long unsigned i=0;
    if (!(mymoons[0]==start_pos[0]&&mymoons[1]==start_pos[1]&&mymoons[2]==start_pos[2]&&mymoons[3]==start_pos[3])){ std::cout<<"UNIT TEST FAILED"<<std::endl;return 1;}
    while(true) {
        for(int j=0;j<3;j++)for(int k=j+1;k<4;k++)compare(mymoons[j],mymoons[k]);
        if (!(i%10000000))std::cout<<i<<":\n"<<mymoons[0]<<'\n'<<mymoons[1]<<'\n'<<mymoons[2]<<'\n'<<mymoons[3]<<std::endl<<std::endl;
        for (int j=0;j<4;j++)for (int k=0;k<3;k++) mymoons[j].pos[k]+=mymoons[j].vel[k];
//        std::cout<<std::endl<<"AT STEP "<<i<<std::endl;
//        for (int k=0;k<4;k++)std::cout<<mymoons[k]<<std::endl;
//        if (i==2772){std::cout<<mymoons[0]<<';'<<mymoons[1]<<';'<<mymoons[2]<<';'<<mymoons[3]<<std::endl;break;}
        /*if (!(i%100000))std::cout<<i<<std::endl;*/ ++i; if (mymoons[0]==start_pos[0]&&mymoons[1]==start_pos[1]&&mymoons[2]==start_pos[2]&&mymoons[3]==start_pos[3]) break;
    }
    std::cout<<"AFTER "<<i<<" STEPS, POSITION "<<mymoons[0]<<';'<<mymoons[1]<<';'<<mymoons[2]<<';'<<mymoons[3]<<" OBTAINED"<<std::endl;
    return 0;
}
