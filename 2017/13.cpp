#include <iostream>
#include <fstream>
#include <unordered_map>

struct scanner {
    unsigned range,position=0;
    short direction=-1;
};

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

    std::unordered_map<int,scanner> scanners;
    int max=0;
    while(!input_file.eof()){
        int depth;
        scanner new_scanner;
        input_file>>depth;
        if(input_file.eof())break;
        if(input_file.get()!=':'||input_file.get()!=' '){
            std::cout<<"Something wrong with the file"<<std::endl;
            return 1;
        }
        input_file>>new_scanner.range;
        scanners[depth]=new_scanner;
        if (max<depth)max=depth;
    }

    long long my_pos=0,severity =0;
    while (my_pos<=max) {
        if (scanners.count(my_pos) && !scanners[my_pos].position)
            severity += my_pos*scanners[my_pos].range;
        for (auto& i : scanners) {
            if (i.second.position==0 || i.second.position==i.second.range-1){
                i.second.direction = -1*i.second.direction;
            }
            i.second.position+=i.second.direction;
        }
        my_pos++;
    }
    std::cout<<"SEVERITY WHEN GOING DIRECTLY: "<<severity<<std::endl;
    return 0;
}
