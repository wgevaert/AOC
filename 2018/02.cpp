#include <iostream>
#include <fstream>
#include <string>

typedef std::pair<unsigned long,unsigned long> answer_t;

answer_t& operator+=(answer_t& a,answer_t b){a.first+=b.first;a.second+=b.second;return a;}

answer_t check(std::string line){
    unsigned char_cnt[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    answer_t rtr(0,0);
    for(char a:line)char_cnt[a-'a']++;
    for(size_t i=0;i<26;i++)if(char_cnt[i]==2)rtr.first=1;else if(char_cnt[i]==3)rtr.second=1;
    return rtr;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::fstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    std::string line;
    answer_t answer;
    std::getline(input_file,line);
    while(line.size()){
        answer+=check(line);
        std::getline(input_file,line);
    }
    std::cout<<answer.first*answer.second<<std::endl;

    return 0;
}
