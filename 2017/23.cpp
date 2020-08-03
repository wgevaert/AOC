#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

unsigned get_X(std::ifstream & input_file) {
    if(input_file.get()!=' '){std::cout<<"OH NOoES!"<<std::endl;exit(1);}
    return static_cast<unsigned>(input_file.get()-'a');
}

long get_Y(std::ifstream & input_file,bool&relative) {
    if(input_file.get()!=' '){std::cout<<"OH NOoS!"<<std::endl;exit(1);}
    char a=input_file.get(),A;
    long b=0,sign=1;
    if (a>='a'&&a<='z'){
        if((A=input_file.get())!='\n'){input_file.unget();if(A!=' '){std::cout<<"OH NO!"<<std::endl;exit(1);}}
        relative=true;
        return static_cast<long>(a-'a');
    }
    if (a=='-'){sign=-1;a=input_file.get();}
    while(a!='\n'&&a!=' '){
        if('0'>a||a>'9'){std::cout<<"OH NO!"<<a<<std::endl;exit(1);}
        b = b*10+a-'0';
        a=input_file.get();
    }
    if(a==' ')input_file.unget();
    relative=false;
    return sign*b;
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

    static long registers['h'-'a'];
    long Y,X_l;
    unsigned X;
    bool relative, X_rel;

    std::vector<std::function<bool(long*,unsigned&)>> operations;
    while(!input_file.eof()) {
        switch(input_file.get()) {
            case 's':
                switch(input_file.get()) {
                    case 'e':
                        if(input_file.get()!='t'){
                            std::cout<<"OEPSie"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y = get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](long* registers,unsigned&pos){registers[X]=(relative?registers[Y]:Y);pos++;return false;});
                        break;
                    case 'u':
                        if(input_file.get()!='b'){
                            std::cout<<"OEPSie"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y = get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](long* registers,unsigned&pos){registers[X]-=(relative?registers[Y]:Y);pos++;return false;});
                        break;
                    default:std::cout<<"HELP ME!"<<std::endl;return 1;
                }
                break;
            case 'm':
                if(input_file.get()!='u'||input_file.get()!='l'){
                    std::cout<<"OOPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                Y =get_Y(input_file,relative);
                operations.push_back([X,Y,relative](long* registers,unsigned&pos){static unsigned invocations=0;std::cout<<++invocations<<std::endl;registers[X]*=(relative?registers[Y]:Y);pos++;return false;});
                break;
            case 'j':
                if(input_file.get()!='n'||input_file.get()!='z'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X_l = get_Y(input_file,X_rel);
                Y = get_Y(input_file,relative);
                operations.push_back([X_l,X_rel,Y,relative](long* registers,unsigned&pos){if((X_rel?registers[X_l]:X_l)!=0){pos+=(relative?registers[Y]:Y);}else pos++;return false;});
                break;
            default:
                if(!input_file.eof()){std::cout<<"HELP!"<<std::endl;return 1;}
        }
    }
    unsigned pos=0;
    while(pos < operations.size()&& !operations[pos](registers,pos)){}

    return 0;
}
