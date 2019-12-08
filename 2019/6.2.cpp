#include <iostream>

int main() {
    const int base = 46, unit=base*base*base;
    int orbits[unit]; // gives the index of the parent node
    for (int i=0;i<unit;i++)orbits[i] = unit;
    char a='$';
    int index=0,value=0,answer=0, you=73078, san = 59731;
    while(a!='&') {
        while ((a=std::cin.get())!=')') {
            if ('0'<=a&&'9'>=a) {
                index = index*base + a-'0';
            } else if('A'<=a&&a<='Z') {
                index = index*base + a-'A'+10;
            } else {
                std::cout<<"Unrecognised char: "<<a<<" with value "<< static_cast<int>(a) <<" at first"<<std::endl;
            }
        }
        if (index > unit) std::cout<<"HELP!"<<index<<std::endl;
        while ((a=std::cin.get())!='\n' && a != '&') {std::cout<<a;
            if ('0'<=a&&'9'>=a) {
                value = value*base + a-'0';
            } else if('A'<=a&&a<='Z') {
                value = value*base + a-'A'+10;
            } else {
                std::cout<<"Unrecognised char: "<< a <<" with value "<< static_cast<int>(a) <<" at second"<<std::endl;
            }
        }
        if (value >= unit) std::cout<<"HELP!!"<<index<<std::endl;
        orbits[value]=index;
        if (index == value)std::cout<<"OEI!"<<index<<' '<<value<<"OEI!"<<std::endl;
        index = value = 0;answer++;
    }
    for (int i=0;i<unit;i++)
        if(orbits[i]!=unit)
            std::cout<<i<<'-'<<orbits[i]<<std::endl;

    //walk from you
    if (orbits[you]==unit) std::cout<<"SOMETHING WRONG HERE"<<std::endl;
    int jumps=0;
    while (orbits[you]!=unit) {
        int tmp = you;
        you = orbits[you];
        orbits[tmp]=jumps+unit;
        ++jumps;
    }

    //walk from san
    if (orbits[san]==unit) std::cout<<"SOMETHING WRONG THERE"<<std::endl;
    jumps = 0;
    while (orbits[san]<unit) {
        san = orbits[san];
        ++jumps;
    }
    std::cout<<"ANSWER: "<<orbits[san]-unit+jumps-2<<std::endl;
    return 0;
}
