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
    char a=input_file.get();
    long b=0,sign=1;
    if (a>='a'&&a<='z'){
        if(input_file.get()!='\n'){std::cout<<"OH NO!"<<std::endl;exit(1);}
        relative=true;
        std::cout<<"y="<<a<<std::endl;
        return static_cast<long>(a-'a');
    }
    if (a=='-'){sign=-1;a=input_file.get();}
    while(a!='\n'){
        if('0'>a||a>'9'){std::cout<<"OH NO!"<<a<<std::endl;exit(1);}
        b = b*10+a-'0';
        a=input_file.get();
    }
std::cout<<"Y="<<sign*b<<std::endl;
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

    static long registers[26];
    long Y;
    unsigned X;
    bool relative;

    std::vector<std::function<bool(long*,long&,unsigned&)>> operations;
    while(!input_file.eof()) {
        switch(input_file.get()) {
            case 's':
                switch(input_file.get()) {
                    case 'n':
                        if(input_file.get()!='d'){
                            std::cout<<"OOPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        if(input_file.get()!='\n'){std::cout<<"Missing newline..."<<std::endl;return 1;}
                        operations.push_back([X](long* registers,long &prev_sound,unsigned&pos){prev_sound=registers[X];pos++;return false;});
                        break;
                    case 'e':
                        if(input_file.get()!='t'){
                            std::cout<<"OEPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y =get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](long* registers,long &prev_sound, unsigned&pos){registers[X]=(relative?registers[Y]:Y);pos++;return false;});
                        break;
                }
                break;
            case 'a':
                if(input_file.get()!='d'||input_file.get()!='d'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                Y = get_Y(input_file,relative);
                operations.push_back([X,Y,relative](long* registers,long &prev_sound, unsigned&pos){registers[X]+=(relative?registers[Y]:Y);pos++;return false;});
                break;
            case 'm':
                switch(input_file.get()) {
                    case 'u':
                        if(input_file.get()!='l'){
                            std::cout<<"OOPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y =get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](long* registers,long &prev_sound, unsigned&pos){registers[X]*=(relative?registers[Y]:Y);pos++;return false;});
                        break;
                    case 'o':
                        if(input_file.get()!='d'){
                            std::cout<<"OEPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y =get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](long* registers,long &prev_sound, unsigned&pos){registers[X]%=(relative?registers[Y]:Y);pos++;return false;});
                        break;
                }
                break;
            case 'r':
                if(input_file.get()!='c'||input_file.get()!='v'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                if(input_file.get()!='\n'){std::cout<<"Missing newline..."<<std::endl;return 1;}
                operations.push_back([X](long* registers,long &prev_sound, unsigned&pos){if(!registers[X]){pos++;return false;}std::cout<<prev_sound<<std::endl;pos++;return true;});
                break;
            case 'j':
                if(input_file.get()!='g'||input_file.get()!='z'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                Y = get_Y(input_file,relative);
                operations.push_back([X,Y,relative](long* registers,long &prev_sound, unsigned&pos){if(registers[X]>0){std::cout<<"JUMPING"<<std::endl;pos+=(relative?registers[Y]:Y);}else pos++;return false;});
                break;
            default:
                if(!input_file.eof()){std::cout<<"HELP!"<<std::endl;return 1;}
        }
    }
    unsigned pos=0;
    long prev_sound;
for(int i=0;i<26;i++)std::cout<<static_cast<char>(i+'a')<<' ';std::cout<<std::endl;
    while(pos < operations.size()&& !operations[pos](registers,prev_sound,pos)){for(int i=0;i<26;i++)std::cout<<(i==pos?"(":"")<<registers[i]<<(i==pos?")":"")<<' ';std::cout<<std::endl;}
    std::cout<<pos<<' '<<prev_sound<<std::endl;

    return 0;
}
