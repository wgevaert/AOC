#include <iostream>

int main() {
    long long int program [1024];
    long long int blueprint_program [1024];
    long long int input, input_1, input_2, input_signal,output,max_output=0;
    int inputs[5];
    int meukjes[2];
    int position=0, length=0, param_1,param_2,input_index,meukjes_index;
    char a;
    std::cin>>input;
    while(std::cin.get() == ',') {
        blueprint_program[length]=input;
        length ++;
        std::cin>>input;
    }
    blueprint_program[length]=input;
    for (int i=0;i<=length;i++) {
        std::cout<<blueprint_program[i]<<',';
    }std::cout<<std::endl;
    // Programma is gelezen.
    for (int i=0;i<=3125;i++) {
        inputs[0]=i%5;inputs[1]=(i/5)%5;inputs[2]=(i/25)%5;inputs[3]=(i/125)%5;inputs[4]=(i/625)%5;input_signal=0;bool invalid=false;for(int j=1;j<5;j++)for(int k=0;k<j;k++)if(inputs[j]==inputs[k])invalid=true;
        if(invalid)continue;
        std::cout<<"Attempting program with inputs "<<inputs[0]<<' '<<inputs[1]<<' '<<inputs[2]<<' '<<inputs[3]<<' '<<inputs[4]<<std::endl;
        for (int j=0;j<5;j++) {
            meukjes[0]=inputs[j];meukjes[1]=input_signal;meukjes_index=0;position=0;
            for (int j=0;j<=length;j++) {
                program[j]=blueprint_program[j];
            }
            while((input = program[position]) %100 != 99) {
                param_1=(input/100)%10;param_2=(input/1000)%10;
                if (input > 10000||param_1>1||param_2>1) {std::cout<<"ERROR: invalid parameter mode: "<<input<<" at "<<position<<std::endl;return 1;}
                if (input %100 <=2||(input%100>=5&&input%100<=8)) {input_1 = (param_1 ? program[position+1] : program[program[position +1]]); input_2 = (param_2 ? program[position+2] : program[program[position +2]]);}

                switch (input % 100) {
                    case 1:
                        program[program[position+3]] = input_1 + input_2;std::cout<<"Setting "<<program[position+3]<<" to "<<program[program[position+3]] <<'='<<input_1<<'+'<<input_2<<std::endl;position +=4;
                        break;
                    case 2:
                        program[program[position+3]] = input_1 * input_2;std::cout<<"Setting "<<program[position+3]<<" to "<<program[program[position+3]] <<'='<<input_1<<'*'<<input_2<<std::endl;position +=4;
                        break;
                    case 3:if(param_1||param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
                        program[program[position+1]] = meukjes[meukjes_index];std::cout<<"Stored "<<program[program[position+1]]<<"=meukjes["<<meukjes_index<<"] at "<<program[position+1]<<std::endl;meukjes_index++;
                        position+=2;
                        break;
                    case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}input_signal=(param_1 ? program[position+1] : program[program[position+1]]);std::cout<<"stdout: "<<input_signal<<std::endl;position+=2;break;
                    case 5:if(input_1)position=input_2;else position+=3;break;
                    case 6:if(!input_1)position=input_2;else position+=3;break;
                    case 7:
                        program[program[position+3]]=(input_1<input_2?1:0);
                        position+=4;
                        break;
                    case 8:
                        program[program[position+3]]=(input_1==input_2?1:0);
                        position+=4;
                        break;
                    default : std::cout<<"ERROR: unexpected value encountered: " << program[position]<<" at position "<<position<<std::endl;return 1;
                }std::cout<<std::endl;
            }
        }
        if (input_signal > max_output){max_output=input_signal;std::cerr<<max_output<<std::endl;}

    }
    return 0;
}
