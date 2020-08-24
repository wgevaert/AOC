#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

struct guard{
    unsigned asleep[60]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
};

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return  0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<"BAD!"<<std::endl; return 1;}
    std::vector<std::string> lines;
    std::string line;
    std::getline(input_file,line);
    while(line.size()){
        lines.push_back(line);
        std::getline(input_file,line);
    }
    std::sort(lines.begin(),lines.end());

    unsigned cur_guard,asleep;
    std::unordered_map<unsigned,guard> guards;
    for(auto a:lines){
        unsigned time=(a[15]-'0')*10+a[16]-'0';
        switch(a[19]){
            case 'f':asleep=time;break;
            case 'w':for(size_t i=asleep;i<time;i++)guards[cur_guard].asleep[i]++;break;
            case 'G':cur_guard=0;for(unsigned i=0;a[26+i]!=' ';i++)cur_guard=cur_guard*10+a[26+i]-'0';break;
            default:std::cout<<a[19]<<" was unexpected"<<std::endl;
        }
    }

    unsigned max_total_sleep=0,max_moment_sleep=0;
    unsigned answer1,answer2;
    for(auto a:guards){
        unsigned asleep=0,max=0,arg_max=-1;
        for(size_t i=0;i<60;i++){
            asleep+=a.second.asleep[i];
            if(max <= a.second.asleep[i]){
                max = a.second.asleep[i];
                arg_max=i;
            }
            if(max_moment_sleep<=a.second.asleep[i]){max_moment_sleep=a.second.asleep[i];answer2=i*a.first;}
        }
        if(asleep >= max_total_sleep){max_total_sleep=asleep;answer1=arg_max*a.first;}
    }
    std::cout<<answer1<<' '<<answer2<<std::endl;
    return 0;
}
