#include <iostream>
#include <fstream>

bool is_valid(unsigned x,unsigned y,unsigned z){
    return x+y>z&&x+z>y&&y+z>x;
}

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    char a;
    unsigned id_total=0;
    for(;;){
        bool valid=true,found_goal=true;
        unsigned checksum_cnt[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        unsigned max=0,id;
        while((a=input_file.get())<'0'||a>'9'&&!input_file.eof()){
            if('a'<=a&&a<='z'){
                 checksum_cnt[a-'a']++;
                 if(checksum_cnt[a-'a']>max)max=checksum_cnt[a-'a'];
            } else if(a!='-'){if(!input_file.eof())std::cout<<" WHAT IS "<<a<<" DOING HERE? "<<std::endl;valid=false;break;}
        }
        if(input_file.eof())break;
        input_file.unget();
        input_file>>id;
        if(input_file.get()!='['){std::cout<<"weird..."<<std::endl;}
        int letter=0;
        for(int iter=0;iter<5;iter++){
            while(checksum_cnt[letter]!=max){
                if(letter<25)letter++;else{letter=0;max--;}
            }
            if(input_file.get()!=static_cast<char>(letter+'a')){
                valid=false;
                break;
            }
            if(letter<25)letter++;else{letter=0;max--;}
        }
        if(valid)id_total+=id;
        while((a=input_file.get())!='\n');
    }
    std::cout<<id_total<<std::endl;
    return 0;
}
