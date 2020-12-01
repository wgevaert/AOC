#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr<<"Usage: "<<argv[0]<<" {input_file}"<<std::endl;
        exit(1);
    }
    std::ifstream input(argv[1]);

    if (!input.good()) {
        std::cerr<<"Input file "<<argv[1]<<" did not open correctly"<<std::endl;
        exit(1);
    }

    bool in_list[2020];

    for (int i=0;i<2020;i++) {
        in_list[i]=false;
    }

    unsigned number;

    while(!input.eof()) {
        input>>number;
        in_list[number] = true;
        if (in_list[2020-number]) {
            std::cout<<"Part 1: "<<(number * (2020-number)) <<" with i="<<number<<" and j="<<(2020-number)<<std::endl;
        }
    }

    input.close();

    // all info we need for part 2 is now in in_list.

    for (int i=1;i<2020/3;i++) for(int j=i+1;j<2020/2;j++)
        if (in_list[i] && in_list[j] && in_list[2020-i-j]) {
            std::cout<<"Part 2: "<<(i*j*(2020-i-j))<<" with i="<<i<<" and j="<<j<<" and k="<<(2020-i-j)<<std::endl;
        }

    return 0;
}
