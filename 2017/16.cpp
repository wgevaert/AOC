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

    unsigned short line[16];
    unsigned short letter[16];

    unsigned short line2[16];
    unsigned short letter2[16];

    for(int i=0;i<16;i++)
        line[i]=letter[i]=i;
    for(int i=0;i<16;i++)
        line2[i]=letter2[i]=i;

    do {
        switch(input_file.get()) {
            case 's':
                unsigned short a;
                input_file>>a;
                for(int i=0;i<16;i++)
                    letter[i]=(letter[i]+a)%16;
                for(int i=0;i<16;i++)
                    line[letter[i]]=i;
                break;
            case 'x':
                unsigned short b,c;
                input_file>>b;
                if(input_file.get()!='/'){std::cout<<"SOMETHING WENT WRONG"<<std::endl;return 1;}
                input_file>>c;
                a=line[b];line[b]=line[c];line[c]=a;
                for(int i=0;i<16;i++)
                    letter[line[i]]=i;
                break;
            case 'p':
                b=static_cast<unsigned short>(input_file.get()-'a');
                if(input_file.get()!='/'){std::cout<<"SOMETHING WENT WRONG"<<std::endl;return 1;}
                c=static_cast<unsigned short>(input_file.get()-'a');
                if(b>=16||c>=16){std::cout<<"THIS IS VERY WRONG"<<static_cast<char>(b+'a')<<static_cast<char>(c+'a')<<std::endl;return 1;}
                a=letter2[b];letter2[b]=letter2[c];letter2[c]=a;
                for(int i=0;i<16;i++)
                    line2[letter2[i]]=i;
                break;
        }
    } while(input_file.get() ==',');
    for(char i='a';i<='p';i++)
        std::cout<<i;
    std::cout<<std::endl<<"THE ANSWER TO THE FIRST PART IS ";
    for(int i=0;i<16;i++)
        std::cout<<static_cast<char>(line2[line[i]]+'a');

    static bool cycled[16];
    static bool cycled2[16];

    long unsigned order;
    long unsigned a_billion=1000000000;
    char the_line[16];
    for(int i=0;i<16;i++)
        the_line[i]=static_cast<char>(i+'a');

    for(int i=0;i<16;i++)if(!cycled2[i]){
        int j=i;
        order=0;
        do{
            j=line2[j];
            cycled2[j]=true;
            order++;
        } while(j!=i);
        for(unsigned j=0;j<a_billion%order;j++){
            int k=i;
            do{
                char a=the_line[k];
                the_line[k]=the_line[line2[k]];
                the_line[line2[k]]=a;
                k=line2[k];

            }while(line2[k]!=i);
        }
    }

    std::cout<<std::endl<<"NOW WE HAVE ";
    for(int i=0;i<16;i++)
        std::cout<<the_line[i];

    for(int i=0;i<16;i++)if(!cycled[i]){
        int j=i;
        order=0;
        do{
            j=line[j];
            cycled[j]=true;
            order++;
        } while(j!=i);
        for(unsigned j=0;j<a_billion%order;j++){
            int k=i;
            do{
                char a=the_line[k];
                the_line[k]=the_line[line[k]];the_line[line[k]]=a;
                k=line[k];
            }while(line[k]!=i);
        }

    }

    std::cout<<std::endl<<"THE ANSWER TO THE SECOND PART IS ";
    for(int i=0;i<16;i++)std::cout<<the_line[i];
        std::cout<<std::endl;

    input_file.close();
    return 0;
}
