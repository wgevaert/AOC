#include <iostream>
#include <vector>
#include <functional>
#include <fstream>

typedef std::function<int(unsigned long* registers)> operation;

std::vector<operation>& read_operations(std::ifstream& input_file){
    std::vector<operation>* rtr = new std::vector<operation>;
    unsigned unit;
    int offset;
    for(;;){switch(input_file.get()){
        case 'h':
            if(input_file.get()!='l'||input_file.get()!='f'||input_file.get()!=' '){std::cout<<"Weird..."<<std::endl;exit(1);}
            unit = input_file.get()-'a';if(unit>1){std::cout<<"BAD!"<<std::endl;exit(1);}
            rtr->push_back([unit](unsigned long*reg){if(reg[unit]%2)std::cout<<"DOING UGLY THINGS"<<std::endl;reg[unit]/=2;return 1;});
            break;
        case 't':
            if(input_file.get()!='p'||input_file.get()!='l'||input_file.get()!=' '){std::cout<<"WeIrd..."<<std::endl;exit(1);}
            unit = input_file.get()-'a';if(unit>1){std::cout<<"BAd!"<<std::endl;exit(1);}
            rtr->push_back([unit](unsigned long*reg){if(reg[unit] > static_cast<unsigned long>(-1)/3)std::cout<<"DOING BAD THINGS"<<std::endl;reg[unit]*=3;return 1;});
            break;
        case 'i':
            if(input_file.get()!='n'||input_file.get()!='c'||input_file.get()!=' '){std::cout<<"WeirD..."<<std::endl;exit(1);}
            unit = input_file.get()-'a';if(unit>1){std::cout<<"BaD!"<<std::endl;exit(1);}
            rtr->push_back([unit](unsigned long*reg){reg[unit]++;return 1;});
            break;
        case 'j':
            switch(input_file.get()){
                case 'm':
                    if(input_file.get()!='p'||input_file.get()!=' '){std::cout<<"WeIrD..."<<std::endl;exit(1);}
                    input_file>>offset;if(offset==0){std::cout<<"bAD!"<<std::endl;exit(1);}
                    rtr->push_back([offset](unsigned long*reg){return offset;});
                    break;
                case 'i':
                    switch(input_file.get()){
                        case 'e':
                            if(input_file.get()!=' '){std::cout<<"weIrD..."<<std::endl;exit(1);}
                            unit = input_file.get()-'a';if(unit>1){std::cout<<"baD!"<<std::endl;exit(1);}
                            if(input_file.get()!=','||input_file.get()!=' '){std::cout<<"weIRD..."<<std::endl;exit(1);}
                            input_file>>offset;if(offset==0){std::cout<<"bAd!"<<std::endl;exit(1);}
                            rtr->push_back([unit,offset](unsigned long*reg){if(!(reg[unit]%2))return offset;return 1;});
                            break;
                        case 'o':
                            if(input_file.get()!=' '){std::cout<<"wEIrD..."<<std::endl;exit(1);}
                            unit = input_file.get()-'a';if(unit>1){std::cout<<"Bad!"<<std::endl;exit(1);}
                            if(input_file.get()!=','||input_file.get()!=' '){std::cout<<"wEiRD..."<<std::endl;exit(1);}
                            input_file>>offset;if(offset==0){std::cout<<"bad!"<<std::endl;exit(1);}
                            rtr->push_back([unit,offset](unsigned long*reg){if(reg[unit]==1)return offset;return 1;});
                            break;
                        default:
                            std::cout<<"WeiRD"<<std::endl;exit(1);
                    }
                    break;
                default:std::cout<<"WeIRd"<<std::endl;exit(1);
            }
            break;
        default:
            if(input_file.eof())return *rtr;
            std::cout<<"WEird..."<<std::endl;exit(1);
    }if(input_file.get()!='\n'){std::cout<<"BAD?"<<std::endl;exit(1);}}
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"Usage: {program} {input_file} [{a-value}]"<<std::endl;return 0;}
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}

    std::vector<operation> operations=read_operations(input_file);

    unsigned position =0;
    unsigned long registers[2]={(argc>=3&&argv[2][0]=='1'?1ul:0ul),0};
    while(position<operations.size()){position+=operations[position](registers);}
    std::cout<<registers[1]<<std::endl;
    return 0;
}
