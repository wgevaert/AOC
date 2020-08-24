#include <iostream>
#include <fstream>

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    unsigned short pad1[3][3]={{1,2,3},{4,5,6},{7,8,9}};
    char pad2[7][7] = {{'\0','\0','\0','\0','\0','\0','\0'},{'\0','\0','\0','1','\0','\0','\0'},{'\0','\0','2','3','4','\0','\0'},{'\0','5','6','7','8','9','\0'},{'\0','\0','A','B','C','\0','\0'},{'\0','\0','\0','D','\0','\0','\0'},{'\0','\0','\0','\0','\0','\0','\0'}};
    int x1=1,y1=1,x2=1,y2=3;

    for(;;){
        switch (input_file.get()){
            case 'L':
                if (x1>0)x1--;
                if(pad2[y2][x2-1])x2--;
                break;
            case 'D':
                if (y1<2)y1++;
                if(pad2[y2+1][x2])y2++;
                break;
            case 'U':
                if (y1>0)y1--;
                if(pad2[y2-1][x2])y2--;
                break;
            case 'R':
                if(x1<2)x1++;
                if(pad2[y2][x2+1])x2++;
                break;
            case '\n':
                if(argc>=3)std::cout<<pad2[y2][x2];else std::cout<<pad1[y1][x1];
                break;
            default:
                if(!input_file.eof())
                    std::cout<<"WEIRD...";
                std::cout<<std::endl;
                return 0;
        }
    }
    std::cout<<std::endl;
    return 0;
}
