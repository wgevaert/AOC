#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    std::string line;
    std::getline(input_file,line);
    while(line.size()>0){
        int i=0;
        while(i<line.size()&& (line[i]>'9'||line[i]<'0'))i++;
        int j=i;
        while(j<line.size()&&line[j]<='9'&&line[j]>='0')j++;
        unsigned id= std::stoi(line.substr(i,j-i));
        std::cout<<id<<':';
        for(auto a:line)if(a=='-')std::cout<<' ';else std::cout<<static_cast<char>((a-'a'+id)%26+'a');
        std::cout<<std::endl;
        std::getline(input_file,line);
    }
    return 0;
}
