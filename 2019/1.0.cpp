#include <iostream>

int main() {
    long long unsigned total = 0;
    int current;
    std::cin>>current;
    while (current != 0) {
        while (current > 8) {
            std::cout<<"current: "<<current<<std::endl;
            total += (current = (current/3) -2);
        }
        std::cout<<" total :" << total<<std::endl;
        std::cin>>current;
    };
    std::cout<< total<<std::endl;
    
}
