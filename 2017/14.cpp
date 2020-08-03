#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int* hash(std::string input) {
    const short size = 256;
    const short postfix[5]={17,31,73,47,23};
    short a=0,b=1,skip=0,pos=0;
    short knot[2][size];
    for(int i=0;i<size;i++)
        knot[0][i]=knot[1][i]=i;
    std::vector<short> lengths;
    for (auto i:input)
        lengths.push_back(static_cast<short>(i));
    for (int i=0;i<5;i++)
        lengths.push_back(postfix[i]);

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

    int* rtr = new int[128];
    for(int i=0;i<256;i+=16) {
        short xored=knot[b][i];
        for(int j=1;j<16;j++) {
            xored = xored ^ knot[b][i+j];
        }
        for(int j=7;j>=0;j--){rtr[i/2+j]=(xored%2)==1;xored/=2;}
    }
    return rtr;
}

void flow(int a,int b, int** grid, int number) {
    if(grid[a][b] != number) {
        std::cout<<"SOMETHING WENT WRONG"<<std::endl;
    }
    if(a+1<128&&grid[a+1][b]==1){
        grid[a+1][b]=number;
        flow(a+1,b,grid,number);
    }
    if(a>0&&grid[a-1][b]==1){
        grid[a-1][b]=number;
        flow(a-1,b,grid,number);
    }
    if(b+1<128&&grid[a][b+1]==1){
        grid[a][b+1]=number;
        flow(a,b+1,grid,number);
    }
    if(b>0&&grid[a][b-1]==1){
        grid[a][b-1]=number;
        flow(a,b-1,grid,number);
    }
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

    std::string A="hxtvlmkl";
    int* grid[128];
    for (int i=0;i<128;i++)
        grid[i]=hash(A+"-"+std::to_string(i));

    int region=2;
    for (int i=0;i<128;i++)for(int j=0;j<128;j++)if(grid[i][j]==1){grid[i][j]=region;flow(i,j,grid,region++);}
    for(int i=0;i<128;i++){for(int j=0;j<44;j++)if(grid[i][j]==0)std::cout<<".... ";else {std::cout.width(4);std::cout<<grid[i][j]<<' ';}std::cout<<std::endl;}
    std::cout<<region-2<<std::endl;
    for(int i=0;i<128;i++)std::cout<<'.';std::cout<<std::endl;
    return 0;
}
