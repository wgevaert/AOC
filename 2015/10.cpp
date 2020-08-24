#include <iostream>
#include <string>

int main(int argc, char*argv[]) {
	if (argc<=2) {
		std::cout<<"PLEASE PROVIDE 2 INPUTS"<<std::endl;
		return 1;
	}
	int i=0, cnt=0;
	char cur;
	std::string input[2] = {std::string(argv[1]),""};
	std::cout<<i<<':'<<input[0]<<std::endl;
	while (i<std::stoi(argv[2])) {
		input[(i+1)%2]="";
    	for (unsigned j=0;j<input[i%2].size();j++) {
	    	cur = input[i%2][j];
	    	cnt = 0;
	    	do {
				j++;
				cnt++;
			} while (j<input[i%2].size() && input[i%2][j]==cur);
			j--;
			if (cnt>10)std::cout<<"HELPP"<<std::endl;
			input[(i+1)%2] += cnt+'0';
			input[(i+1)%2] += cur;
    	}
    	std::cout<<i<<':'<<' '<<static_cast<int>(input[i%2][0])-'0'<<"…"<<static_cast<int>(input[i%2][0])-'0'<<std::endl;
    	i++;
	}
	std::cout<<"ANSWER: "<<input[0].size()<<std::endl;
	return 0;
}
