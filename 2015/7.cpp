#include <iostream>
#include <string>

struct instruction {
    unsigned short (*operation)(instruction**, int);
    instruction **inputs;
    unsigned short value = 666;
    bool initialised=false;
    int size;
    instruction() {
        inputs = NULL;
    }
    instruction(unsigned short val) {
        inputs = NULL;
        value=val;
        initialised=true;
        size = 0;
    }
    instruction(instruction** ipts, unsigned short (*oprtn)(instruction**, int)) {
        inputs = ipts;
        operation = oprtn;
    }
    ~instruction() {
        if(inputs!=NULL)delete[] inputs;
    }
};

unsigned short my_and(instruction** values, int size) {
   if (size != 2){std::cerr<<"size "<<size<<" should be 2 for &"<<std::endl;exit(1);}
   return values[0]->value & values[1]->value;
}

unsigned short my_or(instruction** values, int size) {
   if (size != 2){std::cerr<<"size "<<size<<" should be 2 for |"<<std::endl;exit(1);}
   return values[0]->value | values[1]->value;
}

unsigned short my_lshift(instruction** values, int size) {
   if (size != 2){std::cerr<<"size "<<size<<" should be 2 for <<"<<std::endl;exit(1);}
   return values[0]->value << values[1]->value;
}

unsigned short my_rshift(instruction** values, int size) {
   if (size != 2){std::cerr<<"size "<<size<<" should be 2 for >>"<<std::endl;exit(1);}
   return (values[0]->value) >> (values[1]->value);
}

unsigned short my_not(instruction** values, int size) {
   if (size != 1){std::cerr<<"size "<<size<<" should be 1 for !"<<std::endl;exit(1);}
   return ~values[0]->value;
}

unsigned short my_assign(instruction** values, int size) {
   if (size != 1){std::cerr<<"size "<<size<<" should be 1 for ="<<std::endl;exit(1);}
   return values[0]->value;
}

void cout_name(unsigned short (*operation)(instruction**, int)) {
 if (operation == &my_and) {
        std::cout<<"AND";
    } else if (operation == &my_or) {
        std::cout<<"OR";
    } else if (operation == &my_rshift) {
        std::cout<<"RSHIFT";
    } else if (operation == &my_lshift) {
        std::cout<<"LSHIFT";
    } else if (operation == &my_not) {
        std::cout<<"NOT";
    } else if (operation == &my_assign) {
        std::cout<<"=";
    } else {
        std::cerr<<"Unknown operation "<<(void*)operation<<" not in "<<((void*)&my_and)<<' '<<((void*)&my_or)<<' '<<((void*)&my_rshift)<<' '<<((void*)&my_lshift)<<' '<<((void*)&my_not)<<std::endl;exit(1);
    }
}

bool my_do(instruction &in, const instruction circuit_addr[]) {
    for (int i=0;i< in.size;i++)if(!in.inputs[i]->initialised){std::cerr<<"Input "<<i<<" not initialised"<<std::endl;return false;}
    std::cout<<(&in - circuit_addr)<<"<-"<< '('<< in.inputs[0]->value<<' ';cout_name(in.operation);if(in.inputs[1])std::cout<<' '<< in.inputs[1]->value;else std::cout<<" NULL";
    std::cout<<") = *"<<in.inputs[0] - circuit_addr<<' ';cout_name(in.operation);if(in.inputs[1])std::cout<<" *"<< in.inputs[1]-circuit_addr;else std::cout<<" NULL";
    in.value = in.operation(in.inputs,in.size);
    in.initialised = true;
    return true;
}

