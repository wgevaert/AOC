#include <iostream>
#include <vector>
#include <fstream>

struct operation {
    unsigned short type;
    short args[2];
    int numberofargs;
    bool arg_is_relative[2];
    long long get_arg(unsigned short arg, long long* registers){
        if(arg >= numberofargs){std::cerr<<"ERROR!"<<std::endl;exit(1);}
        if(arg_is_relative[arg])return registers[args[arg]];
        return args[arg];
    }
    void set_arg(unsigned short arg, long long* registers, long long value) {
        if(arg >= numberofargs){std::cerr<<"ERROR!!"<<std::endl;exit(1);}
        if(!arg_is_relative[arg]){std::cout<<"SKIPPED"<<std::endl;}
        else
            registers[args[arg]] = value;
    }
};

#define cpy 0
#define inc 1
#define dec 2
#define jnz 3
#define out 4

unsigned long invoke (std::vector<operation>& operations, size_t& pos, long long* registers) {
    unsigned long rtr=-1;
    switch (operations[pos].type) {
        case cpy:
            operations[pos].set_arg(1,registers,operations[pos].get_arg(0,registers));
            pos++;
            break;
        case inc:
            operations[pos].set_arg(0,registers,operations[pos].get_arg(0,registers)+1);
            pos++;
            break;
        case dec:
            operations[pos].set_arg(0,registers,operations[pos].get_arg(0,registers)-1);
            pos++;
            break;
        case jnz:
            if(operations[pos].get_arg(0,registers)==0)
                pos++;
            else
                pos+= operations[pos].get_arg(1,registers);
            break;
        case out:
            rtr=operations[pos].get_arg(0,registers);
            pos++;
            break;
        default: std::cout<<"UNKNOWN TYPE "<<operations[pos].type<<std::endl;
    }
    return rtr;
}

operation read_args(std::ifstream& input_file,short unsigned numberofargs){
    operation rtr;
    rtr.numberofargs=numberofargs;
    for(short unsigned i=0;i<numberofargs;i++){
        char a = input_file.get();
        if (a=='-'||('0'<=a&&a<='9')){input_file.unget();short arg;input_file>>arg;rtr.args[i]=arg;rtr.arg_is_relative[i]=false;}
        else if('a'<=a&&a<='d'){rtr.arg_is_relative[i]=true;rtr.args[i]=a-'a';}
        input_file.get();//gobble ' ' or '\n';
    }
    return rtr;
}

std::vector<operation> read_input(char* input_name) {
    std::vector<operation> rtr={};
    std::ifstream input_file(input_name);
    if(!input_file.good()) {
        std::cerr<<"INPUT FILE "<<input_name<<" IS NOT GOOD"<<std::endl;exit(1);
    }
    while(!input_file.eof()) {
    switch(input_file.get()){
        case 'c':
            if(input_file.get()!='p'||input_file.get()!='y'||input_file.get()!=' '){std::cerr<<"HELP :("<<std::endl;exit(1);}
            rtr.push_back(read_args(input_file,2));
            rtr.back().type = cpy;
            break;
        case 'i':
            if(input_file.get()!='n'||input_file.get()!='c'||input_file.get()!=' '){std::cerr<<"HELP :O"<<std::endl;exit(1);}
            rtr.push_back(read_args(input_file,1));
            rtr.back().type = inc;
            break;
        case 'd':
            if(input_file.get()!='e'||input_file.get()!='c'||input_file.get()!=' '){std::cerr<<"HELP :P"<<std::endl;exit(1);}
            rtr.push_back(read_args(input_file,1));
            rtr.back().type = dec;
            break;
        case 'j':
            if(input_file.get()!='n'||input_file.get()!='z'||input_file.get()!=' '){std::cerr<<"HELP :@"<<std::endl;exit(1);}
            rtr.push_back(read_args(input_file,2));
            rtr.back().type = jnz;
            break;
        case 'o':
            if(input_file.get()!='u'||input_file.get()!='t'||input_file.get()!=' '){std::cerr<<"HELP :#"<<std::endl;exit(1);}
            rtr.push_back(read_args(input_file,1));
            rtr.back().type = out;
            break;
        case '\n':break;
        default:if(input_file.eof())break;std::cerr<<"HELP :&"<<std::endl;exit(1);
    }
    }
    return rtr;
}

bool attempt(long long unit, std::vector<operation>& operations){
    long long registers[4]={unit,0,0,0};
    size_t pos=0;
    unsigned long a,prev=1,max=-1;
    while(pos<operations.size()){a=invoke(operations,pos,registers);if(a!=max){std::cout<<a<<' '<<std::flush;if(a==prev)return false;prev=a;}}
    std::cout<<"weird"<<std::endl;
    return false;
}

int main(int argc, char** argv) {
    if(argc < 2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    std::vector<operation> operations = read_input(argv[1]);
    for(long long unit=0;unit<2048;unit++){std::cout<<std::endl<<"attempting "<<unit<<':'<<std::endl;attempt(unit,operations);}
    return 0;
}
