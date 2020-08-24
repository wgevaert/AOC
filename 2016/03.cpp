#include <iostream>
#include <fstream>

bool is_valid(unsigned x,unsigned y,unsigned z){
    return x+y>z&&x+z>y&&y+z>x;
}

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    unsigned valid_total[2]={0,0},a[3][3],i=0;

    for(;;){
        input_file>>a[0][i];
        input_file>>a[1][i];
        input_file>>a[2][i];
        if(input_file.eof())break;
        i = (i+1)%3;
        if(!(i%3))
            for(int j=0;j<3;j++) {
                if(is_valid(a[0][j],a[1][j],a[2][j]))
                    valid_total[0]++;
                if(is_valid(a[j][0],a[j][1],a[j][2]))
                    valid_total[1]++;
            }
    }
    std::cout<<valid_total[0]<<' '<<valid_total[1]<<std::endl;
    return 0;
}
