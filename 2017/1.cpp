#include <iostream>
#include <fstream>
#include <string>

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
    std::string content;
    input_file >> content;
    std::cout<<"Read the following in the file "<<filename<<':'<<'\n'<<content<<std::endl;

    long long unsigned total=0;
    std::cout<<"size of file is "<<content.length()<<std::endl;
    for (unsigned i=0;i<content.length();i++) {
        if (content[i] == content[(i+(content.length()/2))%content.length()]) {
            std::cout<<content[i]<<'+';
            total += static_cast<unsigned>(content[i]-'0');
        }
    }
    std::cout<<'='<<total<<std::endl;
    return 0;
}
