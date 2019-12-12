#include <iostream>
#include <algorithm>
#include <cmath>

unsigned isqrt(unsigned value)// Stole this from the interweb.
{
    unsigned result = static_cast<unsigned>(std::sqrt(static_cast<float>(value)));
    do { ++result; } while(result * result <= value);
    do { --result; } while(result * result > value);
    return result;
}

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

long long unsigned get_catchup_time(int x1,int x2,int v1, int v2) {// Get time until next transit. (hihi, ketchup)
	if (x1==x2) return 0;
    int dx=x1-x2,dv=v1-v2;if(dx<0){dx*=-1;dv*=-1;};
    return (dv-1+isqrt((dv-1)*(dv-1)+4*dx))/2;
}

void big_step(unsigned step_size, moon& a, int i, int moons_smaller) {// Do a big step, given there are no transits.
//    std::cout<<"BIG STEP "<<(3-2*moons_smaller)<<' '<<moons_smaller<<' '<<a.vel[i]<<std::endl;
    a.pos[i] +=
        static_cast<int>(
            step_size*(step_size+1)
        )/2 * (3-2*moons_smaller) +
        static_cast<int>(step_size) * a.vel[i];
    a.vel[i] += static_cast<int>(step_size)*(3-2*moons_smaller);
}

void compare (moon &a,moon&b, int index) {
    if (a.pos[index] < b.pos[index]) {
        ++a.vel[index];--b.vel[index];
    } else if (a.pos[index] > b.pos[index]) {
       --a.vel[index];++b.vel[index];
    }
}

void small_step(moon mymoons[],int index) {
    for(int j=0;j<3;j++)for(int k=j+1;k<4;k++)compare(mymoons[j],mymoons[k], index);
    for (int j=0;j<4;j++) mymoons[j].pos[index]+=mymoons[j].vel[index];
//    std::cout<<"SMALL STEP"<<std::endl;

}

int main(int argc, char* argv[]) {
    if (argc!=2&&argc!=3) {
        std::cout<<"Part 2 must be parallelised, please provide 1 input"<<std::endl;return 1;
    }
    int index = argv[1][0] - '0';
    if (index >3||index<0){
        std::cout<<"Please only give values '0'=x, '1'=y or '2'=z"<<std::endl;return 1;
    }
    bool verbose = !(argc==3 && argv[2][0] == 'q');
    moon start_pos[4]={moon(-10,-13,7),moon(1,2,1),moon(-15,-3,13),moon(3,7,-4)};
    moon mymoons[4]={moon(-10,-13,7),moon(1,2,1),moon(-15,-3,13),moon(3,7,-4)};
    int rel_pos[4]={0,1,2,3};//Sorted position of planets
    long long unsigned time=0;
    unsigned catchup_time, dummy;
    bool init = false, initinit = false;
    do {if (init)initinit=true;
        for (int i=0;i<3;i++){
            small_step(mymoons, index);//Carefully transit in 3 steps.
            if  (!init) {
              small_step(start_pos,index);
            }
        }
        time += 3;
        std::sort(rel_pos, rel_pos+4, [mymoons, index](const int a,const int b)->bool {return mymoons[a].pos[index]<mymoons[b].pos[index];});
if(verbose) std::cout<<mymoons[rel_pos[0]].pos[index]<<','<<mymoons[rel_pos[0]].vel[index]<<"   "<<mymoons[rel_pos[1]].pos[index]<<','<<mymoons[rel_pos[1]].vel[index]<<"   "<<mymoons[rel_pos[2]].pos[index]<<','<<mymoons[rel_pos[2]].vel[index]<<"   "<<mymoons[rel_pos[3]].pos[index]<<','<<mymoons[rel_pos[3]].vel[index]<<" <~ "<<time<<std::endl;
if(verbose) for(int i=0;i<4;i++) std::cout<<rel_pos[i]<<','<<mymoons[rel_pos[i]].pos[index]<<" IS LESS THAN ";
if(verbose) std::cout<<std::endl;
        catchup_time = -1;//overflow to max.
        for (int i=0;i<3;i++)
            if ((dummy=get_catchup_time(mymoons[rel_pos[i]].pos[index],mymoons[rel_pos[i+1]].pos[index], mymoons[rel_pos[i]].vel[index], mymoons[rel_pos[i+1]].vel[index])) < catchup_time) {
                catchup_time = dummy;
if (verbose) std::cout<<'('<<dummy<<')'<<' ';
            } else if (verbose)std::cout<<dummy<<' ';
if(verbose)  std::cout<<" ~> "<<catchup_time<<std::endl;
        time +=catchup_time;
        for (int i=0;i<4;i++){//Do all steps until next transit in one go
            big_step(catchup_time, mymoons[rel_pos[i]], index, i);
            if (!init) {// Easier to calculate time relative to starting position close to transit.
                big_step(catchup_time, start_pos[rel_pos[i]], index, i);
                time = 0;
            }
        }
        if (!init && catchup_time > 1)init=true;
    } while (!initinit || start_pos[0].pos[index]!=mymoons[0].pos[index] || start_pos[0].vel[index]!=mymoons[0].vel[index] || start_pos[1].pos[index]!=mymoons[1].pos[index] || start_pos[1].vel[index]!=mymoons[1].vel[index] || start_pos[2].pos[index]!=mymoons[2].pos[index] || start_pos[2].vel[index]!=mymoons[2].vel[index] || start_pos[3].pos[index]!=mymoons[3].pos[index] || start_pos[3].vel[index]!=mymoons[3].vel[index]);
if(verbose)    std::cout<<"ANSWER: ";
    std::cout<<time<<std::endl;
if(verbose) std::cout<<" You should return the lcm of the 3 answers for the respective inputs 0,1,2"<<std::endl;

    return 0;
}
