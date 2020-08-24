#include <iostream>
#include <unordered_map>
#include <vector>
//#include <utility>

struct operation {
	std::string from, to;
	operation(const std::string fro,const std::string t) {
		from = fro; to = t;
	}
	operation(){};
};

int main() {
	std::unordered_map<std::string,std::pair<operation, int>> replacements_container={};
	std::string line;
	std::vector<operation> operations={};
	do {
		std::getline(std::cin, line);
		std::string fro="", t="";
		int i=0;
		if (line.size() == 0)break;
		while(line[i]!=' ') {
			fro += line[i++];
		};
		if (line[i++]!=' ' || line[i++]!='=' || line[i++]!='>' || line[i++]!=' ') {
			std::cout<<line[i-1]<<" WAS UNEXPECTED"<<std::endl;
		}
		while(i<line.size()) {
			t += line[i++];
		};
		std::cout<<' '<<fro<<"=>"<<t<<std::endl;
		operations.push_back(operation(fro,t));
	} while (line.size() > 0);
	std::getline(std::cin, line);
	for (auto it : operations) {
		for (int i=0;i<line.size()-it.from.size()+1;i++) {
			bool found=true;
			for (int j=0;j<it.from.size();j++) {
				if (line[i+j]!=it.from[j]){
					found=false;
					break;
				}
			}
			if (found) {
				std::string start=line.substr(0,i),end=line.substr(i+it.from.size(),line.size()-(i+it.from.size()));
				//std::cout<< start << '('<<it.from<<"->"<<it.to<<')'<<end<<'\n'<<std::endl;
				replacements_container[start+it.to+end] = {it,i};
			}
		}
	}
    std::cout<<"ANSWER: "<<replacements_container.size()<<std::endl;
    return 0;
}
