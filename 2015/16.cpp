#include <iostream>
#include <string>
#include <unordered_map>

struct range {
    int value;
    char operation;
    range(int a,char b){value=a;operation=b;}
};

const std::unordered_map<std::string, range> numbers {
    {"children", range(3,'=')},
    {"cats", range(7,'>')},
    {"samoyeds", range(2,'=')},
    {"pomeranians", range(3,'<')},
    {"akitas", range(0,'=')},
    {"vizslas", range(0,'=')},
    {"goldfish", range(5,'<')},
    {"trees", range(3,'>')},
    {"cars", range(2,'=')},
    {"perfumes", range(1,'=')}      
};

int main() {
    std::string line;
    int i=1,dummy;
    while(true) {
        std::getline(std::cin,line,' ');
        if (line.size()==0)
            break;
        if (line != std::string("Sue")) {
            std::cout<<"HUH? "<<line<<std::endl;
            break;
        }
        std::cin>>dummy;
        if (dummy != i) {
            std::cout<<"Sue numbering is incorrect "<<dummy<<std::endl;
            break;
        }
        std::cin.get();//':'
        bool this_sue=true;
        do {
            std::cin.get();//' '
            std::getline(std::cin,line,':');
            //std::cout<<line<<std::endl;
            //std::cout<<line[0]<<' '<<line<<' '<<numbers.at(line)<<std::endl;
            std::cin.get();//' '
            std::cin>>dummy;
            //std::cout<<'*'<<dummy<<std::endl;
            if ((numbers.at(line).operation == '=' && numbers.at(line).value != dummy) || (numbers.at(line).operation == '<' && numbers.at(line).value <= dummy) || (numbers.at(line).operation == '>' && numbers.at(line).value >= dummy)) {
                this_sue=false;
                break;
            }
        } while (std::cin.get() != '\n');
        if (this_sue)
            std::cout<<"Sue "<<i<<" seems promising"<<std::endl;
        else while(std::cin.get() != '\n');
        ++i;
    }
    return 0;
}
