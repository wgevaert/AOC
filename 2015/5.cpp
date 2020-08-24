#include <iostream>

int main() {
    long unsigned nice_cnt=0;
    int vowel_cnt;
    bool twice,happy;
    char a,prev='$';

    while(!std::cin.eof()) {
        prev = '$';twice=false;happy=true;vowel_cnt=0;
        while(!std::cin.eof() && (a=std::cin.get())!='\n') {
            switch (a) {
                case 'a':
                case 'e':
                case 'i':
                case 'o':
                case 'u':
                    vowel_cnt++;
                    if(prev==a) {
                        twice=true;
                        std::cout<<'('<<a<<'='<<prev<<')';
                    }std::cout<<'{'<<a<<'}';
                    break;
                case 'b':
                case 'd':
                case 'q':
                case 'y':
                    if(prev == a-1){
                        happy=false;// no break
                        std::cout<<'!'<<prev<<a<<'!';
                    }
                default :
                    if(prev==a) {
                        twice=true;
                        std::cout<<'('<<a<<'='<<prev<<')';
                    }std::cout<<a;
            }
            prev=a;
        }
        if (twice && happy && vowel_cnt >=3) {
            ++nice_cnt;
            std::cout<<" <-";
        }
        std::cout<<std::endl;
    }
    std::cout<<"answer: "<<nice_cnt<<std::endl;
}
