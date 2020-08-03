#include <iostream>


int main() {
    unsigned long long sixteen_bits=65536,judge_count =0;;
    unsigned long long A=873,B=583,factor_A=16807,factor_B=48271,period=2147483647,max=-1;
    for (int i=0;i<5000000;i++) {
        if(A%sixteen_bits==B%sixteen_bits)judge_count++;
        do{(A = (A*factor_A)%period);}while(A%4);
        do{(B = (B*factor_B)%period);}while(B%8);
    }
    std::cout<<A%sixteen_bits<<' '<<B%sixteen_bits<<' '<<judge_count<<std::endl;
}
