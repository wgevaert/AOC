#include <iostream>
#include <fstream>
#include <vector>

struct fabric_sq {
    std::vector<unsigned> claimed_by;
};

const unsigned size=1000;

bool read_line(std::ifstream& input_file,fabric_sq **fabric){
    if(input_file.get()!='#'){std::cout<<"# expected"<<std::endl;return false;}
    unsigned id,x,y,dx,dy;
    input_file>>id;
    if(input_file.get()!=' '||input_file.get()!='@'||input_file.get()!=' '){std::cout<<" @  expected"<<std::endl;return false;}
    input_file>>x;
    if(input_file.get()!=','){std::cout<<", expected"<<std::endl;return false;}
    input_file>>y;
    if(input_file.get()!=':'||input_file.get()!=' '){std::cout<<":  expected"<<std::endl;return false;}
    input_file>>dx;
    if(input_file.get()!='x'){std::cout<<"x expected"<<std::endl;return false;}
    input_file>>dy;
    if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return false;}
    for(size_t i=0;i<dx;i++)for(size_t j=0;j<dy;j++)fabric[x+i][y+j].claimed_by.push_back(id);
    return true;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    fabric_sq *fabric[size];
    for(unsigned i=0;i<size;i++)fabric[i]=new fabric_sq[size];
    unsigned cnt=0,doubles=0;
    while(read_line(input_file,fabric))cnt++;
    if(!input_file.eof()){std::cout<<"expected eof"<<std::endl;return 1;}
    std::vector<bool> used_twice(cnt,false);
    for(int i=0;i<size;i++)
        for(int j=0;j<size;j++)
            if(fabric[i][j].claimed_by.size() >1){
                doubles++;
                for(auto a:fabric[i][j].claimed_by)
                    used_twice[a-1]=true;
            }
    for(size_t i=0;i<used_twice.size();i++){
        if(!used_twice[i])std::cout<<i+1<<" has a good claim"<<std::endl;
    }
    std::cout<<doubles<<std::endl;
    for(unsigned i=0;i<size;i++)delete[] fabric[i];

    return 0;
}
