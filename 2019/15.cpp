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
    int dir=0,px,py,dx,dy,x_min=board_size,x_max=board_size,y_min=board_size,y_max=board_size;
    unsigned cnt,k;
    bool found=false;

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
                k=-1; // unsigned overflow to max.
                cnt=0;
                for(int i=1;i<=4;i++){
                    get_direction(i,dx,dy);
                    cnt = robot_pos[px+ox+dx][py+oy+dy];
                    //When found: Walk back over the least non-zero weights
                    if (cnt !=1 && cnt < k && (!found || cnt !=0)){
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
               std::cout<<"ROBOT NOW AT "<<px<<' '<<py<<" MOVING "<<dx<<','<<dy<<" SAYING "<<input_1<<", BOARD SIZE "<<x_min<<','<<y_min<<" UP TO "<<x_max<<','<<y_max<<std::endl;
               switch (input_1) {
                   case 0:
                       robot_pos[px+ox+dx][py+oy+dy]=1;
                       if(px+ox+dx>x_max)x_max=px+ox+dx;else if(px+ox+dx<x_min)x_min=px+ox+dx;
                       if(py+oy+dy>y_max)y_max=py+oy+dy;else if(py+oy+dy<y_min)y_min=py+oy+dy;
                       break;
                   case 1:
                       px+=dx;if(px+ox>x_max)x_max=px+ox;else if(px+ox<x_min)x_min=px+ox;
                       py+=dy;if(py+oy>y_max)y_max=py+oy;else if(py+oy<y_min)y_min=py+oy;
                       if (robot_pos[px+ox][py+oy]>=2) {
                           if (!found)
                               robot_pos[px+ox][py+oy]++;
                           else
                               robot_pos[px+ox][py+oy]+=7;
                        }
                        else robot_pos[px+ox][py+oy]=2;
                       break;
                   case 2:
                       px+=dx;if(px+ox>x_max)x_max=px+ox;else if(px+ox<x_min)x_min=px+ox;
                       py+=dy;if(py+oy>y_max)y_max=py+oy;else if(py+oy<y_min)y_min=py+oy;
                       robot_pos[px+ox][py+oy]=-1;
                       std::cout<<"HURRAY!"<<std::endl;
                       found = true;
               }
               for(int j=y_max;j>=y_min;j--) {
                   for(int i=x_min;i<=x_max;i++){
                       if (i==ox+px && j==oy+py)
                           std::cout<<'R';
                       else if(robot_pos[i][j]==1)
                           std::cout<<'#';
                       else if(robot_pos[i][j]==0)
                           std::cout<<' ';
                       else if(robot_pos[i][j]==2)
                           std::cout<<'.';
                       else std::cout<<(robot_pos[i][j]%10);
                   }std::cout<<std::endl;
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
        if (found && px==0&&py==0) {
            break;
        }
    }
    cnt =0;
    // A visual indicator to see the full path.
   for(int j=y_max;j>=y_min;j--){
       for(int i=x_min;i<=x_max;i++){
           if (i==ox && j==oy)
               std::cout<<'O';
           else if(robot_pos[i][j]>=9) {
               std::cout<<' ';
               cnt++;
           }else if(robot_pos[i][j]==-1)
               std::cout<<'X';
           else if (robot_pos[i][j]==1)
               std::cout<<'#';
           else if (robot_pos[i][j]==0)
               std::cout<<'?';
           else std::cout<<'@';
       }std::cout<<std::endl;
   }
   std::cout<<"ANSWER: "<<cnt+1<<std::endl;
    return 0;
}

