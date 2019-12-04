#include <iostream>

int increase(int i) {//Increase i to the next non-descending int.
    if(i%10!=9)return i+1;
    if(i%100!=99)return i+1+1*((i/10)%10+1);
    if(i%1000!=999)return i+1+11*((i/100)%10+1);
    if(i%10000!=9999)return i+1+111*((i/1000)%10+1);
    if(i%100000!=99999)return i+1+1111*((i/10000)%10+1);
    if(i%1000000!=999999)return i+1+11111*((i/100000)%10+1);
    if(i>=1000000)std::cout<<"HELP"<<std::endl;
    std::cout<<"OEI!"<<std::endl;return 0;
}

int find_next(int i) {// Find the first non-descending int >=i
    if (i>=10000000){std::cout<<"OEPSIE!"<<std::endl;return 0;}
    if ((i/1000000)%10 > (i/100000)%10)return ((i/1000000)%10)*111111+(i/1000000)*1000000;
    if ((i/100000)%10 > (i/10000)%10)return (i/100000)*100000+(i/100000)%10*11111;
    if ((i/10000)%10 > (i/1000)%10)return (i/10000)*10000+(i/10000)%10*1111;
    if ((i/1000)%10 > (i/100)%10)return (i/1000)*1000+(i/1000)%10*111;
    if ((i/100)%10 > (i/10)%10)return (i/100)*100+(i/100)%10*11;
    if ((i/10)%10 > (i/1)%10)return (i/10)*10+(i/10)%10*1;
    return i;
}

bool is_nice(int i) {
    for (int j=1;j<i*10;j*=10) {
        if ((i/j)%10 == (i/(j*10))%10) {//double digit found!
           j*=10; //walk up;
           if ((i/j)%10 != (i/(j*10))%10) {
              return true;
           }
           while(j<i*10 && (i/j)%10 == (i/(j*10))%10)j*=10;
        }
    }
    return false;
}

int main() {
    int lower,upper,unit=0;std::cin>>lower;if(std::cin.get()!='-')std::cout<<"OEPS"<<std::endl;std::cin>>upper;lower=find_next(lower);
    for(int i=lower;i<=upper;i=increase(i)) {
        if (is_nice(i))
            ++unit;
    }
    std::cout<<unit;
    return 0;
}
