#include <iostream>

long unsigned power(unsigned base, unsigned exp) {
    unsigned jets=0;
    unsigned pow=base,ret=1;
    while(exp>>jets!=0) {
        if ((exp>>jets)%2) {
            ret*=pow;
        }
        pow = pow*pow;
        jets++;
    }
    return ret;
}

int min(int a,int b){return (a<b ? a : b);}

int main() {
    const unsigned primes[7]={2,3,5,7,11,13,17};
    long unsigned minimum = -1;
    long unsigned current = power(2,21);
    long unsigned sum;
    unsigned short a=-1;std::cout<<a<<std::endl;
    for (int i=1;i<25;i++) {
        for(int j=0;j<=min(i,13);j++) {
            for(int k=0;k<=min(j,9);k++) {
                for(int l=0;l<=min(k,7);l++) {
                    for(int m=0;m<=min(l,6);m++) {
                        for(int n=0;n<=min(m,5);n++) {
                            for(int o=0;o<=min(n,5);o++) {
                                current = power(primes[0],i)*power(primes[1],j)*power(primes[2],k)*power(primes[3],l)*power(primes[4],m)*power(primes[5],n)*power(primes[6],o);
                                sum = (power(primes[0],i+1)-1)/(primes[0]-1) * (power(primes[1],j+1)-1)/(primes[1]-1) *(power(primes[2],k+1)-1)/(primes[2]-1) *(power(primes[3],l+1)-1)/(primes[3]-1) *(power(primes[4],m+1)-1)/(primes[4]-1) *(power(primes[5],n+1)-1)/(primes[5]-1) *(power(primes[6],o+1)-1)/(primes[6]-1);
                                if (current == 720720)std::cout<<sum<<'~'<<std::endl;
                                if (sum >= 34000000) {
                                    if(current < minimum) {
                                        std::cout<<(minimum=current)<<' '<<i<<','<<j<<','<<k<<','<<l<<','<<m<<','<<n<<','<<o<<' '<<sum<<std::endl;
                                    } else std::cout<<current<<'>'<<minimum<<std::endl;
                                } else std::cout<<current<<": "<<sum<<'<'<<34000000<<std::endl;
                                if (current > minimum) {
                                    current /=power(primes[6],o);
                                    break;
                                }
                            }
                            if (current > minimum) {
                                current /=power(primes[5],n);
                                break;
                            }
                        }
                        if (current > minimum) {
                            current /=power(primes[4],m);
                            break;
                        }
                    }
                    if (current > minimum) {
                        current /=power(primes[3],l);
                        break;
                    }
                }
                if (current > minimum) {
                    current /=power(primes[2],k);
                    break;
                }
            }
            if (current > minimum) {
                current /=power(primes[1],j);
                break;
            }
        }
        if (current > minimum) {
            current /=power(primes[0],i);
            break;
        }
   }
    return 0;
}
