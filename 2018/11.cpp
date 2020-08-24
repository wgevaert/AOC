#include <iostream>
#include <tuple>

int get_power_level(long x,long y, long serial){
    long rack_id = x+10;
    return (((rack_id*y+serial)*rack_id)/100)%10 - 5;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input}"<<std::endl;return 0;}
    long serial_number=std::stol(argv[1]);
    std::cout<<get_power_level(3,5,8)<<" should be 4"<<std::endl;
    int answer[3];
    int max = 0;
    for (int size=1;size<=300;size++) {// While implementing a faster way, this had already finished
        for(int i=1;i<=300-size;i++)for(int j=1;j<=300-size;j++){
            int cur_ans=0;
            for(int k=0;k<size;k++)for(int l=0;l<size;l++)cur_ans+=get_power_level(i+k,j+l,serial_number);
            if(cur_ans > max){
                max = cur_ans;
                answer[0]=i;answer[1]=j;answer[2]=size;
            }
        }
std::cout<<size<<std::endl;
    }
    std::cout<<answer[0]<<','<<answer[1]<<','<<answer[2]<<std::endl;
    return 0;
}
