#include <iostream>
#include <fstream>
#include <vector>

bool has_occurred(const std::vector<std::vector<unsigned short>>& b,const unsigned& index) {
    for(unsigned long i=index-1;i<index;i--) {
        bool equal = true;
        for (unsigned j=0;j<b[0].size();j++) {
            if (b[i][j]!=b[index][j]) {
                equal = false;
                break;
            }
        }
        if (equal){
            std::cout<<"LOOP OF SIZE "<<index-i<<" FOUND"<<std::endl;
            return true;
        }
    }
    return false;

}

void do_step(std::vector<std::vector<unsigned short>>& b) {
    unsigned short max = 0;
    unsigned max_index, b_index=b.size()-1, b_size = b[b_index].size();
    std::vector<unsigned short> a;
    for(unsigned i=0;i<b_size;i++) {
        if (max <b[b_index][i]) {
            max_index = i;
            max = b[b_index][i];
        }
    }
    std::vector<unsigned short> new_configuration;
    for (unsigned i=0;i<b_size;i++) {
        unsigned short new_value = max/b_size + (((i-max_index-1+b_size)%b_size) < (max % b_size));
        if (i!=max_index)
            new_value += b[b_index][i];
        new_configuration.push_back(new_value);
    }
    b.push_back(new_configuration);
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

    unsigned short a;
    std::vector<std::vector<unsigned short>> b;
    std::vector<unsigned short> c={};
    b.push_back(c);
    for(;;){
        input_file>>a;
        if(input_file.eof())
            break;
        b[0].push_back(a);
        std::cout<<a<<' ';
    }
    std::cout<<std::endl;
    unsigned long i=0;

    while(!has_occurred(b,i++)) {
        do_step(b);
    }
    std::cout<<--i;

    return 0;
}
