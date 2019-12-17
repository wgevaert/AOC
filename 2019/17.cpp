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

std::string print_int(int a) {
	std::string rtr="";
	if (a>='A')return std::to_string(a);
	if (a>=10)rtr += std::to_string(a/10+'0') + ",";
	rtr += std::to_string(a%10+'0');
	return rtr;
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
    //std::cout<<length<<std::endl;
    if (length>4096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    /*for (int i=0;i<=length;i++) {
        std::cout<<program[i]<<',';
    }std::cout<<std::endl;*/
    /******************************************************************/
    const int board_size=100;
    static int board[2*board_size][2*board_size];
    int x_max=0,y_max=0,px=0,py=0,rx,ry,dx,dy;

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
                std::cout<<"WHY YOU NEED INPUT?"<<std::endl;return 1;
                /******************************************************/
                program[index_1]=input_1;
                position+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
               /********** Output input_1 to the user/thing. **********/
               switch(static_cast<char>(input_1)) {
                   case '.':px++;if(px>x_max)x_max=px;break;
                   case '#':board[px++][py]=1;if(px>x_max)x_max=px;break;
                   case '^':dx=0;dy=-1;rx=px;ry=py;board[px++][py]=1;if(px>x_max)x_max=px;break;
                   case '>':dx=1;dy=0;rx=px;ry=py;board[px++][py]=1;if(px>x_max)x_max=px;break;
                   case 'v':dx=0;dy=1;rx=px;ry=py;board[px++][py]=1;if(px>x_max)x_max=px;break;
                   case '<':dx=-1;dy=1;rx=px;ry=py;board[px++][py]=1;if(px>x_max)x_max=px;break;
                   case '\n':py++;px=0;if(py>y_max)y_max=py;
               }
               //std::cout<<static_cast<char>(input_1);
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
    int calibration_param = 0;
    for (int i=0;i<y_max;i++) {
        for(int j=0;j<x_max;j++) {
            int neighbours=0;
            if (board[j][i]&&j>0&&j<x_max-1&&i>0&&i<y_max-1) {
                neighbours = board[j+1][i]+board[j-1][i]+board[j][i+1]+board[j][i-1];
            }
            if (neighbours == 4) {
                calibration_param += i*j;
                std::cout<<'O';
                board[j][i]=2;
            } else
                std::cout<<(board[j][i] ? (board[j][i]==1 ?'#':'X'):'.');
        }std::cout<<std::endl;
    }
    std::cout<<"ANSWER: "<<calibration_param<<std::endl;
    std::vector<int> instructions={};
    int cnt=0;
    while (true) {// Let the robot virtually walk.
        if (in_bounds(rx+dx,ry+dy,board_size) && board[rx+dx][ry+dy]) {
            board[rx+dx][ry+dy]--;
            cnt++;rx+=dx;ry+=dy;
        } else {
            if (cnt>0){
                instructions.push_back(cnt);
            }
            int dx2=dx,dy2=dy;
            turn('R',dx2,dy2);
            if (in_bounds(rx+dx2,ry+dy2,board_size) && board[rx+dx2][ry+dy2]) {
                dx=dx2;dy=dy2;
                instructions.push_back('R');
            } else {
                turn('L',dx,dy);
                if (in_bounds(rx+dx,ry+dy,board_size) && board[rx+dx][ry+dy]) {
                    instructions.push_back('L');
                } else break;
            }
            cnt=0;
        }
    }
    int a_end=9,b_begin=10,b_end=19,c_begin=20,c_end=29;
    bool b_decided=false, c_decided=false;
    std::vector<char> main;
    while(true) {
        int begin = a_end+1,end=begin;
        bool a_satisfied=true,b_satisfied=true,c_satisfied=true;
        while((a_satisfied||(b_decided && b_satisfied)||(c_decided&&c_satisfied)) && end<static_cast<int>(instructions.size())) {
            if (a_satisfied && (end-begin>a_end || instructions.at(end-begin) != instructions.at(end)))
                a_satisfied= false;
            if (a_satisfied && end-begin==a_end) {
                main.push_back('A');
                begin = ++end;
                b_satisfied = true;
                c_satisfied = true;
                if (end == static_cast<int>(instructions.size()))
                    break;
            }
            if (b_decided && b_satisfied && (end-begin>b_end-b_begin || instructions.at(end-begin+b_begin) != instructions.at(end)))
                b_satisfied = false;
            if (b_decided && b_satisfied && end-begin==b_end-b_begin) {
                main.push_back('B');
                begin = ++end;
                a_satisfied = true;
                c_satisfied = true;
                if (end == static_cast<int>(instructions.size()))
                    break;
            }
            if (c_decided && c_satisfied && (end-begin>c_end-c_begin || instructions.at(end-begin+c_begin) != instructions.at(end)))
                c_satisfied = false;
            if (c_decided && c_satisfied && end-begin==c_end-c_begin) {
                main.push_back('C');
                begin = ++end;
                a_satisfied = true;
                b_satisfied = true;
                if (end == static_cast<int>(instructions.size()))
                    break;
            }
            end++;
        }
        if (!(a_satisfied||(b_decided && b_satisfied)||(c_decided&&c_satisfied))) {
            if (!b_decided) {
                b_end = begin + b_end - b_begin;
                b_begin = begin;
                b_decided = true;
            } else if (!c_decided) {
                c_end = begin + c_end - c_begin;
                c_begin = begin;
                c_decided = true;
            } else {
				main.clear();
				main.push_back('A');
                if (c_end - c_begin <=2) {
                    if (b_end - b_begin <=2) {
                        a_end --;
                        b_decided = false;
                        b_end = b_begin + 9;
                    } else
                        b_end --;
                    c_decided = false;
                    c_end = c_begin+9;
                } else {
                    c_end--;
                }
            }
        } else {
            if (end == static_cast<int>(instructions.size()))
                break;
            else  {
                std::cout<<"SOMETHING REALY WEIRD GOING ON HERE!"<<std::endl;
                return 1;
            }
        }
    }
    std::cout<<"The pattern to follow for part 2 is: "<<std::endl;
    for (unsigned i=0;i<main.size();i++)
        std::cout<<static_cast<int>(main[i])<<(i==main.size()-1?"":",44,");
    std::cout<<",10,"<<std::endl;
    for (int i=0;i<=a_end;i++)
        std::cout<<print_int(instructions[i])<<(i==a_end?"":",44,");
    std::cout<<",10,"<<std::endl;
    for (int i=b_begin;i<=b_end;i++)
        std::cout<<print_int(instructions[i])<<(i==b_end?"":",44,");
    std::cout<<",10,"<<std::endl;
    for (int i=c_begin;i<=c_end;i++)
        std::cout<<print_int(instructions[i])<<(i==c_end?"":",44,");
    std::cout<<",10"<<std::endl;
    return 0;
}