bool readline(instruction* circuit, instruction *constants[], int &cnts_nr, bool initialised[]) {
    char a;
    instruction* inputs[2]={NULL,NULL};
    unsigned short (*operation)(instruction**, int)=NULL;
    unsigned short dummy;
    int index =0;
    bool reading = true;
    a = std::cin.get();
std::cout<<"READ (";
    while(reading) {
        if (a=='\n' || a==' ')a=std::cin.get();//gobble whitespace
        if (std::cin.eof()){
std::cout<<')'<<std::endl;
            return false;
        }//handle eof
        if ('a'<=a&&a<='z') {//input is lowecase-string adress
            dummy=0;
            while (a!=' '){
                if ('a'>a||a>'z'){std::cerr<<"Found non-[a-z]-char "<<a<<'='<< static_cast<int>(a)<<" at "<<index<<std::endl;exit(1);}
                dummy = 27*dummy + a - 'a'+1;
                a=std::cin.get();
            }
std::cout<<index<<"adr-"<<dummy<<' ';
            inputs[index] = &(circuit[dummy]);
            ++ index;
        } else if ('0'<=a && a<='9') {//input is int-value
            dummy =0;
            while (a!=' ') {
                 if ('0'>a||a>'9'){std::cerr<<"Found non-[0-9]-char "<<a<<'='<< static_cast<int>(a)<<" at "<<index<<std::endl;exit(1);}
                 dummy = 10*dummy + a - '0';
                 a=std::cin.get();
            }
std::cout<<"const-"<<dummy<<'#'<<cnts_nr<<' ';
            inputs[index] = constants[cnts_nr] = new instruction(dummy);
            ++index;
            ++cnts_nr;
        } else if (a=='N')/*NOT*/
            std::cin.get();
        else {std::cerr<<"UNEXPECTED FIRST CHAR: "<<a<<'='<< static_cast<int>(a)<<std::endl;exit(1);}
        a=std::cin.get();
        switch (a) {
            case 'A': /*AND*/    operation = &my_and;break;
            case 'O':  /*OR*/    operation = &my_or;break;
            case 'R':/*RSHIFT*/  operation = &my_rshift;break;
            case 'L':/*LSHIFT*/  operation = &my_lshift;break;
            case 'T': /*noT*/    operation = &my_not;break;
            case '-':  /*->*/    reading = false;break;
            default : std::cerr<<"UNEXPECTED CHAR: "<<a<<'='<< static_cast<int>(a)<<std::endl;exit(1);
        }
        while ((a=std::cin.get())!= ' ');// gobble rest of comment
    }
    --index;
    // read output number
    if (a==' ')a=std::cin.get();
    dummy = 0;
    while(a!='\n'){
        if ('a'>a||a>'z'){std::cerr<<"Found non-[a-z]-char "<<a<<'='<< static_cast<int>(a)<<" at output."<<std::endl;exit(1);}
        dummy = 27*dummy + a - 'a'+1;
        a=std::cin.get();
    }
    if (operation == NULL) {
        operation = &my_assign;
    }if (!initialised[dummy])initialised[dummy]=true;else{std::cout<<"OVERWRITING "<<dummy<<std::endl;}
    circuit[dummy].size = index + 1;// arrays are 0-based;
    circuit[dummy].inputs = new instruction*[index];
    for(int i=0;i<=index;i++) {
        circuit[dummy].inputs[i]=inputs[i];
    }
    circuit[dummy].operation=operation;
std::cout<<')';
cout_name(operation);
std::cout<<" at "<<dummy<<std::endl;
    return true;
}

bool initialise (instruction* ptr, instruction circuit_addr[]) {
    if (ptr == NULL){std::cerr<<"FOUND NULL POINTER! stack-trace:"<<std::endl;return false;}
    if (!ptr->initialised) {
/*std::cout<<"determining "<<ptr - circuit_addr<<" of size "<< ptr->size<<' ';
        for (int i=0;i< ptr->size;i++){
            if (ptr->inputs[i]->initialised) {std::cout<<ptr->inputs[i]->value<<' ';} else std::cout<<'#'<<ptr->inputs[i] - circuit_addr<<' ';
        }if(ptr->size){cout_name(ptr->operation);}std::cout<<std::endl;*/
        for (int i=ptr->size-1;i>=0;--i){
            //bool was_init = ptr->inputs[i]->initialised;
            if (!initialise(ptr->inputs[i], circuit_addr)){ //Enter recursion, produce a stacktrace on failure.
                std::cout<<"OEI "<<ptr<< ' ';cout_name(ptr->operation);std::cout<<std::endl;
                return false;
            }/*
            if (!was_init) {
                std::cout<< ptr - circuit_addr << ' '<< ptr->inputs[i]->size<< ' '<<ptr->inputs[i]->value<< '#' ;cout_name(ptr->inputs[i]->operation);for (int j=0;j< ptr->inputs[i]->size;j++)std::cout<<ptr->inputs[i]->inputs[j]->value<<' ';std::cout<<std::endl;
            }*/
            if (ptr->inputs[i]->size >= 1){
                 if(ptr->inputs[i]->inputs == NULL){
                    std::cerr<<"THIS IS REALLY WEIRD"<<std::endl;
                    return false;
                 }
            }
        }
        my_do(*ptr, circuit_addr); //Initialise pointer using initialised values.
        std::cout<<" = "<<ptr->value<<std::endl;
    }

    if (!ptr->initialised){
        std::cerr<<"HIER GAAT IETS FOUT!!!"<<std::endl;
    }
    return true;
}

int main() {
    static instruction circuit[27*27];//static is init as 0;
    static instruction *constants[27*27*2];
    static bool initialised[27*27];
    int numberofconstants=0;
    while(readline(circuit, constants, numberofconstants, initialised));// read inputs.

    initialise(&(circuit[1]), circuit);
    std::cout<<"ANSWER: "<< circuit[1].value<<std::endl;

    // destructor
    std::cout<<"Found constants:";
    for(int i=0;i<numberofconstants;i++){
        std::cout<<constants[i]->value<<' ';
        delete constants[i];
    }std::cout<<std::endl;
    return 0;
}
