#include <iostream>
#include <fstream>
#include <string>
#include <vector>

unsigned long long get_exploded_length(std::ifstream& input_file,unsigned rep_length,unsigned long long repetitions, unsigned& pos){
    // assuming ({rep_length}x{repetitions} has been read
    unsigned long exploded_length=0;
    while(pos<rep_length) {
//std::cout<<'-'<<pos<<std::endl;
        char a = input_file.get();
        if(a<=' ')a=input_file.get();
        if(input_file.eof()){std::cout<<"FINISHED INSIDE LOOP"<<std::endl;exit(1);}
        if(a=='(') {
            unsigned new_rep_length=0,new_repetitions=0;
            pos++;
                a=input_file.get();
            while(a!='x'){if(a<'0'||'9'<a){std::cout<<"ILL_FORMED MARKER CONTAINS"<<a<<std::endl;exit(1);}
                new_rep_length=new_rep_length*10u+static_cast<unsigned>(a-'0');
                a=input_file.get();
                pos++;
            }
            pos++;
                a=input_file.get();
            while(a!=')'){if(a<'0'||'9'<a){std::cout<<"ILL_FORMED MARKER CONTAINS"<<a<<std::endl;exit(1);}
                new_repetitions=new_repetitions*10u+static_cast<unsigned>(a-'0');
                a=input_file.get();
                pos++;
            }
            unsigned new_pos=0;
//std::cout<<pos;
            exploded_length+=get_exploded_length(input_file, new_rep_length,new_repetitions,new_pos);
            pos+=new_pos+1;
        } else {
            pos++;
            exploded_length++;
        }
    }
std::cout<<"RETURNING "<<exploded_length<<'*'<<repetitions<<'='<<exploded_length*repetitions<<std::endl;
    return exploded_length*repetitions;
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is not good"<<std::endl;return 1;}

    unsigned rep_length,repetitions;
    unsigned long long length=0;
    std::vector<unsigned>lengths={0};
    for(;;){
        char a=input_file.get();
        if(a<=' ')a=input_file.get();
        if(input_file.eof())break;
        if(a=='('){
            // reading markers
            input_file>>rep_length;
            if(input_file.get()!='x'){std::cout<<"ILL_FORMED MARKER"<<std::endl;return 1;}
            input_file>>repetitions;
            if(input_file.get()!=')'){std::cout<<"ILL_FORMED MARKER"<<std::endl;return 1;}
            unsigned pos=0;
            length+=get_exploded_length(input_file, rep_length, repetitions, pos);
        } else {std::cout<<a<<std::endl;
            length++;
            if(rep_length)rep_length--;
        }
    }
    std::cout<<length<<std::endl;
    return 0;
}
