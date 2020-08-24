#include <iostream>
#include <fstream>
#include <vector>

void addr(long * registers, long A,long B,long C){registers[C]=registers[A]+registers[B];}
void addi(long * registers, long A,long B,long C){registers[C]=registers[A]+B;}
void mulr(long * registers, long A,long B,long C){registers[C]=registers[A]*registers[B];}
void muli(long * registers, long A,long B,long C){registers[C]=registers[A]*B;}
void banr(long * registers, long A,long B,long C){registers[C]=registers[A]&registers[B];}
void bani(long * registers, long A,long B,long C){registers[C]=registers[A]&B;}
void borr(long * registers, long A,long B,long C){registers[C]=registers[A]|registers[B];}
void bori(long * registers, long A,long B,long C){registers[C]=registers[A]|B;}
void setr(long * registers, long A,long B,long C){registers[C]=registers[A];}
void seti(long * registers, long A,long B,long C){registers[C]=A;}
void gtir(long * registers, long A,long B,long C){registers[C]=A>registers[B];}
void gtri(long * registers, long A,long B,long C){registers[C]=registers[A]>B;}
void gtrr(long * registers, long A,long B,long C){registers[C]=registers[A]>registers[B];}
void eqir(long * registers, long A,long B,long C){registers[C]=A==registers[B];}
void eqri(long * registers, long A,long B,long C){registers[C]=registers[A]==B;}
void eqrr(long * registers, long A,long B,long C){registers[C]=registers[A]==registers[B];}

typedef void (*opcode)(long*,long,long,long);

struct instruction {
    opcode op;
    long A,B,C;
    instruction(opcode _op,long _A, long _B, long _C) : op(_op),A(_A),B(_B),C(_C){}
    void execute(long * reg) {
        op(reg,A,B,C);
    }
};

int main(int argc, char** argv) {
    if(argc < 2){std::cout<<"Usage: {program} {input_file} [{part2?}] [{21 instead?}]"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<instruction> code;

    unsigned pointer_reg, ip=0;

    if(input_file.get()!='#'||input_file.get()!='i'||input_file.get()!='p'||input_file.get()!=' '){std::cout<<"Expected pointer binding"<<std::endl;return 1;}
    input_file>>pointer_reg;
    if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
    for(;;) {
        unsigned A,B,C;
        opcode op;
        switch(input_file.get()) {
            case 'a':if(input_file.get()!='d'||input_file.get()!='d'){std::cout<<"dd expected"<<std::endl;return 1;}
                switch(input_file.get()) {
                    case 'r':op=addr;break;
                    case 'i':op=addi;break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            case 'm':if(input_file.get()!='u'||input_file.get()!='l'){std::cout<<"ul expected"<<std::endl;return 1;}
                switch(input_file.get()) {
                    case 'r':op=mulr;break;
                    case 'i':op=muli;break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            case 'b':
                switch(input_file.get()) {
                    case 'a':
                        if(input_file.get()!='n'){std::cout<<"n expected"<<std::endl;return 1;}
                        switch(input_file.get()) {
                            case 'r':op=banr;break;
                            case 'i':op=bani;break;
                            default:std::cout<<"HELP!"<<std::endl;return 1;
                        }break;
                    case 'o':
                        if(input_file.get()!='r'){std::cout<<"r expected"<<std::endl;return 1;}
                        switch(input_file.get()) {
                            case 'r':op=borr;break;
                            case 'i':op=bori;break;
                            default:std::cout<<"HELP!"<<std::endl;return 1;
                        }break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            case 's':if(input_file.get()!='e'||input_file.get()!='t'){std::cout<<"et expected"<<std::endl;return 1;}
                switch(input_file.get()) {
                    case 'r':op=setr;break;
                    case 'i':op=seti;break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            case 'g':if(input_file.get()!='t'){std::cout<<"t expected"<<std::endl;return 1;}
                switch(input_file.get()) {
                    case 'r':
                        switch(input_file.get()) {
                            case 'r':op=gtrr;break;
                            case 'i':op=gtri;break;
                            default:std::cout<<"HELP!"<<std::endl;return 1;
                        }break;
                    case 'i':if(input_file.get()!='r'){std::cout<<"r expected"<<std::endl;return 1;}
                        op=gtir;break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            case 'e':if(input_file.get()!='q'){std::cout<<"q expected"<<std::endl;return 1;}
                switch(input_file.get()) {
                    case 'r':
                        switch(input_file.get()) {
                            case 'r':op=eqrr;break;
                            case 'i':op=eqri;break;
                            default:std::cout<<"HELP!"<<std::endl;return 1;
                        }break;
                    case 'i':if(input_file.get()!='r'){std::cout<<"r expected"<<std::endl;return 1;}
                        op=eqir;break;
                    default:std::cout<<"HELP!"<<std::endl;return 1;
                }break;
            default:if(input_file.eof())break;std::cout<<"Help!"<<std::endl;return 1;
        }
        if(input_file.eof())break;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>A;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>B;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>C;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
        code.emplace_back(op,A,B,C);
    }

    long registers[6]={(argc == 3 ? 1 : 0),0,0,0,0,0};
    std::vector<long long> units={};
    while(ip < code.size()) {
        registers[pointer_reg] = ip;
        code[ip].execute(registers);
        if (argc>=4 && ip==28){bool new_unit=true;for(auto a:units)if(registers[2]==a){new_unit=false;break;}if(new_unit){units.push_back(registers[2]);std::cout<<(code[ip].op==eqrr)<<' '<<registers[0]<<','<<registers[1]<<','<<registers[2]<<','<<registers[3]<<','<<registers[4]<<','<<registers[5]<<std::endl;}}
        if (argc<=3 && ip==1) {std::cout<<"searching for sum of divisors of "<<registers[2]<<std::endl;}//registers[0]<<','<<registers[1]<<','<<registers[2]<<','<<registers[3]<<','<<registers[4]<<','<<registers[5]<<std::endl;
        ip = registers[pointer_reg] + 1;
    }

    for(int i=0;i<4;i++)std::cout<<registers[i]<<' ';
    std::cout<<std::endl;

    return 0;
}
