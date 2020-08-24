#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool supports_TLS (std::string line) {
    bool in_unit=false,good=false;
    for(unsigned i=0;i<(line.size()-3);i++){
        if(line[i]=='[')in_unit=true;
        else if(line[i]==']')in_unit=false;
        else if(line[i]==line[i+3]&&line[i]!=line[i+1]&&line[i+1]==line[i+2]){if(in_unit)return false;else good=true;}
    }
    return good;
}

bool supports_SSL (std::string line) {
    bool in_unit=false;
    std::vector<std::pair<char,char>> aba={},bab={};
    for(unsigned i=0;i<(line.size()-2);i++){
        if(line[i]=='[')in_unit=true;
        else if(line[i]==']')in_unit=false;
        else if(line[i]==line[i+2]&&line[i]!=line[i+1]){if(in_unit)bab.emplace_back(line[i+1],line[i]);else aba.emplace_back(line[i],line[i+1]);}
    }
    for(auto a:aba)for(auto b:bab)if(a.first==b.first&&a.second==b.second)return true;
    return false;
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is not good"<<std::endl;return 1;}
    std::string line;
    std::getline(input_file,line);
    unsigned TLS_cnt=0,SSL_cnt=0;
    while(!input_file.eof()){
        TLS_cnt+=supports_TLS(line);
        SSL_cnt+=supports_SSL(line);
        std::getline(input_file,line);
    }
    std::cout<<TLS_cnt<<' '<<SSL_cnt<<std::endl;
    return 0;
}
