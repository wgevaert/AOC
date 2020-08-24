#include <iostream>
#include <vector>
#include <string>

std::vector<bool> checksum(const std::vector<bool>& input,size_t size){
    if(size%2){
        return input;
    }
    std::vector<bool>output;
    for(size_t i=0;i<size;i+=2){
        output.emplace_back(input[i]==input[i+1]);
    }
    return checksum(output,output.size());
}

int main(int argc, char **argv) {
    if (argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    std::vector<bool> data; //bool vectors are nice
    size_t i=0;
    size_t size = (argc<3 ? 272 : std::stoul(argv[2]));
    while(argv[1][i]){
        data.emplace_back(argv[1][i]-'0');
        i++;
    }
    while(data.size()<size){
        size_t i=data.size();
        data.emplace_back(false);
        do {
            data.emplace_back(!data[--i]);
        } while(i!=0);
    }
    for (auto b:checksum(data,size))std::cout<<b;
        std::cout<<std::endl;
    return 0;
}
