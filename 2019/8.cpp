#include <iostream>

int main() {
    const int width = 25, height = 6, size=width*height;
    int zeroes=size, min_zeroes=size, ones=0, twos=0, index=0;
    char a;
    while ((a=std::cin.get())<='9' && a>='0') {
        if (!(index%size)) {// In a new layer.
            if (min_zeroes > zeroes) {
                std::cout<< ones * twos<<'='<<ones<<'*'<<twos<<" at "<<zeroes<<" zeroes"<<std::endl;
                min_zeroes = zeroes;
            }
            zeroes = ones = twos = index = 0;
        }
        switch(a) {
            case '0':++zeroes;break;
            case '1':++ones;break;
            case '2':++twos;break;
        }
        ++index;
    }
    return 0;
}
