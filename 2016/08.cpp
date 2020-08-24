#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is not good"<<std::endl;return 1;}

    static bool screen[50][6], dummy_row[50], dummy_col[6];
    for(;;){
        if(input_file.get()!='r'){if(input_file.eof())break;std::cout<<"HELP!"<<std::endl;return 1;}
        unsigned A,B;
        switch(input_file.get()){
            case 'e':
                if(input_file.get()!='c'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"WOO!"<<std::endl;return 1;}
                input_file>>A;
                if(input_file.get()!='x'){std::cout<<"BOO!"<<std::endl;return 1;}
                input_file>>B;
                for(unsigned i=0;i<A;i++)for(unsigned j=0;j<B;j++)screen[i][j]=true;
                break;
            case 'o':
                if(input_file.get()!='t'||input_file.get()!='a'||input_file.get()!='t'||input_file.get()!='e'||input_file.get()!=' '){std::cout<<"Help!"<<std::endl;return 1;}
                switch (input_file.get()) {
                    case 'c':
                        if(input_file.get()!='o'||input_file.get()!='l'||input_file.get()!='u'||input_file.get()!='m'||input_file.get()!='n'||input_file.get()!=' '||input_file.get()!='x'||input_file.get()!='='){std::cout<<"Yelp!"<<std::endl;return 1;}
                        input_file>>A;
                        if(input_file.get()!=' '||input_file.get()!='b'||input_file.get()!='y'||input_file.get()!=' '){std::cout<<"NOES!"<<std::endl;return 1;}
                        input_file>>B;
                        for(int i=0;i<6;i++)dummy_col[i]=screen[A][i];
                        for(int i=0;i<6;i++)screen[A][(i+B)%6]=dummy_col[i];
                        break;
                    case 'r':
                        if(input_file.get()!='o'||input_file.get()!='w'||input_file.get()!=' '||input_file.get()!='y'||input_file.get()!='='){std::cout<<"Noes!"<<std::endl;return 1;}
                        input_file>>A;
                        if(input_file.get()!=' '||input_file.get()!='b'||input_file.get()!='y'||input_file.get()!=' '){std::cout<<"NOES!"<<std::endl;return 1;}
                        input_file>>B;
                        for(int i=0;i<50;i++)dummy_row[i]=screen[i][A];
                        for(int i=0;i<50;i++)screen[(i+B)%50][A]=dummy_row[i];
                        break;
                    default:std::cout<<"weird..."<<std::endl;return 1;
                }
                break;
           default:std::cout<<"Weird..."<<std::endl;return 1;
        }
        if(input_file.get()!='\n'){std::cout<<"YOU NEED TO EAT MORE!"<<std::endl;return 1;}
    }
    unsigned total=0;
    for(int j=0;j<6;j++){for(int i=0;i<50;i++)if(screen[i][j]){total++;std::cout<<'#';}else std::cout<<' ';std::cout<<std::endl;}std::cout<<total<<std::endl;
    return 0;
}
