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

int main(int argc, char* argv[]) {
    /****************** SETTING UP INTCODE PROGRAM ********************/
    long long int program [4096];
    long long int input, input_1,input_2;
    int position=0, instruction, length=0, param_1,param_2,param_3,rel_base=0,index_1,index_3;
    long unsigned total_blocks=0;
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
    const int ox=board_size,oy=board_size;
    

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
	std::vector<char> instructions={};
	int cnt=0;
	while (true) {// Let the robot virtually walk.
		if (in_bounds(rx+dx,ry+dy,board_size) && board[rx+dx][ry+dy]) {
			board[rx+dx][ry+dy]--;
		    cnt++;rx+=dx;ry+=dy;
	    } else {
			if (cnt>=10)
			    instructions.push_back(cnt/10+'0');
			if (cnt>0){
			    instructions.push_back(cnt%10+'0');
			    instructions.push_back(',');
			}
			int dx2=dx,dy2=dy;
			turn('R',dx2,dy2);
			if (in_bounds(rx+dx2,ry+dy2,board_size) && board[rx+dx2][ry+dy2]) {
				dx=dx2;dy=dy2;
				instructions.push_back('R');
				instructions.push_back(',');
			} else {
			    turn('L',dx,dy);
			    if (in_bounds(rx+dx,ry+dy,board_size) && board[rx+dx][ry+dy]) {
					instructions.push_back('L');
					instructions.push_back(',');
				} else break;
			}
			cnt=0;
		}
	}
	instructions.pop_back();
	std::cout<<"The pattern to follow for part 2 is: ";
	for (auto it : instructions)
	   std::cout<<static_cast<int>(it)<<',';
    return 0;
}

