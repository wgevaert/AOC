#include <iostream>
#include <fstream>
#include <string>

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
    short length;
    do {
        input_file >> length;
        for(int i=0;i<length;i++) {
            knot[b][(pos+i)%size] = knot[a][(pos+length-i-1)%size];
        }
for(int i=0;i<size;i++)if(i==pos)std::cout<<"(["<<knot[a][i]<<']'<<' ';else if(i==(length+pos-1)%size)std::cout<<knot[a][i]<<") ";else std::cout<<knot[a][i]<<' ';
std::cout<<std::endl;
        for(int i=0;i<length;i++) {
            knot[a][(pos+i)%size] = knot[b][(pos+i)%size];
        }
        pos=(pos+length+(skip++))%size;
for(int i=0;i<size;i++)if(i==pos)std::cout<<'['<<knot[b][i]<<']'<<' ';else std::cout<<knot[b][i]<<' ';
std::cout<<','<<skip<<'\n'<<std::endl;
        a=!a;b=!b;
    } while(input_file.get() ==',');
    std::cout<<std::endl<<knot[b][0]<<'*'<<knot[b][1]<<'='<<knot[b][0]*knot[b][1]<<std::endl;
    return 0;
}
