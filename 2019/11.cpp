#include <iostream>
#include <fstream>
#include <string>

void turn (int & x, int & y, int direction) {
    switch (direction) {
        case 0://turn left
           std::cout<<"TURNING LEFT"<<std::endl;
           switch (x) {
              case -1:
                  if (y!=0){std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);}x=0;y=-1;
                  break;
              case 0 :
                  switch (y) {
                      case 1:
                          x=-1;y=0;
                          break;
                      case -1:
                          x=1;y=0;
                          break;
                      default :
                         std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);
                  }
                  break;
              case 1 :
                  if (y!=0){std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);}x=0;y=1;
                  break;
              default :
                  std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);std::cout<<x<<' '<<y<<std::endl;
                  break;
           }
           break;
        case 1://turn right
           std::cout<<"TURNING RIGHT"<<std::endl;
           switch (x) {
              case -1:
                  if (y!=0){std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);}x=0;y=1;
                  break;
              case 0 :
                  switch (y) {
                      case 1:
                          x=1;y=0;
                          break;
                      case -1:
                          x=-1;y=0;
                          break;
                      default :
                         std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);
                  }
                  break;
              case 1 :
                  if (y!=0){std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);}x=0;y=-1;
                  break;
              default : std::cout<<"UNKNOWIN INITIAL DIRECTION "<<x<<' '<<y<<std::endl;exit(1);break;

           }
           break;
        default : std::cout<<"UNKNOWN DIRECTION "<<direction<<std::endl;exit(1);
    }
}

int main(int argc, char* argv[]) {
    const int board_size = 100;// *2
    long long int program [2048];
    long long int input, input_1,input_2;
    int position=0, instruction, length=0, param_1,param_2,param_3,rel_base=0,index_1,index_3;
    static bool hull [board_size * 2][board_size * 2];// static is initialised at false;
    static bool painted[board_size*2][board_size*2];// static is initialised at false;
    long unsigned painted_total=0;
    int rob_x=board_size,rob_y=board_size, dir_x=0, dir_y=-1, cnt=0, init_color;
    int max_x=board_size, max_y=board_size, min_x=board_size, min_y=board_size;
    bool paint_or_move=false;
    std::ofstream image;

    if (argc != 2) {
       std::cout<<"EXACTLY 1 TERMINAL PARAMETER EXPECTED"<<std::endl;
       return 1;
    }
    init_color = static_cast<int>(argv[1][0] - '0');
    if (init_color != 0 && init_color != 1) {
        std::cout<<"PARAMETER SHOULD BE EITHER 1 OR 0"<<std::endl;
        return 1;
    }
    hull[rob_x][rob_y]=init_color;
    std::cin>>input;
    while(std::cin.get() == ',') {
        program[length]=input;
        length ++;
        std::cin>>input;
    }
    std::cout<<length<<std::endl;
    if (length>1024){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }std::cout<<std::endl;

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
                // read color of current position of hull of spaceship.
                program[index_1] = hull[rob_x][rob_y]? 1 : 0;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}

                //Provide robot with instructions.
                if (paint_or_move) {
                    turn(dir_x,dir_y,input_1);
                    rob_x+=dir_x;rob_y+=dir_y;
                    if (rob_x < min_x) min_x=rob_x;
                    else if (rob_y < min_y) min_y=rob_y;
                    else if (rob_x > max_x) max_x=rob_x;
                    else if (rob_y > max_y) max_y=rob_y;
                    std::cout<<"ROBOT NOW AT "<<rob_x<<' '<<rob_y<<" WITH BOARD DIMENSIONS "<<min_x<<','<<min_y<<" UP TO "<<max_x<<','<<max_y<<std::endl;
                    image.open(std::string((cnt<1000?"0":""))+std::string((cnt<100?"0":""))+std::string((cnt<10?"0":""))+std::to_string(cnt) + ".pgm",std::ofstream::out);image <<"P2 "<<max_x-min_x+1<<' '<<max_y -min_y+1<<' '<<3<<' ';
                    if (!image.good()){std::cout<<"FAILED TO OPEN "<<(cnt+".txt")<<std::endl;return 1;}
                    for (int y=min_y;y<=max_y;y++) {
                        for(int x=max_x;x>=min_x;x--) {
                            if (x==rob_x && y==rob_y) {
                                std::cout<<'O';
                                image<<' '<<'2';
                            } else {image<<' '<<(hull[x][y]?'3':(painted[x][y]?'1':'0'));std::cout<<(hull[x][y]?'#':' ');}
                       }std::cout<<std::endl;image<<std::endl;
                   }image<<std::endl;
                   image.close();cnt++;
               } else {
                   if (input_1==1)hull[rob_x][rob_y]=true;else if(input_1==0)hull[rob_x][rob_y]=false;else{std::cerr<<"OEI!"<<std::endl;return 1;}
                   if (!painted[rob_x][rob_y]){
                       painted_total++;std::cout<<"TOTAL PAINT INCREASED"<<std::endl;
                       image.open(std::string((cnt<1000?"0":""))+std::string((cnt<100?"0":""))+std::string((cnt<10?"0":""))+std::to_string(cnt) + ".pgm",std::ofstream::out);image <<"P2 "<<max_x-min_x+1<<' '<<max_y -min_y+1<<' '<<3<<' ';
                       if (!image.good()){std::cout<<"FAILED TO OPEN "<<(cnt+".txt")<<std::endl;return 1;}
                           for (int y=min_y;y<=max_y;y++) {
                           for(int x=max_x;x>=min_x;x--) {
                               if (x==rob_x && y==rob_y) {
                                   std::cout<<'O';
                                   image<<' '<<'2';
                               } else {image<<' '<<(hull[x][y]?'3':(painted[x][y]?'1':'0'));std::cout<<(hull[x][y]?'#':' ');}
                           }std::cout<<std::endl;image<<std::endl;
                       }image<<std::endl;
                       image.close();cnt++;
                   }
                   painted[rob_x][rob_y]=true;
                   std::cout<<"PAINTED "<<rob_x<<' '<<rob_y<<(hull[rob_x][rob_y]?" WHITE":" BLACK")<<std::endl;

               }
               paint_or_move = !paint_or_move;
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

    // Calculate total painted area and output full board.
    std::cout<<std::endl<<"TOTAL PAINTED AREA:"<<std::endl;
    for (int y=2*board_size-1;y>=0;y--) {int cnt=0;
        for(int x=0;x<=2*board_size;x++) {
            std::cout<<(painted[x][y]?'O':'.'); if(painted[x][y])++cnt;
        }std::cout<<' '<<cnt<<std::endl;
    }

    std::cout<<"ANSWER: "<<painted_total<<std::endl;
    return 0;
}
