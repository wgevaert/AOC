#include <iostream>

unsigned long long gcd(unsigned long long a, unsigned long long b) {
    if (b==0)return a;
    return gcd(b,a%b);
}

unsigned long long get_lcm(unsigned long long a,unsigned long long b) {
    return a/gcd(a,b)*b;
}

int main(int argc, char* argv[]) {
    if (argc<3) {
        std::cout<<"Cannot calculate lcm of single value"<<std::endl;return 1;
    }
    long long unsigned lcm = std::stoull(argv[1]);
    for (int i=2;i<argc;i++)lcm = get_lcm(std::stoull(argv[i]),lcm);
    std::cout<<lcm<<std::endl;
    return 0;
}
