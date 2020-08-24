#include <iostream>
#include <fstream>

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

void get_direction(int command, int& dx, int & dy) {
    switch(command) {
        case NORTH: dx=0;dy=1;break;
        case SOUTH: dx=0;dy=-1;break;
        case WEST: dx=-1;dy=0;break;
        case EAST: dx=1;dy=0;break;
        default:std::cout<<command<<" IS NOT A VALID DIRECTION"<<std::endl;
    }
}

struct flower {
    bool flowing = true;
    int px,py;
    flower(int pos_x,int pos_y) {px=pos_x;py=pos_y;flowing=true;}
    flower(){};
};

int main() {
    /****************** SETTING UP INTCODE PROGRAM ********************/
    long long int program [3096];
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
    if(false)std::cout<<length<<std::endl;
    if (length>3096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    if(false)for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }std::cout<<std::endl;
    /******************************************************************/

    const int board_size=200,ox=board_size,oy=board_size;
    static int robot_pos[2*board_size][2*board_size];
    robot_pos[ox][oy]=2;
    int dir=0,px,py,dx,dy,x_oxygen,y_oxygen,x_min=board_size,x_max=board_size,y_min=board_size,y_max=board_size;
    unsigned cnt,k;
    bool twos=true;

    /****************** Copied IntCode interpreter ********************/
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
                k=-1;//unsigned overflow to max
                cnt=0;
                for(int i=1;i<=4;i++){ // Walk to least resistance.
                    get_direction(i,dx,dy);
                    cnt = robot_pos[px+ox+dx][py+oy+dy];
                    // 1 is actually infinite
                    if (cnt !=1 && cnt < k){
                        k = cnt;
                        dir = i;
                    }
                }
                get_direction(dir,dx,dy);
                input_1 = dir;
                /******************************************************/
                program[index_1]=input_1;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
               /********** Output input_1 to the user/thing. **********/
               switch (input_1) {
                   case 0:
                       robot_pos[px+ox+dx][py+oy+dy]=1;
                       if(px+ox+dx>x_max)x_max=px+ox+dx;else if(px+ox+dx<x_min)x_min=px+ox+dx;
                       if(py+oy+dy>y_max)y_max=py+oy+dy;else if(py+oy+dy<y_min)y_min=py+oy+dy;
                       break;
                   case 1:
                       px+=dx;if(px+ox>x_max)x_max=px+ox;else if(px+ox<x_min)x_min=px+ox;
                       py+=dy;if(py+oy>y_max)y_max=py+oy;else if(py+oy<y_min)y_min=py+oy;
                       if (robot_pos[px+ox][py+oy]>=2)
                           robot_pos[px+ox][py+oy]++;  
                        else robot_pos[px+ox][py+oy]=2;
                       break;
                   case 2:
                       px+=dx;if(px+ox>x_max)x_max=px+ox;else if(px+ox<x_min)x_min=px+ox;
                       py+=dy;if(py+oy>y_max)y_max=py+oy;else if(py+oy<y_min)y_min=py+oy;
                       robot_pos[px+ox][py+oy]=-1;
                       x_oxygen = px;
                       y_oxygen = py;
               }
               twos=false;
               for(int j=y_max;j>=y_min;j--){
                   for(int i=x_min;i<=x_max;i++){
                       if(robot_pos[i][j]==2)
                           // There are better ways to find the full board,
                           // But I'm lazy.
                           twos=true;
                   }
               }
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
        if (!twos)
            //The board should be fully explored by now.
            break;
    }
    // Starting part 2 of the problem...
    for(int j=y_max;j>=y_min;j--){
       for(int i=x_min;i<=x_max;i++){
           if(robot_pos[i][j]==1){}
               //std::cout<<'#';
           else if(robot_pos[i][j]==-1){}
               //std::cout<<'O';
           else {
               robot_pos[i][j]=0;
               //std::cout<<' ';
           }
       }//std::cout<<std::endl;
    }
    flower myflowers[50];
    bool flowed,flowing=true;
    int x_stopped=x_oxygen+ox,y_stopped=y_oxygen+oy,flower_cnt=1;
    myflowers[0]=flower(x_oxygen,y_oxygen);
    cnt=0;
    while (flowing) {
        flowing = false;
        int dummy=flower_cnt;
        for (int flwr=0;flwr<dummy;flwr++) {
            if (!myflowers[flwr].flowing){
                continue;
            }
            px=myflowers[flwr].px;
            py=myflowers[flwr].py;
            flowed = false;
            for (int i=1;i<=4;i++) {
                get_direction(i,dx,dy);
                if (robot_pos[ox+px+dx][oy+py+dy]==0) {
                    if (!flowed) {
                        myflowers[flwr].px = px+dx;
                        myflowers[flwr].py = py+dy;
                    } else {
                        myflowers[flower_cnt++]=flower(px+dx,py+dy);
                        flowing = true;
                    }
                    robot_pos[ox+px+dx][oy+py+dy] = -1;
                    flowed = true;
                }
            }
            if (!flowed) {
                myflowers[flwr].flowing = false;
            }
            if (myflowers[flwr].flowing) {
                flowing = true;
            }
        }
        std::cout<<"\n\nAFTER "<<cnt+1<<" MINUTES:"<<std::endl;
        for(int j=y_max;j>=y_min;j--){
           for(int i=x_min;i<=x_max;i++){
               if (i==x_stopped && j==y_stopped)
                   std::cout<<'X';
               else if(robot_pos[i][j]==1)
                   std::cout<<'#';
               else if(robot_pos[i][j]==-1)
                   std::cout<<'O';
               else
                   std::cout<<' ';
           }std::cout<<std::endl;
        }
        ++cnt;
    }
    std::cout<<"AFTER "<<cnt-1<<" MINUTES, ALL THE ELVES CAN HAPPILY BREATHE AGAIN"<<std::endl;
    return 0;
}

