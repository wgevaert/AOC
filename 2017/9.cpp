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
    long long unsigned score_total=0,cur_score=0,garbage_total=0;
    bool in_garbage=false;
    while (!input_file.eof()) {
        if (!in_garbage) {
            switch(input_file.get()) {
                case '{':score_total+=++cur_score;break;
                case '}':--cur_score;break;
                case '<':in_garbage=true;break;
            }
        }else{
            switch(input_file.get()) {
                case '!':input_file.get();break;
                case '>':in_garbage=false;break;
                default:garbage_total++;
            }
        }
    }
std::cout<<score_total<<' '<<garbage_total<<std::endl;
    return 0;
}
