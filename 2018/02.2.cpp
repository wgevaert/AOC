#include <iostream>
#include <fstream>
#include <vector>

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
    for(auto a=lines.begin();a<lines.end()-1;a++)for(auto b=a+1;b<lines.end();b++){
        unsigned difs =0;
        if(a->size()!=b->size())break;
        for(size_t i=0;i<a->size();i++)if((*a)[i]!=(*b)[i])difs++;
        if(difs <= 1){for(size_t i=0;i<a->size();i++)if((*a)[i]==(*b)[i])std::cout<<(*a)[i];std::cout<<std::endl;}
    }
}
