#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

bool read_line(auto& input_file, auto& container, long long& max) {
    std::string reg_mod,reg_test;
    long long int_mod,int_test;
    long long sign;
    input_file>>reg_mod;
    if (input_file.eof()) {
        return false;
    }
    if (input_file.get()!=' '){
        std::cerr<<"Something went wrong :("<<std::endl;return false;
    }
    switch(input_file.get()){
        case 'i':
            if (input_file.get()!='n'||input_file.get()!='c'||input_file.get()!=' ') {
                std::cerr<<"Something went wrong :(("<<std::endl;return false;
            }
            sign=+1;
            break;
        case 'd':
            if (input_file.get()!='e'||input_file.get()!='c'||input_file.get()!=' ') {
                std::cerr<<"Something went wrong :((("<<std::endl;return false;
            }
            sign=-1;
            break;
        default:
            std::cerr<<"Something went wrong :O"<<std::endl;return false;
    }
    input_file>>int_mod;
    if(input_file.get()!=' '||input_file.get()!='i'||input_file.get()!='f'||input_file.get()!=' '){
        std::cerr<<"Something went wrong :P"<<std::endl;return false;
    }
    input_file>>reg_test;
    if(input_file.get()!=' '){
        std::cerr<<"Something went wrong :("<<std::endl;return false;
    }
    bool (*comp)(long long,long long);
    switch(input_file.get()) {
        case '<':
            switch(input_file.get()){
                case '=':
                    comp=[](long long a,long long b){return a<=b;};
                    input_file.get();
                    break;
                case ' ':
                    comp=[](long long a,long long b){return a<b;};
                    break;
                default:
                    std::cerr<<"Something went wrong D:"<<std::endl;return false;
            }
            break;
        case '>':
            switch(input_file.get()){
                case '=':
                    comp=[](long long a,long long b){return a>=b;};
                    input_file.get();
                    break;
                case ' ':
                    comp=[](long long a,long long b){return a>b;};
                    break;
                default:
                    std::cerr<<"Something went wrong D:"<<std::endl;return false;
            }
            break;
        case '!':
            if(input_file.get()!='='||input_file.get()!=' '){
                std::cerr<<"Something went wrong D:"<<std::endl;return false;
            }
            comp=[](long long a,long long b){return a!=b;};
            break;
        case '=':
            if(input_file.get()!='='||input_file.get()!=' '){
                std::cerr<<"Something went wrong D:"<<std::endl;return false;
            }
            comp=[](long long a,long long b){return a==b;};
            break;
        default:
            std::cerr<<"Something went wrong D:"<<std::endl;return false;
    }
    input_file>>int_test;
    if ((!container.count(reg_test) && comp(0,int_test)) || comp(container[reg_test],int_test)) {
        if(!container.count(reg_mod)) {
            container[reg_mod] = sign*int_mod;
        } else {
            container[reg_mod] += sign*int_mod;
        }
        if(container[reg_mod] >= max)
            max=container[reg_mod];
    }
    return true;
}

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

    std::unordered_map<std::string,long long> registers;
    long long max_in_process=0;

    while(read_line(input_file, registers, max_in_process));

    long long max=0;
    for(auto i:registers) {
        if(i.second > max)max=i.second;
    }
    std::cout<<max<<' '<<max_in_process<<std::endl;
    return 0;
}
