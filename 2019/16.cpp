#include <iostream>
#include <string>

long long myabs(long long a){
	if(a<0)return -1*a;
	return a;
}


int get_10(int mod_2,int mod_5) {// get 0<=c<10 such that c%2=mod_2 and c%5=mod_5;
	if (mod_2==mod_5%2) {
		return mod_5;
	}
	return (mod_5+5)%10;
}

int main(int argc, char* argv[]) {
	std::string line[2]={"",""};
	const int phases=100;
	const int basepattern[4]={0,1,0,-1};
	int offset=0;
	bool part2=false;
	long int subtotal;
	if(argc<2) {
		std::cout<<"PLEASE PROVIDE AN INPUT"<<std::endl;
	}
	std::string input=argv[1];
	if (argc>=3 && argv[2][0]=='2') {
		std::string dummy = input;
		offset = std::stoi(input.substr(0,7));
		// 10000 = 2^13+2^10+2^9+2^8+2^4
		for (int i=1;i<=13;i++) {
			dummy = dummy + dummy;
			// Now dummy contains 2^i times 'input'
			if (i==4||i==8||i==9||i==10||i==13) {
				line[0]+=dummy;
			}
		}
		part2=true;
		if (line[0].size() >= 2*offset || line[0].size() - 8<=offset) {
			std::cout<< "Well, that's too difficult."<<std::endl;
			return 1;
		}
		line[0] = line[0].substr(offset, line[0].size()-offset);
	}
	else line[0]=input;
	std::cout<<"FOUND STRING OF SIZE "<<line[0].size()<<std::endl;
	
	if (!part2) {
		std::cout<<"HERE!"<<std::endl;
    	for (int i=0;i<phases+1;i++){
	    	line[(i+1)%2]="";
	    	std::cout<<line[i%2]<<std::endl;
		    for (int j=0;j<static_cast<int>(line[i%2].size());j++) {
		        subtotal =0;
    			for (int k=0;k<static_cast<int>(line[i%2].size());k++) {
	    			subtotal += basepattern[((k+1)/(j+1))%4]*(line[i%2][k]-'0');
		    	}
			    line[(i+1)%2]+=static_cast<char>((myabs(subtotal)%10)+'0');
    		}
	    	std::cout<<i<<std::endl;
	    	std::cout<<line[0]<<std::endl;
		}
		std::cout<<"DONE!";
		for(int i=offset;i<offset+8;i++)std::cout<<line[0][i%line[0].size()];
		std::cout<<std::endl<<line[1]<<std::endl;
	} else {
		int ans [8]={0,0,0,0,0,0,0,0};
		int binom_2[2][2]={{1,0},{1,1}};
		int binom_5[5][5]={{1,0,0,0,0},{1,1,0,0,0},{1,2,1,0,0},{1,3,3,1,0},{1,4,1,4,1}};
		for (unsigned i=0;i<line[0].size();i++) {
			unsigned adic = 99+i;
			unsigned bin = get_10(
			    binom_2[(adic >> 6)%2][1] * binom_2[(adic >> 5)%2][1] * binom_2[(adic >> 1)%2][1] * binom_2[(adic)%2][1], //99 = 1100011_2
			    binom_5[(adic/25)%5][3]*binom_5[(adic/5)%5][4]*binom_5[(adic)%5][4] // 99 = 344_5
			); // = binom(99+i,99) % 10
			for (unsigned j=0;j<8;j++) {
				if (i+j >= line[0].size())
				    break;
				ans[j] = (ans[j] + bin*(line[0][i+j] - '0'))%10;
			}
		}
		for (int i=0;i<8;i++)std::cout<<ans[i];
		std::cout<<std::endl;
	}
	return 0;
}
