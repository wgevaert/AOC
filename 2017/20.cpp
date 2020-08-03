#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

struct particle {
    long p[3],v[3],a[3];
    bool alive=true,dying=false;
    particle at(long step);
};

particle particle::at(long step) {
    particle rtr=*this;
    for(int i=0;i<3;i++){
        rtr.p[i]+=step*rtr.v[i]+step*(step+1)*rtr.a[i]/2;
        rtr.v[i]+=rtr.a[i]*step;
    }
    return rtr;
}

unsigned long abs(long a) {
    if(a<0)
        return static_cast<unsigned long>(-1*a);
    return static_cast<unsigned long>(a);
}

unsigned long norm(long* a) {
    return abs(a[0])+abs(a[1])+abs(a[2]);
}

bool square(long& a) {
    if(a<0)return false;
    if(a==0)return true;
    long ans=std::sqrt(a);
    if(ans*ans!=a){ans++;if(ans*ans!=a)return false;}
    a=ans;
    return true;
}

void read_particles(std::ifstream &input_file,std::vector<particle>&particles){
    for(;;){
        particle return_particle;
        if((input_file.get()!='p'||input_file.get()!='='||input_file.get()!='<')&&!input_file.eof())
            std::cout<<"OOPS"<<std::endl;
        if(input_file.eof())break;
        input_file>>return_particle.p[0];
        if(input_file.get()!=',')
            std::cout<<"OOPs"<<std::endl;
        input_file>>return_particle.p[1];
        if(input_file.get()!=',')
            std::cout<<"OOpS"<<std::endl;
        input_file>>return_particle.p[2];
        if(input_file.get()!='>'||input_file.get()!=','||input_file.get()!=' '||input_file.get()!='v'||input_file.get()!='='||input_file.get()!='<')
            std::cout<<"OOps"<<std::endl;
        input_file>>return_particle.v[0];
        if(input_file.get()!=',')
            std::cout<<"oOPs"<<std::endl;
        input_file>>return_particle.v[1];
        if(input_file.get()!=',')
            std::cout<<"oOpS"<<std::endl;
        input_file>>return_particle.v[2];
        if(input_file.get()!='>'||input_file.get()!=','||input_file.get()!=' '||input_file.get()!='a'||input_file.get()!='='||input_file.get()!='<')
            std::cout<<"oOps"<<std::endl;
        input_file>>return_particle.a[0];
        if(input_file.get()!=',')
            std::cout<<"oOPs"<<std::endl;
        input_file>>return_particle.a[1];
        if(input_file.get()!=',')
            std::cout<<"oOpS"<<std::endl;
        input_file>>return_particle.a[2];
        if(input_file.get()!='>'||input_file.get()!='\n')
            std::cout<<"OOOPS"<<std::endl;
        particles.push_back(return_particle);
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {name of input file}"<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }
    std::vector<particle> particles={};
    read_particles(input_file,particles);
    std::cout<<"READ "<<particles.size()<<" PARTICLES"<<std::endl;

    unsigned long min_a =-1,cur_a;
    for(particle x:particles){
        cur_a=norm(x.a);
        if(min_a>cur_a)min_a=cur_a;
    }
    std::vector<unsigned> min_indices;
    for(unsigned i=0;i<particles.size();i++){
        if(norm(particles[i].a)<=min_a)
            min_indices.push_back(i);
    }
    std::cout<<"MINIMAL A OF "<<min_a<<" FOUND FOR PARTICLES ";for(unsigned i:min_indices)std::cout<<i;
    std::cout<<std::endl;

    const unsigned long NEVER = -1,steps=100;
    unsigned long step_max = 0;
    for (unsigned i=1;i<particles.size();i++)
        for(unsigned j=0;j<i;j++) {
            long P=particles[i].p[0]-particles[j].p[0],V=particles[i].v[0]-particles[j].v[0],A=particles[i].a[0]-particles[j].a[0];
            unsigned long collision_time = NEVER;
            if (P==0){
                collision_time=0;
            } else if (A==0){
                if (V!=0) {
                    if (V<0){P*=-1;V*=-1;};if (!(P%V) && P>0)collision_time = -1*P/V;
                }
            } else {
                if (A<0){A*=-1;V*=-1;P*=-1;}
                long D=2*V+A;
                D=D*D-8*A*P;
                if (square(D) && D > 2*V+A && !((D-2*V-A)%(2*abs(A)))) {
                    collision_time = (D-2*V-A)/(2*A);
                }
            }
            if (collision_time != NEVER) {
                particle pi=particles[i].at(collision_time),pj=particles[j].at(collision_time);
/*                std::cout <<':'
                          <<particles[i].at(collision_time-1).p[0]<<' '
                          <<particles[i].at(collision_time).p[0]<<' '
                          <<particles[i].at(collision_time+1).p[0]<<','
                          <<particles[j].at(collision_time-1).p[0]<<' '
                          <<particles[j].at(collision_time).p[0]<<' '
                          <<particles[j].at(collision_time+1).p[0]<<':';*/
                if (pi.p[0]!=pj.p[0]){std::cout<<" MATH IS BROKEN"<<std::endl;return 1;}
                if (pi.p[1]!=pj.p[1]||pi.p[2]!=pj.p[2])collision_time=NEVER;
                else std::cout<<i<<" AND "<<j<<" COLLIDE ON x AT "<<collision_time<<std::endl;
                if(collision_time!=NEVER && step_max < collision_time)step_max=collision_time;
                if(step_max > steps){std::cout<<"INSUFFICIENT STEPS"<<std::endl;return 1;}
            }
        }

    std::cout<<" RUNNING A SIMULATION FOR "<<step_max<<" STEPS"<<std::endl;
    unsigned particles_total=particles.size();
    for (unsigned i=0;i<=step_max;i++){
        for (unsigned j=0;j<particles.size();j++) {
            if(particles[j].alive){
                for (unsigned k=j+1;k<particles.size();k++) {
                    if (
                        particles[k].alive &&
                        particles[j].p[0]==particles[k].p[0]&&
                        particles[j].p[1]==particles[k].p[1]&&
                        particles[j].p[2]==particles[k].p[2]){
                        particles[j].alive=false;particles[k].dying=true;
                    }
                }
                if(particles[j].dying)particles[j].alive=particles[j].dying=false;
                if (!particles[j].alive)particles_total--;
            }
        }
        for(particle& a:particles)a=a.at(1);
    }
    std::cout<<particles_total;

    return 0;
}
