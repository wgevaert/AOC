#include <iostream>

int main() {
    long long int program [1024];
    long long int input, input_1,input_2;
    int position=0, length=0, param_1,param_2;
    char a;
    std::cin>>input;std::cout<<"First input should be 1, remember!"<<std::endl;
    while(std::cin.get() == ',') {
        program[length]=input;
        length ++;
        std::cin>>input;
    }
    program[length]=input;
    for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }std::cout<<std::endl;
    while((input = program[position]) %100 != 99) {
        param_1=(input/100)%10;param_2=(input/1000)%10;
        if (input > 10000||param_1>1||param_2>1) {std::cout<<"ERROR: invalid parameter mode: "<<input<<" at "<<position<<std::endl;return 1;}
        if (input %100 <=2) {input_1 = (param_1 ? program[position+1] : program[program[position +1]]); input_2 = (param_2 ? program[position+2] : program[program[position +2]]);}

        switch (input % 100) {
            case 1:
               program[program[position+3]] = input_1 + input_2;std::cout<<"Setting "<<program[position+3]<<" to "<<program[program[position+3]] <<'='<<input_1<<'+'<<input_2<<std::endl;position +=4;
                break;
            case 2:
               program[program[position+3]] = input_1 * input_2;std::cout<<"Setting "<<program[position+3]<<" to "<<program[program[position+3]] <<'='<<input_1<<'*'<<input_2<<std::endl;position +=4;
                break;
            case 3:if(param_1||param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
                a='n';
                while (a != 'y') {
                    std::cout<<"Please provide some input for "<<program[position+1]<<std::endl;
                    std::cin>>input_2;
                    std::cout<<"Do you want to store "<<input_2<<" at "<<program[position+1]<<'?'<<std::endl;
                    std::cin>>a;
                }
                program[program[position+1]] = input_2;std::cout<<"Stored "<<program[program[position+1]]<<" at "<<program[position+1]<<std::endl;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}std::cout<<"stdout: "<<(param_1 ? program[position+1] : program[program[position+1]])<<std::endl;position+=2;break;
            default : std::cout<<"ERROR: unexpected value encountered: " << program[position]<<" at position "<<position<<std::endl;return 1;
        }std::cout<<std::endl;
    }
    return 0;
}
