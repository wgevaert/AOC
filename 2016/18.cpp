#include <iostream>
#include <fstream>
#include <vector>

std::vector<bool> read_input(char* input_name) {
    std::ifstream input_file(input_name);
    if(!input_file.good()){std::cout<<input_name<<" is bad"<<std::endl;exit(1);}
    std::vector<bool> rtr={};
    char a=input_file.get();
    while(a!='\n'){if(input_file.eof()){std::cout<<"Unexpected ending found"<<std::endl;exit(1);}rtr.emplace_back(a=='.');a=input_file.get();}
    return rtr;
}

bool get_tile(size_t index,std::vector<bool> prev_row){
    //safe if left == right;
    bool left=(index?prev_row[index-1]:true);
    bool right=(prev_row.size()==index+1?true:prev_row[index+1]);
    return left == right;
}

int main(int argc, char** argv) {
    if(argc < 2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    std::vector<bool> rows[2]={read_input(argv[1]),{}};
    rows[1] = std::vector<bool>(rows[0].size());
    std::cout<<rows[1].size()<<std::endl;
    size_t a=0,b=1,cols=(argc>=3?std::stoul(argv[2]):40);
    unsigned total=0;
    for(auto x:rows[a])
        total+=x;
    for(int i=0;i<cols-1;i++){
        for(size_t i=0;i<rows[a].size();i++){
            rows[b][i]=get_tile(i,rows[a]);
        }
        a = 1-a;b=1-b;
        for(auto x:rows[a])
            total+=x;
        if(!(i%10000))std::cout<<i<<std::endl;
    }
    std::cout<<total<<std::endl;
    return 0;
}
