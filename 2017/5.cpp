#include <iostream>
#include <fstream>
#include <vector>

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

    int a;
    std::vector<long long> b={};
    for(;;){
        input_file>>a;
        if(input_file.eof())
            break;
        b.push_back(a);
        std::cout<<a<<' ';
    }
    std::cout<<std::endl;

    int pos=0;unsigned steps = 0;
    while (pos >=0 && pos <b.size()) {
        pos+=b[pos]++;
        ++steps;
    }
    std::cout<<"ANSWER: "<<steps<<std::endl;
    return 0;
}
