#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    if(argc<2){std::cout<<" usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    const unsigned long long last=4294967295;
    unsigned long long min=0,input1,input2,allowed_cnt=0,max,cnt=0;
    input_file>>input1;
    do{
        do{
            if(input_file.get()!='-'){std::cout<<"HELP"<<std::endl;return 1;}
            input_file>>input2;
            if(input1 <= min && input2>=min){min=input2+1;input_file.seekg(0);}
            else if(input_file.get()!='\n'){std::cout<<"HELP!"<<std::endl;return 1;}
            input_file>>input1;
        }while(!input_file.eof());
if(!cnt++)std::cout<<min<<std::endl;
        input_file.clear();
        input_file.seekg(0);
        input_file>>input1;
        max = last;
        do{
            if(input_file.get()!='-'){std::cout<<"HELP!!!"<<std::endl;return 1;}
            input_file>>input2;
            if(min <= input1 && max >=input1){max=input1-1;}
            else if(input_file.get()!='\n'){std::cout<<"HELP!!"<<std::endl;return 1;}
            input_file>>input1;
        }while(!input_file.eof());
        allowed_cnt += max-min+1;
        min=max+1;
        input_file.clear();
        input_file.seekg(0);
        input_file>>input1;
    }while(max<last);
    input_file.close();
std::cout<<allowed_cnt<<std::endl;
    return 0;
}
