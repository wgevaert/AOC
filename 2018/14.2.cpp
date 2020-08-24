#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv){
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;}
    std::string input(argv[1]);
    std::vector<unsigned short> recipes = {3,7};
    unsigned elve0=0,elve1=1,index=0;
    while(true) {
        unsigned short unit = recipes[elve0]+recipes[elve1];
        bool found=false;
        while(recipes.size()-index > input.size()){
            found = true;
            for(unsigned i=0;i<input.size();i++)
                if(static_cast<unsigned short>(input[i]-'0') != recipes[index+i]){found = false;break;}
            if(!found)index++;else break;
        }
        if(found){std::cout<<index<<std::endl;break;}
        if(unit>=10)recipes.emplace_back(unit/10);
        recipes.emplace_back(unit%10);
        elve0 = (elve0 + recipes[elve0]+1)%recipes.size();
        elve1 = (elve1 + recipes[elve1]+1)%recipes.size();
    }
    return 0;
}
