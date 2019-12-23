#include <iostream>
#include <fstream>
#include <queue>

int main(int argc, char* argv[]) {
    /****************** SETTING UP INTCODE PROGRAM ********************/
    const int program_max_size = 4096;
    const int cpu_cnt = 50;
    long long int program [program_max_size][cpu_cnt];
    long long int input, input_1,input_2;
    bool use_2,use_3, verbose = (argc>=2 && argv[1][0]=='v');
    static bool has_stopped[cpu_cnt];
    static bool idling[cpu_cnt];
    static int position[cpu_cnt];
    static bool booted[cpu_cnt];
    std::queue<long long> myqueue[cpu_cnt];
    static int rel_base[cpu_cnt];
    int send_address,send_process_cnt=0, cur_program=0, stopped_cnt = 0, idling_cnt = 0, instruction, length=0, param_1,param_2,param_3,index_1,index_2,index_3;
    std::ifstream input_file;
    input_file.open("/tmp/input.txt");// You might want to change this sometimes
    if (!input_file.good()) {
        std::cout<<"COULD NOT OPEN INPUT FILE"<<std::endl;return 1;
    }
    input_file>>input;
    while(input_file.get() == ',') {
        program[length][0]=input;
        length ++;
        input_file>>input;
    }
    input_file.close();
    if (length>3096){std::cout<<"OEI!"<<std::endl;return 1;}
    program[length][0]=input;
    for (int i=0;i<=length;i++) {
		for (int j=0;j<cpu_cnt;j++)
		    program[i][j]=program[i][0];
    }
    for (int i=length+1;i<program_max_size;i++)
        for(int j=0;j<cpu_cnt;j++)
            program[i][j] = 0;
    /******************************************************************/
    const int NAT_address =255;
    long long NAT_values[2];
    long long packets_send = 0,NAT_prev_Y = 666,ans_1=1234567890;
    

    //Copied IntCode interpreter
    while(stopped_cnt<cpu_cnt) {// Let every computer do one step, then go to next computer
		if (has_stopped[cur_program]) {
			cur_program = (cur_program +1)%cpu_cnt;
			continue;
		}
		input = program[position[cur_program]][cur_program];
		if(input%100 == 99) {
			has_stopped[cur_program] = true;
			std::cout<<cur_program<<" STOPPED "<<std::endl;
			stopped_cnt++;
		}
        param_1=(input/100)%10;param_2=(input/1000)%10;param_3=(input/10000)%10;
        if (input > 100000||param_1>2||param_2>2||param_3==1||param_3>2) {std::cout<<"ERROR: invalid parameter mode: "<<input<<" at "<<position[cur_program]<<std::endl;return 1;}
        instruction = input %100;
        index_1 = (param_1 ? (param_1 == 1 ? position[cur_program]+1 : program[position[cur_program]+1][cur_program]+rel_base[cur_program]) : program[position[cur_program]+1][cur_program]);

        use_2 = instruction !=3 && instruction !=4 && instruction!=9;
        use_3 = instruction==1||instruction==2||instruction==7||instruction==8;
        if (use_2)
            index_2=(param_2 ? (param_2 == 1 ? position[cur_program] +2 : program[position[cur_program]+2][cur_program]+rel_base[cur_program]) : program[position[cur_program]+2][cur_program]);
        if (use_3)
            index_3=(param_3 ? (param_3 == 1 ? position[cur_program] +3 : program[position[cur_program]+3][cur_program]+rel_base[cur_program]) : program[position[cur_program]+3][cur_program]);

        if (index_1>=program_max_size-3||index_2>=program_max_size-3||index_3>=program_max_size-3) {
			std::cout<<index_1<<' '<<index_2<<' '<<index_3<<" YOUR INTCODE BUFFER IS TOO SMALL ON COMPUTER "<<cur_program<<std::endl;
			return 1;
		}
		input_1=program[index_1][cur_program];
		if (use_2)
		    input_2=program[index_2][cur_program];

        switch (instruction) {
            case 1:
               program[index_3][cur_program] = input_1 + input_2;position[cur_program] +=4;
                break;
            case 2:
               program[index_3][cur_program] = input_1 * input_2;position[cur_program] +=4;
                break;
            case 3:if(param_1==1||param_2!=0||param_3!=0){std::cout<<"HUH? Ik snap "<<input<<" op "<<position[cur_program]<<" niet"<<std::endl;return 1;}
                /*********** Do some UI-things to get input_1 *********/
                if (!booted[cur_program]) {
					input_1=cur_program;
					booted[cur_program] = true;
				}
				else if (myqueue[cur_program].empty()) {
					input_1=-1;
					if (!idling[cur_program]) {
						idling[cur_program] = true;
					    idling_cnt++;
					}
				}
				else {
					input_1 = myqueue[cur_program].front();
					myqueue[cur_program].pop();
				    if(verbose)std::cout<<" COMPUTER "<<cur_program<<" RECEIVED "<<input_1<<std::endl;
				}
                /******************************************************/
                program[index_1][cur_program]=input_1;
                position[cur_program]+=2;
                break;
            case 4:if(param_2){std::cout<<"HUH? Ik snap "<<input<<" op "<<position[cur_program]<<" niet"<<std::endl;return 1;}
               /********** Output input_1 to the user/thing. **********/
               if (!send_process_cnt) {
				   send_address = input_1;
			   } else {
				   if (send_address>=cpu_cnt) {
					   if (send_address == NAT_address) {
						   if(verbose)std::cout<<"NAT RECEIVED "<<input_1<<" FROM "<<cur_program<<std::endl;
						   NAT_values[send_process_cnt-1] = input_1;
						   if (ans_1 == 1234567890 && send_process_cnt == 2)
						       ans_1=input_1;
					   }
					   else {
					       std::cout<<"ERROR: TRYING TO SEND "<<input_1<<" TO NON_EXISTING CPU "<<send_address<<" AT STEP "<<send_process_cnt<<std::endl;
					   }
				   }
				   else {
				       myqueue[send_address].push(input_1);
				       if (idling[send_address]) {
						   idling[send_address] = false;
						   idling_cnt--;
					   }
					   packets_send ++;
				   }
			   }
			   send_process_cnt  = (send_process_cnt+1)%3;
               /*******************************************************/
               position[cur_program]+=2;
               break;

            case 5:if(input_1)position[cur_program]=input_2;else position[cur_program]+=3;break;
            case 6:if(!input_1)position[cur_program]=input_2;else position[cur_program]+=3;break;
            case 7:
                program[index_3][cur_program]=(input_1<input_2?1:0);
                position[cur_program]+=4;
                break;
            case 8:
                program[index_3][cur_program]=(input_1==input_2?1:0);
                position[cur_program]+=4;
                break;
            case 9:rel_base[cur_program] +=input_1;position[cur_program]+=2;break;
            default : std::cout<<"ERROR: unexpected value encountered: " << program[position[cur_program]][cur_program]<<" at position "<<position[cur_program]<<std::endl;return 1;
        }
        if (!(send_process_cnt)) {// Are we still sending? Then do not switch to the next thread
    		cur_program = (cur_program+1) % cpu_cnt;
		}
		if (idling_cnt == cpu_cnt && packets_send) {// NAT also sends at the start when I do not check for that.
			myqueue[0].push(NAT_values[0]);
			myqueue[0].push(NAT_values[1]);
			if(verbose)std::cout<<"NAT sends "<<NAT_values[0]<<','<<NAT_values[1]<<" TO 0"<<std::endl;
			if (NAT_values[1]!=NAT_prev_Y)
			    NAT_prev_Y = NAT_values[1];
			else {
				std::cout<<NAT_prev_Y<<" OCCURRED TWICE!!!\nPART 1 HAS ANSWER "<<ans_1<<std::endl;
				return 0;
			}
			idling_cnt--;
			idling[0]=false;
		}
    }
    std::cout<<" ALL COMPUTERS STOPPED, THAT'S WEIRD..."<<std::endl;
    return 0;
}

