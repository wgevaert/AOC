#include <iostream>
#include <string>

int main() {
    long long int program [1024];
    long long int input_1, input_2;
    int index_1, index_2, index_3, position=0, length=0, output;
    std::cin>>input_1;

    while(std::cin.get() == ',') {
        program[length]=input_1;
        length ++;
        std::cin>>input_1;
    }
    program[length]=input_1;
    program[1]=12;program[2]=2;
    for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }
    while(program[position] != 99) {

        index_1 = program[position+1];
        index_2 = program[position+2];
        index_3 = position + 3;
        input_1 = program[index_1];
        input_2 = program[index_2];
        output = program[index_3];

        switch (program[position]) {
            case 1: 
               for(int i=0;i<=length;i++) {
                   std::string text = std::to_string(program[i]);
                   if (index_1==i) {
                       text = '[' + text + ']';
                   }
                   if (index_2==i) {
                       text = '[' + text + ']';
                   }
                   if (index_3==i) {
                       text = '(' + text + ')';
                   }
                   if (output==i) {
                       text = '<' + text +  '>';
                   }
                   std::cout<< text<<' ';
               }std::cout<<std::endl;
               program[output] = input_1 + input_2;

                std::cout << position <<": setting "
                          << output << " = ("<<index_3<<") to "
                          << program[output]<< " = (" << index_1 << ")+("<< index_2 << ") = "
                          << input_1 <<'+'<< input_2 << std::endl;
                break;
            case 2:
               for(int i=0;i<=length;i++) {
                   std::string text = std::to_string(program[i]);
                   if (index_1==i) {
                       text = '{' + text + '}';
                   }
                   if (index_2==i) {
                       text = '{' + text + '}';
                   }
                   if (index_3==i) {
                       text = '(' + text + ')';
                   }
                   if (output==i) {
                       text = '<' + text +  '>';
                   }
                   std::cout<< text<< ' ';
               }std::cout<<std::endl;
               program[output] = input_1 * input_2;
                std::cout << position <<": setting "
                          << output << " = ("<<index_3<<") to "
                          << program[output] << " = (" << index_1 << ")*(" << index_2 << ") = "
                          << input_1 <<'*'<< input_2 << std::endl;
                break;
            default : std::cout<<"ERROR: unexpected value encountered: " << program[position]<<" at position "<<position<<std::endl;return 1;
        }std::cout<<std::endl;
        position +=4;
    }
    std::cout<<length<<' '<<program[0]<<std::endl;
    return 0;
}
