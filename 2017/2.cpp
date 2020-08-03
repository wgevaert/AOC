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
    long long unsigned checksum = 0;
    do {
        unsigned min=-1,temp,max=0;
        do {
            input_file>>temp;
            if (temp < min)min=temp;
            if (temp > max)max=temp;
            std::cout<<min<<' '<<temp<<' '<<max<<std::endl;
        } while (input_file.get() !=10 && input_file.good());
        checksum += max - min;
        std::cout<<"SUBTOTAL: "<<checksum<<std::endl;
    } while(!input_file.eof());
    std::cout<<"ASWER: "<<checksum<<std::endl;
    return 0;
}
