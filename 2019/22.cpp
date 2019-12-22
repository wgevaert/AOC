#include <iostream>
#include <string>
#include <vector>

/*const*/ int numberofcards = 10007;

#define DEAL_NEW 'N'
#define DEAL_INC 'I'
#define CUT 'C'

struct operation {
	int arg;
	char func;
	operation(char fc){func=fc;}
	operation(char fc,int ag){func=fc;arg=ag;}
};

int deal_new_stack(int pos) {
	return numberofcards - pos-1;
}

int cut (int pos, int cut_dist) {
	cut_dist = (cut_dist + numberofcards) % numberofcards;// Positive cuts are easier.
	if (pos < cut_dist)
	    return pos + (numberofcards - cut_dist);
	else
	    return pos - cut_dist;
}

int deal_with_increment(int pos, int increment) {
	return (increment * pos) % numberofcards;
}

int main(int argc, char* argv[]) {
	int pos_2019=2019; // the position we're interested in.
	if (argc >=2) {
		numberofcards = 10;
		pos_2019 = std::stoi(argv[1]);
	}
	std::string line;
	std::vector<operation> operations;
	while (true) {
		std::getline(std::cin,line);
		if (line.size() == 0)
		    break;
		if (line[0] == 'd') {
			if(line[5] == 'i') {
				std::cout<<"DEALING "<<pos_2019<<" INTO NEW STACK TO ";
				std::cout<<(pos_2019 = deal_new_stack(pos_2019))<<std::endl;
				operations.push_back(operation(DEAL_NEW));
			} else if (line[5] == 'w') {
				operations.push_back(operation(DEAL_INC,std::stoi(line.substr(20,line.size()-20))));
				std::cout<<"DEALING "<<pos_2019<<" WITH INCREMENT "<<operations.back().arg<<" TO ";
				std::cout<<(pos_2019 = deal_with_increment(pos_2019,operations.back().arg))<<std::endl;
			} else std::cout<<"UNEXPECTED CHAR: "<<line[5]<<' '<<static_cast<int>(line[5])<<std::endl;
		} else if (line[0] == 'c') {
			operations.push_back(operation(CUT,std::stoi(line.substr(4,line.size()-4))));
			std::cout<<" Cutting "<<pos_2019<<" TO "<<operations.back().arg<<" GIVES ";
			std::cout<<(pos_2019 = cut(pos_2019,operations.back().arg))<<std::endl;
		}
	}
	std::cout<<operations.size()<<" OPERATIONS READ "<<pos_2019<<std::endl;
	
	return 0;
}
