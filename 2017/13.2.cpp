#include <iostream>
#include <fstream>
#include <vector>

struct scanner {
    long unsigned period,depth;
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

    std::vector<scanner> scanners;
    while(!input_file.eof()){
        scanner new_scanner;
        input_file>>new_scanner.depth;
        if (input_file.eof()) break;
        if (input_file.get()!=':'||input_file.get()!=' '){
            std::cout<<"Something wrong with the file"<<std::endl;
            return 1;
        }
        input_file>>new_scanner.period;
        new_scanner.period = 2+2*(new_scanner.period - 2);
        scanners.push_back(new_scanner);
    }

    long long unsigned delay=0;
    bool caught;
    static bool caught_at[100];
    do {
        delay++;
        caught = false;
        for (auto s : scanners) {
            if (!((delay+s.depth)%s.period)) {
                if (!caught_at[s.depth]){std::cout<<"WITH DELAY "<<delay<<" CAUGHT AT "<<s.depth<<std::endl;caught_at[s.depth]=true;}
                caught=true;
                break;
            }
        }
    } while (caught);
    std::cout<<delay<<std::endl;
    for (int i=0;i<100;i+=2)if(!caught_at[i])std::cout<<i<<" IS INVISIBLE"<<std::endl;
    return 0;
}
