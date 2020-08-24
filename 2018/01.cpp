#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::fstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    long cur_freq=0,freq_change,answer1;
    bool answered2=false;
    std::vector<long> freqs={};
    input_file>>freq_change;
    unsigned long min_round=-1,answer2;
    do {
        freqs.push_back(cur_freq);
        cur_freq+=freq_change;
        if(!answered2)for(long a:freqs)if(a==cur_freq){answered2=true;answer2=a;}
        input_file>>freq_change;
    }while(!input_file.eof());
    answer1=cur_freq;
    if(!answered2){
        for(auto b=freqs.begin()+1;b!=freqs.end();b++)for(auto a=freqs.begin();*a!=*b;a++)
            if(*b>*a && !((*b-*a)%answer1)){
                if(static_cast<unsigned long>((*b-*a)/answer1)<min_round){
                    min_round=(*b-*a+answer1-1)/answer1;answer2=*a;
                }
            }
            else if(*a>*b && !((*a-*b)%answer1)){
                if(static_cast<unsigned long>((*a-*b)/answer1)<min_round){
                    min_round=(*a-*b+answer1-1)/answer1;answer2=*a;
                }
            }
    }
    std::cout<<answer1<<" is the answer to 1 and "<<answer2<<" to 2, after "<<min_round<<" rounds"<<std::endl;
    return 0;
}
