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
    if (length>3096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length]=input;
    /******************************************************************/
    static bool field[2000][2000];
    const int ship_size = 100;

    static int x_min=500,y_min=1200,x_max=1550,y_max = 1600;
    if (argc>=2 && argv[1][0]=='1'){x_min=0;x_max=50;y_min=0;y_max=50;}
    int cnt=0,x=x_min,y=y_min,answer;
    long long int program_template [program_max_size];
    for (int i=0;i<length;i++)program_template[i]=program[i];
    
    while (x<=x_max && y<=y_max) {
		for (int i=0;i<length;i++)program[i]=program_template[i];
		position = 0;rel_base = 0;

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
					if (cnt%2)input_1=x;
					else input_1 = y;
					cnt++;
					/******************************************************/
					program[index_1]=input_1;
					position+=2;
					break;
				case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position<<" niet"<<std::endl;return 1;}
				   /********** Output input_1 to the user/thing. **********/
				   field[x][y]=input_1;
				   if (x>=x_max){
					   x=x_min;
					   y++;
					   int max_on_a_line = 0, on_a_line;
					   bool success=false;
					   int x_fits,y_fits;
					   if (y-y_min >=ship_size) {
						   for (int j=y_min;j<y;j++){
							   on_a_line = 0;
							   for (int i=x_min;i<x_max;i++) {
								   if (field[i][j]){
									   on_a_line++;
									   if (!success && field[i+ship_size-1][j] && field[i][j+ship_size -1]){
										   success=true;
										   x_fits =i;
										   y_fits=j;
										   // I mixed up x and y...
										   answer = y_fits*10000 + x_fits;
									   }
								   }
								   if (y-y_min == ship_size||(success&& y==y_max-1))std::cout<<(field[i][j] ? '#':'.');
							   }if(y-y_min == ship_size||(success&& y==y_max-1))std::cout<<std::endl;
							   if (on_a_line > max_on_a_line)max_on_a_line = on_a_line;
						   }
						   if (success && y==y_max-1) {
							   std::cout<<"HURRAY! fitting unit found on "<<x_fits<<','<<y_fits<<std::endl;
                               std::ofstream image;
                               image.open("19.pgm",std::ofstream::out);image <<"P2 "<<x_max-x_min<<' '<<y -y_min<<' '<<2<<' ';
                               for (int j=y_min;j<y;j++){
								   for (int i=x_min;i<x_max;i++) {
									   if (field[i][j]){
										   if (0<=i-x_fits &&i-x_fits<ship_size && 0<=j-y_fits && j-y_fits<ship_size){
											   std::cout<<'O';
											   image << " 2";
										   } else {
											   std::cout<<'.';
											   image << " 1";
										   }
									   } else {
										   std::cout<<' ';
										   image <<" 0";
									   }
								   } std::cout<<j<<std::endl;
								   max_on_a_line = on_a_line;
						       }
						       image << std::endl;
						       image.close();
						   }
					   }
				   }
				   else x++;
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
    }
    int total =0;
    for (int i=0;i<50;i++)for(int j=0;j<50;j++)if(field[i][j])total++;
    std::cout<<"ANSWER: "<<(argc >=2 && argv[1][0]=='1' ? total : answer)<<std::endl;
    return 0;
}

