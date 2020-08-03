#include <iostream>
#include <fstream>
#include <string>
#include <vector>

char hex(short a){
    if (a <=9)return static_cast<char>(a+'0');
    return static_cast<char>(a-10+'a');
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {name of input file}"<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }

    const short size = 256;
    short a=0,b=1,skip=0,pos=0;
    short knot[2][size];
    for(int i=0;i<size;i++)
        knot[0][i]=knot[1][i]=i;
    std::vector<short> lengths;
    char A;
    const short postfix[5]={17,31,73,47,23};

    while((A=input_file.get())!='\n') {
        lengths.push_back(static_cast<short>(A));
    }
    for (int i=0;i<5;i++)
        lengths.push_back(postfix[i]);

std::cout<<"GOT THE FOLLOWING CHARS:"<<std::endl;
for(auto i:lengths)std::cout<<i<<' ';
std::cout<<std::endl;

    for(int p=0;p<64;p++){
        for (auto length:lengths) {
            for(int i=0;i<length;i++) {
                knot[b][(pos+i)%size] = knot[a][(pos+length-i-1)%size];
            }
            for(int i=0;i<length;i++) {
                knot[a][(pos+i)%size] = knot[b][(pos+i)%size];
            }
            pos=(pos+length+(skip++))%size;
            a=!a;b=!b;
        }
        skip %= size;
    }
    char hash[size/8];
    for(int i=0;i<256;i+=16) {
        short xored=knot[b][i];
        for(int j=1;j<16;j++) {
            xored = xored ^ knot[b][i+j];
        }
        hash[i/8]=hex(xored/16);
        hash[i/8+1]=hex(xored%16);
        std::cout<<hash[i/8]<<hash[i/8+1];
    }

    std::cout<<std::endl;
    return 0;
}
