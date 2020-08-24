#include "md5.h"
#include <iostream>
#include <string>

int main(int argc, char**argv){
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    unsigned a=0,i=0,b=0;
    const unsigned char nothing = static_cast<unsigned char>(-1);
    unsigned char password[9]={nothing,nothing,nothing,nothing,nothing,nothing,nothing,nothing,'\0'};
    while(a<8||b<8){
        std::string result = md5(std::string(argv[1]) + std::to_string(i));
        if('0'==result[0]&&result[0]==result[1]&&result[1]==result[2]&&result[2]==result[3]&&result[3]==result[4]){
            if(a<8)std::cout<<result[5];
            a++;
            if('0'<=result[5]&&result[5]<='7' && password[result[5]-'0']==nothing){
                b++;
                password[result[5]-'0']=result[6];
//                std::cout<<result[6]<<' '<<b<<std::endl;
            }
        }
        i++;
    }
    std::cout<<std::endl<<password<<std::endl;
    return 0;
}
