#include <iostream>
#include <string>

bool escapable(char a) {
	return a=='"' || a=='\\';
}

int main() {
	std::string line;
	long unsigned char_cnt=0, mem_cnt=0, exc_cnt=0;

	while(true) {
		std::getline(std::cin, line);
		if(line.size() == 0)
		    break;
		exc_cnt +=line.size()+2;
		char_cnt +=line.size();
		mem_cnt += line.size() -2;
		std::cout<<'"';
		for (unsigned i=0;i<line.size();i++) {
			if(escapable(line[i]))
			    exc_cnt++;
		}std::cout<<'"'<<std::endl;
	};
	std::cout<<"PART 1: "<<char_cnt<<'-'<<mem_cnt<<'='<<char_cnt-mem_cnt<<	std::endl;
	std::cout<<"PART 2: "<<exc_cnt<<'-'<<char_cnt<<'='<<' '<<exc_cnt - char_cnt<<std::endl;
	return 0;
}
