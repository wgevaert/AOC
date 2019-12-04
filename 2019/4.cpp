#include <iostream>

long long supersum(int value, int depth) {
    long long result=0;
    if(depth==-1)return 1LL;
    for(int i=1;i<=value;i++) {
        result +=supersum(i,depth-1);
    }
    return result;
}

int main() {//Unfortunately, does not work correctly...
    std::cout<<supersum(8,5)-supersum(4,5)+supersum(4,3)+supersum(2,3)+supersum(2,2)<<std::endl;return 0;
}
