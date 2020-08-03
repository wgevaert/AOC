#include <iostream>
#include <fstream>
#include <string>
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
    long long unsigned checksum = 0;
    do {
        unsigned line_answer = 0,temp;
        std::vector<unsigned> values;
        do {
            input_file>>temp;
            for (auto i :values) {
                if (!(temp%i)) //temp is multiple of i
                    line_answer = temp/i;
                if (!(i%temp)) //i is multiple of temp
                    line_answer = i/temp;
                if (line_answer !=0){
                    std::cout<<i<<' '<<temp<<'='<<line_answer<<std::endl;
                    break;
                }
            }
            values.push_back(temp);
        } while (input_file.get() !=10 && input_file.good());
        if (line_answer == 0) {std::cout<<"ERROR: NO DIVISIBLE NUMBERS FOUND IN ";for(auto i:values)std::cout<<i<<' ';std::cout<<std::endl;return 1;}
        checksum += line_answer;
        std::cout<<"SUBTOTAL: "<<checksum<<std::endl;
    } while(!input_file.eof());
    std::cout<<"ASWER: "<<checksum<<std::endl;
    return 0;
}
