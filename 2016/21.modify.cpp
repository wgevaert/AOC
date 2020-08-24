#include <fstream>
#include <string>
#include <vector>
#include <iostream>

int main(int argc, char** argv) {
    std::ifstream input(argv[1]);
    std::ofstream output(argv[2]);
    std::vector<std::string> file;
    std::string line;
if(argc<3||!input.good()||!output.good()){std::cout<<" hey "<<std::endl;return 1;}
    while(!input.eof()) {
        std::getline(input,line);
        file.push_back(line);
    }input.close();
    for(size_t i=file.size()-1;i<file.size();i--)output<<file[i]<<'\n';
    output.close();
    return 0;
}
