#include <iostream>
#include <fstream>

#define EMPTY 0
#define WALL 1
#define BLOCK 2
#define PADDLE 3
#define BALL 4

int main(int argc, char* argv[]) {
    const int board_size = 100;// *2
    long long int program [3096];// Had to increase this.
    long long int input, input_1,input_2;
    int position=0, instruction, length=0, param_1,param_2,param_3,rel_base=0,index_1,index_3;
    int ox=board_size,oy=board_size, x_min=0,x_max=0,y_min=0,y_max=0;
    static int board[board_size*2][board_size*2]; //static is empty at program start
    for (int i=0;i<2*board_size;i++)for(int j=0;j<2*board_size;j++)if(board[i][j]!=EMPTY){std::cout<<"STATIC IS A LIE"<<std::endl;return 1;}//My kind of unit test.
    int cnt=0, x, y,paddle_x,ball_x;
    bool started=false,manual=!(argc>=2 &&argv[1][0]=='a');// Do you want to play this fun game manually? (You can!)
    long unsigned total_blocks=0;
    std::ifstream input_file;
    input_file.open("/tmp/input.txt");
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
    if (length>3096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[0]=2;//Play for free
    program[length]=input;
    std::cout<<"+-----------------------------------------------------+"<<std::endl;
    std::cout<<"|             WELCOME TO THE INTCODE GAME!            |"<<std::endl;
    std::cout<<"|       WOULD YOU LIKE TO LET THE COMPUTER PLAY?      |"<<std::endl;
    std::cout<<"|   THEN RESTART THE PROGRAM WITH AN 'a' AS ARGUMENT  |"<<std::endl;
    std::cout<<"|             (THE CUMPUTER IS REALY GOOD)            |"<<std::endl;
    std::cout<<"+-----------------------------------------------------+\n\n"<<std::endl;
    std::cout<<"STARTING GAME..."<<std::endl;

    // Copied IntCode interpreter
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
               std::cout<<std::endl<<std::endl<<"YOUR SCORE: "<<board[ox-1][oy]<<std::endl;
               started=true;
               for(int j=y_min+oy;j<=y_max+oy;j++) {
                   for(int i=x_min+ox;i<=x_max+ox;i++) {
                       switch(board[i][j]) {
                           case EMPTY:std::cout<<' ';break;
                           case WALL:std::cout<<'#';break;
                           case BLOCK:std::cout<<'x';break;
                           case PADDLE:std::cout<<'_';break;
                           case BALL:std::cout<<'@';break;
                           default:
                               std::cout<<"UNEXPECTED VALUE "<<board[i][j]<<" ENCOUNTERED AT "<<i-ox<<' '<<j-oy<<std::endl;return 1;
                       }
                   }std::cout<<std::endl;
               }
               if (!manual) {
                   if (paddle_x<ball_x)
                       input_1=1;
                   else if (paddle_x>ball_x)
                       input_1=-1;
                   else input_1=0;
                       
               } else {
                    do {
                        std::cout<<"PLEASE PROVIDE SOME INPUT, Left, Right or Nothing"<<std::endl;
                        switch(std::cin.get()) {
                            case 'L':input_1=-1;break;
                            case 'N':input_1=0;break;
                            case 'R':input_1=1;break;
                            default:input_1=5;
                        }
                    } while (input_1<-1||input_1>1);
                }
                program[index_1]=input_1;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}

               //Draw board.
               if (!(cnt%3))x=input_1;
               else if (!((cnt-1)%3))y=input_1;
               else if (!((cnt-2)%3)) {
                   if (0>x+ox || x>board_size || 0>y+oy||y>board_size) {
                       std::cout<<"BOARD IS TOO SMALL FOR VALUES "<<x<<' '<<y<<std::endl;
                   }
                   if (x==-1&&y==0)  {}
                   else if (x<x_min)x_min=x;
                   else if (x>x_max)x_max=x;
                   if (y<y_min)y_min=y;
                   else if (y>y_max) y_max =y;
                   board[x+ox][y+oy]=input_1;
                   if (input_1==PADDLE) {
                       paddle_x=x;
                   } else if (input_1 == BALL) {
                       ball_x=x;
                   } else if(!started && input_1==BLOCK) {
                       ++total_blocks;
                   }
               }
               cnt++;
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
    std::cout<<"YOUR FINAL SCORE IS: "<<board[ox-1][oy]<<std::endl;
    std::cout<<"THERE WERE "<<total_blocks<<" BLOCKS IN TOTAL"<<std::endl;
    return 0;
}
