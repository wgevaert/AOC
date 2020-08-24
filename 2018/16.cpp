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

opcode opcodes[16]={addr,addi,mulr,muli,banr,bani,borr,bori,setr,seti,gtir,gtri,gtrr,eqir,eqri,eqrr};

struct instruction {
    long I,A,B,C;
    instruction(long _I,long _A, long _B, long _C) : I(_I),A(_A),B(_B),C(_C){}
};

struct sample {
    instruction inst;
    long initial[4],end[4];
    sample(long I,long A,long B,long C,long i0,long i1,long i2,long i3,long e0,long e1,long e2,long e3) : inst(I,A,B,C) {
        initial[0]=i0;initial[1]=i1;initial[2]=i2;initial[3]=i3;
        end[0]=e0;end[1]=e1;end[2]=e2;end[3]=e3;
    }
};

bool test(sample s, opcode op) {
    long regs[4];
    for(int i=0;i<4;i++)regs[i]=s.initial[i];
    op(regs,s.inst.A,s.inst.B,s.inst.C);
    for(int i=0;i<4;i++)if(regs[i]!=s.end[i])return false;
    return true;
}

int main(int argc, char** argv) {
    if(argc < 2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<sample> samples;
    std::vector<instruction> code;

    bool tally[16][16];
    for(int i=0;i<16;i++)for(int j=0;j<16;j++)tally[i][j]=true;

    for(;;) {
        if(input_file.get()=='\n')break;
        unsigned I,A,B,C,i0,i1,i2,i3,e0,e1,e2,e3;
        while(input_file.get()!='[');
        input_file>>i0;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>i1;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>i2;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>i3;
        if(input_file.get()!=']'||input_file.get()!='\n'){std::cout<<"]\\n  expected"<<std::endl;return 1;}
        input_file>>I;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>A;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>B;
        if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
        input_file>>C;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
        while(input_file.get()!='[');
        input_file>>e0;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>e1;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>e2;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        input_file>>e3;
        if(input_file.get()!=']'||input_file.get()!='\n'||input_file.get()!='\n'){std::cout<<"]\\n\\n  expected"<<std::endl;return 1;}
        samples.emplace_back(I,A,B,C,i0,i1,i2,i3,e0,e1,e2,e3);
    }

    for(;;) {
        unsigned I,A,B,C;
        while(input_file.get()=='\n');
        if(input_file.eof())break;
        input_file.unget();
        input_file>>I;
        if(input_file.get()!=' '){std::cout<<"  expected here"<<input_file.eof()<<std::endl;return 1;}
        input_file>>A;
        if(input_file.get()!=' '){std::cout<<"  expected in here"<<std::endl;return 1;}
        input_file>>B;
        if(input_file.get()!=' '){std::cout<<"  expected somewhere here"<<std::endl;return 1;}
        input_file>>C;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
        code.emplace_back(I,A,B,C);
    }

    unsigned answer1=0;
    for(auto a:samples){
        unsigned op_cnt=0;
        for(unsigned i=0;i<16;i++)
            if(test(a,opcodes[i]))
                op_cnt++;
            else
                tally[a.inst.I][i]=false;
        if(op_cnt >= 3)answer1++;
    }

    short dict[16];
    for(int i=0;i<16;i++)dict[i]=-1;
    unsigned det_cnt=0;
    while(det_cnt < 16) {
        for(unsigned i=0;i<16;i++)
            if(dict[i]==-1) {
                unsigned tally_cnt=0,nr;
                for(unsigned j=0;j<16;j++)if(tally[i][j]){tally_cnt++;nr=j;}
                if(tally_cnt==0)std::cout<<"SOMETHING WENT WRONG"<<std::endl;
                else if(tally_cnt==1) {
                    dict[i]=nr;
                    for(int j=0;j<16;j++)if(j!=i)tally[j][nr]=false;
                    det_cnt++;
                }
            }
        std::cout<<'+';for(int i=0;i<16;i++)std::cout<<'-';std::cout<<'+'<<std::endl;
        for(int i=0;i<16;i++){
            std::cout<<'|';
            for(int j=0;j<16;j++)std::cout<<(tally[j][i] ? 'X' : ' ');
            std::cout<<'|'<<std::endl;
        }
        std::cout<<'+';for(int i=0;i<16;i++)std::cout<<'-';std::cout<<'+'<<std::endl;
    }

    for(auto a:samples){
        unsigned op_cnt=0;
        for(unsigned i=0;i<16;i++)
            if(!test(a,opcodes[dict[a.inst.I]])){
                std::cout<<"dictionary is wrong"<<std::endl;
                return 1;
            }
        if(op_cnt >= 3)answer1++;
    }

    long registers[4]={0,0,0,0};
    for(auto a:code)opcodes[dict[a.I]](registers,a.A,a.B,a.C);

    for(int i=0;i<4;i++)std::cout<<registers[i]<<' ';std::cout<<std::endl<<answer1<<std::endl;

    return 0;
}
