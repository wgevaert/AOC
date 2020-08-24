#include <iostream>
#include <fstream>
#include <vector>

struct step {
    std::vector<unsigned short> requirements={};
    bool completed = false,almost_completed=false;
    unsigned time,worker=-1;
};

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::fstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    step steps[26];
    for(unsigned i=0;i<26;i++)steps[i].time=61+i;
    const unsigned short worker_cnt = 6;
    unsigned short workers[worker_cnt];
    for(int i=0;i<worker_cnt;i++)workers[i] = -1;
    unsigned short max=0;
    while(input_file.get()=='S') {
        unsigned short a,b;
        while(input_file.get()!=' ');
        a=input_file.get()-'A';
        while(input_file.get()!='p');
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        b=input_file.get()-'A';
        while(input_file.get()!='\n');
        steps[b].requirements.push_back(a);
        if(a>max)max=a;if(b>max)max=b;
    }
    max++;
    bool complete;
    unsigned cnt=0;
    do {
        complete = true;
        for(int i=0;i<worker_cnt;i++) {
            if(workers[i]<max){
                steps[workers[i]].time--;
                if(!steps[workers[i]].time)
                    workers[i]=-1;
            }
        }
        ++cnt;
        for(int i=0;i<max;i++)
            if(steps[i].time) {
                complete = false;
                if(steps[i].worker>=worker_cnt){
                    bool can_complete=true;
                    for (unsigned short a :steps[i].requirements)
                        if(steps[a].time){
                            can_complete=false;
                            break;
                        }
                    if(can_complete) {
                        unsigned short assign_to = -1;
                        for(unsigned short j=0;j<worker_cnt;j++)if(workers[j]>=max){assign_to=j;break;}
                        if(assign_to < worker_cnt){steps[i].worker=assign_to;workers[assign_to]=i;}
                    }
                }
            }
    }while(!complete);
    std::cout<<cnt-1<<std::endl;
    return 0;
}
