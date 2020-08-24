#include <iostream>



int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage: {program} {input}"<<std::endl;return 0;}
    size_t size = std::stoull(argv[1]);
    bool* elves = new bool[size];
    for(int i=0;i<size;i++)elves[i]=true;
    int cnt=size;
    bool has_units=true;
    while(cnt>1){
        for(size_t i=0;i<size;i++){
            if(elves[i]){//std::cout<<'@';
                if(!has_units){cnt--;elves[i]=false;}
                has_units=!has_units;
            }//else std::cout<<'.';
        }
//        std::cout<<cnt<<std::endl;
    }
    for(size_t i=0;i<size;i++)if(elves[i])std::cout<<i+1<<" is possible"<<std::endl;
    delete[] elves;
    return 0;
}
