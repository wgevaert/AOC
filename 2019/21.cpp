#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    /****************** SETTING UP INTCODE PROGRAM ********************/
    const int program_max_size = 4096;
    long long int program [program_max_size];
    long long int input, input_1,input_2;
    bool use_2,use_3;
    int position=0, instruction, length=0, param_1,param_2,param_3,rel_base=0,index_1,index_2,index_3;
    std::ifstream input_file;
    input_file.open("/tmp/input.txt");// You might want to change this sometimes
    if (!input_file.good()) {
        std::cout<<"COULD NOT OPEN INPUT FILE"<<std::endl;return 1;
    }
    input_file>>input;
    while(input_file.get() == ',') {
        program[length]=input;
        length ++;
        input_file>>input;
    }
    input_file.close();
    if (length>program_max_size-5){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    /******************************************************************/
    // JUMP WHEN THERE IS A HOLE, AND YOU CAN CONTINUE FROM YOUR EXISTING LANDING SPOT.
    char const * routine_2 = "NOT A J\nNOT B T\nOR T J\nNOT C T\nOR T J\nAND D J\nNOT E T\nNOT T T\nOR H T\nAND T J\nRUN\n\0";
    // JUMP WHEN THERE IS A HOLE, AND THERE IS AN EXISTING LANDING SPOT.
    char const * routine_1 = "NOT A J\nNOT B T\nOR T J\nNOT C T\nOR T J\nAND D J\nWALK\n\0";
    const char* routine;
    if (argc >= 2 && argv[1][0] == '2')
        routine = routine_2;
    else routine = routine_1;
    int routine_nr=0;

    //Copied IntCode interpreter
    while((input = program[position]) %100 != 99) {
        param_1=(input/100)%10;param_2=(input/1000)%10;param_3=(input/10000)%10;
        if (input > 100000||param_1>2||param_2>2||param_3==1||param_3>2) {std::cout<<"ERROR: invalid parameter mode: "<<input<<" at "<<position<<std::endl;return 1;}
        instruction = input %100;
        index_1 = (param_1 ? (param_1 == 1 ? position+1 : program[position+1]+rel_base) : program[position+1]);

        use_2 = instruction !=3 && instruction !=4 && instruction!=9;
        use_3 = instruction==1||instruction==2||instruction==7||instruction==8;
        if (use_2)
            index_2=(param_2 ? (param_2 == 1 ? position +2 : program[position+2]+rel_base) : program[position+2]);
        if (use_3)
            index_3=(param_3 ? (param_3 == 1 ? position +3 : program[position+3]+rel_base) : program[position+3]);

        if (index_1>=program_max_size-3||index_2>=program_max_size-3||index_3>=program_max_size-3) {
			std::cout<<"YOUR INTCODE BUFFER IS TOO SMALL"<<std::endl;
			return 1;
		}
		input_1=program[index_1];
		if (use_2)
		    input_2=program[index_2];

        switch (input % 100) {
            case 1:
               program[index_3] = input_1 + input_2;position +=4;
                break;
            case 2:
               program[index_3] = input_1 * input_2;position +=4;
                break;
            case 3:if(param_1==1||param_2!=0||param_3!=0){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
                /*********** Do some UI-things to get input_1 *********/
                input_1=static_cast<int>(routine[routine_nr++]);
                /******************************************************/
                program[index_1]=input_1;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
               /********** Output input_1 to the user/thing. **********/
               if (input_1 < 256)
                   std::cout<<static_cast<char>(input_1);
               else std::cout<<input_1<<std::endl;
               /*******************************************************/
               position+=2;
               break;

            case 5:if(input_1)position=input_2;else position+=3;break;
            case 6:if(!input_1)position=input_2;else position+=3;break;
            case 7:
                program[index_3]=(input_1<input_2?1:0);
                position+=4;
                break;
            case 8:
                program[index_3]=(input_1==input_2?1:0);
                position+=4;
                break;
            case 9:rel_base +=input_1;position+=2;break;
            default : std::cout<<"ERROR: unexpected value encountered: " << program[position]<<" at position "<<position<<std::endl;return 1;
        }
    }
    return 0;
}

