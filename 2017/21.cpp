#include <iostream>
#include <fstream>
#include <string>

unsigned short get_score(unsigned short a,unsigned short b,unsigned short size){
    return 1<<(size*b+a);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Usage: {program} {name of input file} {number of iterations}"<<std::endl;
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

    static bool* enhance3[512];
    static bool* enhance2[16];
    bool** enhance[2]={enhance2,enhance3};

    unsigned size;
    unsigned short line=0;

    while(!input_file.eof()) {
        size=(line>=6?3:2);
        unsigned short score[8]={0,0,0,0,0,0,0,0};
        for(unsigned short b=0;b<size;b++){
            for(unsigned short a=0;a<size;a++){
                short val = (input_file.get()=='#'?1:0);
                if (input_file.eof())break;
                score[0]+=get_score(a,b,size)*val;
                score[1]+=get_score(a,size-b-1,size)*val;
                score[2]+=get_score(size-a-1,b,size)*val;
                score[3]+=get_score(size-a-1,size-b-1,size)*val;
                score[4]+=get_score(b,a,size)*val;
                score[5]+=get_score(b,size-a-1,size)*val;
                score[6]+=get_score(size-b-1,a,size)*val;
                score[7]+=get_score(size-b-1,size-a-1,size)*val;
            }
            if(input_file.eof())break;
            if (b!=size-1&&input_file.get()!='/'){std::cout<<"SOMETHING WRONG WITH THE SIZE"<<b<<"L"<<std::endl;return 1;}
        }if(input_file.eof())break;
        if(input_file.get()!=' '||input_file.get()!='='||input_file.get()!='>'||input_file.get()!=' '){
            std::cout<<"HELP!"<<std::endl;
        }

        bool* rule = new bool[(size+1)*(size+1)];
        for(unsigned short b=0;b<=size;b++){
            for(unsigned short a=0;a<=size;a++){
                    rule[b*(size+1)+a]=(input_file.get()=='#');
            }
            if (b!=size&&input_file.get()!='/'){std::cout<<"SOMETHING WRONG WITH THE SIZE"<<b<<std::endl;return 1;}
        }
        for(unsigned i=0;i<8;i++)
            enhance[size-2][score[i]]=rule;

        if(input_file.get()!='\n'){std::cout<<"HELP!"<<std::endl;return 1;}
        line++;
    }

    size=3;
    bool *old_art=new bool[size*size],*new_art;
    old_art[0]=old_art[2]=old_art[3]=old_art[4]=false;
    old_art[1]=old_art[5]=old_art[6]=old_art[7]=old_art[8]=true;
    unsigned white=0;
    for(int iter=0;iter<std::stoi(argv[2]);iter++){
        short unsigned divisor;
        if(!(size%2)){
            divisor=2;
        }else if(!(size%3)){
            divisor=3;
        } else{
            std::cout<<"SIZE "<<size<<" IS WEIRD"<<std::endl;return 1;
        }
        unsigned new_size = size*(divisor+1)/divisor;
        new_art = new bool[new_size*new_size];
        for(unsigned i=0;i<size/divisor;i++)for(unsigned j=0;j<size/divisor;j++){
            unsigned short score=0;
            for(unsigned k=0;k<divisor;k++)for(unsigned l=0;l<divisor;l++){
                if(old_art[(i*divisor+k)*size+j*divisor+l])score+=get_score(k,l,divisor);
            }
            for(unsigned k=0;k<=divisor;k++)for(unsigned l=0;l<=divisor;l++){
                    new_art[(i*(divisor+1)+k)*new_size+j*(divisor+1)+l]=enhance[divisor-2][score][k*(divisor+1)+l];
            }
        }
        delete[] old_art;
        old_art=new_art;
        size=new_size;
    }
    for(int i=0;i<size*size;i++)if(new_art[i])white++;
std::cout<<white<<std::endl;

    return 0;
}
