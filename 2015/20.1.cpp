#include <iostream>

const unsigned input = 34000000,goal[2]={input/10,(input+10)/11};

int main(){
    static unsigned presents[2][input/10+1];
    bool answered[2]={false,false};
    for(unsigned i=1;i<=goal[0]+1;i++)for(unsigned j=i-1;j<=goal[0];j+=i)presents[0][j]+=i;
    for(unsigned i=1;i<=goal[1]+1;i++)for(unsigned j=i-1;j<=goal[1]&&j<50*i;j+=i)presents[1][j]+=i;
    for(unsigned i=0;i<=goal[0];i++){if(!answered[0]&&presents[0][i]>=goal[0]){answered[0]=true;std::cout<<"FOR 1, "<<i+1<<" IS THE ANSWER"<<std::endl;}if(!answered[1]&&presents[1][i]>=goal[1]){answered[1]=true;std::cout<<"FOR 2, "<<i+1<<" IS THE ANSWER"<<std::endl;}}
    return 0;
}
