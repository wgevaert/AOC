#include <iostream>

long long supersum(int value, int depth) {
    long long result=0;
    if(depth==-1)return 1LL;
    for(int i=1;i<=value;i++) {
        result +=supersum(i,depth-1);
    }
    return result;
}

int main() {
    // Input is 178416-676461
    // returns 1716-84+35+6+5-28 = 1650
    std::cout<<supersum(8,5)-supersum(4,5) // All increasing sequences >= 2e+5, < 6e+5
               +supersum(4,3) // All increasing sequences >= 6.6e+5, < 6.7e+5
               +supersum(2,4) // All increasing sequences >= 1.8e+5, < 2e+5
               +supersum(2,3) // All increasing sequences >= 1.78e+5, <1.8e+5
               -supersum(3,5) // All strictly-increasing sequences in range
             <<std::endl;
    return 0;
}
