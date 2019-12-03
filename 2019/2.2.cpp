#include <iostream>
#include <string>

int main() {
    std::string program [1024];
    std::string input_1, input_2, operation = "";
    int program_2 [1024];
    int position=0, length=0, output, dummy;

    std::cin>>dummy;
    while(std::cin.get() == ',') {
        program_2[length]=dummy;
        length ++;
        std::cin>>dummy;
    }
    program_2[length]=dummy;
    for (int i=0;i<=length;i++) {
        program[i]=std::to_string(program_2[i]);
    }
    while(std::stoi(program[position]) != 99) {
        if (position != 0) {
            input_1 = program[std::stoi(program[position+1])];
            input_2 = program[std::stoi(program[position+2])];
            output = std::stoi(program[position + 3]);
        } else {
            input_1='a';
            input_2='b';
        }
        switch (std::stoi(program[position])) {
            case 1:
               program[output] = '(' + input_1 + '+' + input_2 + ')';
               break;
            case 2:
               program[output] = '(' + input_1 + '*' + input_2 + ')';
               break;
            default : std::cout<<"ERROR: unexpected value encountered: " << program[position]<<" at position "<<position<<std::endl;return 1;
        }
    }
    std::cout<<program[0];
    return 0;
}
