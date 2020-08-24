#include <iostream>
#include <fstream>


int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    const unsigned max_word_length=10;
    unsigned word_length=0,letter_cnt[max_word_length][26];
for(int i=0;i<max_word_length;i++)for(int j=0;j<26;j++)letter_cnt[i][j]=0;
    char a;
    for(;;){
        unsigned letter=0;
        while((a=input_file.get())!='\n'&&!input_file.eof()){
            if(letter>=max_word_length){std::cout<<max_word_length<<" is too small "<<std::endl;return 1;}
            if ('a'<=a&&a<='z') {
                letter_cnt[letter++][a-'a']++;
            } else {if(!input_file.eof())std::cout<<" WHAT IS "<<a<<" DOING HERE? "<<std::endl;return 1;}
        }
        if(word_length < letter)word_length = letter;
        if(input_file.eof())break;
    }
    for(int i=0;i<word_length;i++){for(int j=0;j<26;j++)std::cout<<' '<<letter_cnt[i][j];std::cout<<std::endl;}
    for(int i=0;i<word_length;i++){
        unsigned max=0,arg_max,min=-1,arg_min;
        for(int j=0;j<26;j++)if(letter_cnt[i][j]>max){
            max=letter_cnt[i][j];arg_max=j;
        } else if(letter_cnt[i][j]<min){
            min=letter_cnt[i][j];arg_min=j;
        }
        std::cout<<static_cast<char>(arg_min+'a');
    }
    std::cout<<std::endl;
    return 0;
}
