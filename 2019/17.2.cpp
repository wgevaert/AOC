#include <iostream>
#include <fstream>
#include <vector>
#include <string>

void turn(char a, int& dx,int& dy) {
    int tmp;
    switch (a) {
        case 'R':if(dx)std::swap(dx,dy);else{tmp=-1*dx;dx=-1*dy;dy=tmp;}break;
        case 'L':if(dy)std::swap(dx,dy);else{tmp=-1*dx;dx=-1*dy;dy=tmp;}break;
    }
}

bool in_bounds(int x,int y,int board_size) {
    return x>=0&&y>=0&&x<board_size&&y<board_size;
}

int main() {
    /****************** SETTING UP INTCODE PROGRAM ********************/
    long long int program [4096];
    long long int input, input_1,input_2;
    int position=0, instruction, length=0, param_1,param_2,param_3,rel_base=0,index_1,index_3;
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
    std::cout<<length<<std::endl;
    if (length>4096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    /*for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }std::cout<<std::endl;*/
    /******************************************************************/
    program[0]=2;
    std::vector<int> routine = 
    {
		65,44,66,44,65,44,67,44,66,44,67,44,66,44,65,44,67,44,66,10,
		76,44,54,44,82,44,56,44,82,44,49,50,44,76,44,54,44,76,44,56,10,
		76,44,49,48,44,76,44,56,44,82,44,49,50,10,
		76,44,56,44,76,44,49,48,44,76,44,54,44,76,44,54,10,
        121,10 //Yes, I do want a livestream :D
    };
    auto it = routine.begin();

    //Copied IntCode interpreter
    while((input = program[position]) %100 != 99) {
        param_1=(input/100)%10;param_2=(input/1000)%10;param_3=(input/10000)%10;
        if (input > 100000||param_1>2||param_2>2||param_3==1||param_3>2) {std::cout<<"ERROR: invalid parameter mode: "<<input<<" at "<<position<<std::endl;return 1;}
        instruction = input %100;
        index_1 = (param_1 ? (param_1 == 1 ? position+1 : program[position+1]+rel_base) : program[position+1]);
        input_1 = program[index_1];
        if (instruction !=3 && instruction !=4 && instruction!=9)
            input_2 = program[(param_2 ? (param_2 == 1 ? position +2 : program[position+2]+rel_base) : program[position+2])];
        if (instruction==1||instruction==2||instruction==7||instruction==8)
            index_3=(param_3 ? (param_3 == 1 ? position +3 : program[position+3]+rel_base) : program[position+3]);

        switch (input % 100) {
            case 1:
               program[index_3] = input_1 + input_2;position +=4;
                break;
            case 2:
               program[index_3] = input_1 * input_2;position +=4;
                break;
            case 3:if(param_1==1||param_2!=0||param_3!=0){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
                /*********** Do some UI-things to get input_1 *********/
                std::cout<<static_cast<char>(input_1=*it);it++;
                /******************************************************/
                program[index_1]=input_1;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
               /********** Output input_1 to the user/thing. **********/
               if (0<=input_1 && input_1<=255)
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

