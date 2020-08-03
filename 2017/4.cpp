#include <iostream>
#include <fstream>
#include <string>
#include <vector>

bool is_anagram(std::string a,std::string b) {
    static int count[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    if (a.length() != b.length())
        return false;
    for (auto i : a) {
        count[i-'a']++;
    }
    for (auto i : b) {
        count[i-'a']--;
    }
    bool yes=true;
    for (int i=0;i<26;i++) {
        if (count[i]!=0){yes=false;count[i]=0;}
    }
    return yes;
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


    long unsigned valid_total = 0;
    char a;
    int line_number = 0;
    do {
        std::string word="";
        bool valid = true;
        std::vector<std::string> passphrase;

        do {
            input_file>>word;
            std::cout<<word<<' ';
            if (valid) {
                for (auto i : passphrase) {
                    if (is_anagram(i,word)) {
                        valid = false;
                        std::cout<<" ---"<<word<<" occured twice--- ";
                        break;
                    }
                }
                passphrase.push_back(word);
            }
        } while((a=input_file.get()) ==' ');
        if (valid) {
            std::cout<<"VALID!";
            valid_total++;
        }
        std::cout<<std::endl<<++line_number<<':'<<' ';
    } while(!input_file.eof());
    std::cout<<"ANSWER: "<<valid_total-1<<std::endl;// The empty newline at end of file is a false positive

    return 0;
}
