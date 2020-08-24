#include <iostream>
#include <vector>

int main(int argc, char** argv){
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;}
    unsigned input = std::stoul(argv[1]);
    std::vector<unsigned short> recipes = {3,7};
    unsigned elve0=0,elve1=1;
    while(recipes.size() <= input+10) {
        unsigned short unit = recipes[elve0]+recipes[elve1];
        if(unit>=10)recipes.emplace_back(unit/10);
        recipes.emplace_back(unit%10);
        elve0 = (elve0 + recipes[elve0]+1)%recipes.size();
        elve1 = (elve1 + recipes[elve1]+1)%recipes.size();
    }
    for(int i=0;i<10;i++)std::cout<<recipes[input+i];
    std::cout<<std::endl;
    return 0;
}
