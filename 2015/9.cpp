#include <iostream>
#include <string>
#include <algorithm>

int index(std::string location) {
	switch(location[0]) {
		case 'F':
		    return 2;
		case 'N':
		    return 3;
		case 'A':
		    switch(location[1]) {
				case 'l':return 0;
				case 'r':return 1;
			}
			break;
		case 'S':
		    switch(location[1]) {
				case 'n': return 4;
				case 't': return 5;
			}
			break;
        case 'T':
            switch(location[1]) {
				case 'a':return 6;
				case 'r':return 7;
			}
	}
	std::cout<<"UNKNOWN LOCATION: ";
	for (unsigned i=0;i<location.size();i++) {
	    std::cout<<location[i]<<'('<<static_cast<int>(location[i])<<')';
    }std::cout<<std::endl;
	return 0;
	
}


int main() {
	std::string location_0, location_1;
	static unsigned long distances[8][8];
	unsigned minimal=-1,maximal=0;
	unsigned route[8] = {0,1,2,3,4,5,6,7};
	while (true) {
	    std::getline(std::cin, location_0,' ');
	    if(std::cin.eof())
	        break;
    	else if (std::cin.get()!='t')
	       std::cout<<"ILL FORMED INPUT"<<std::endl;
    	else if(std::cin.get()!='o')
	       std::cout<<"ILL FORMED_INPUT"<<std::endl;
    	else if(std::cin.get()!=' ')
	       std::cout<<"ILL_FORMED_INPUT"<<std::endl;
	    std::getline(std::cin, location_1,' ');
    	if (std::cin.get()!='=')
	       std::cout<<"ILL INPUT"<<std::endl;
	    else if(std::cin.get() !=' ')
	       std::cout<<"ILL_INPUT"<<std::endl;
	    std::cin>>distances[index(location_0)][index(location_1)];
	    distances[index(location_1)][index(location_0)] = distances[index(location_0)][index(location_1)];
	    std::cin.get();//Gobble newline
	}

	do {
		unsigned max = 0,min=-1;
		unsigned tot_dist=0;
		for (int i=0;i<8;i++) {
			unsigned dummy = distances[route[i]][route[(i+1)%8]];
			tot_dist += dummy;
			if (dummy > max) {
				max = dummy;
			}
			if (dummy < min) {
				min = dummy;
			}
		}
		if (tot_dist - max < minimal)
		    minimal = tot_dist - max;
		if (tot_dist - min > maximal)
		    maximal = tot_dist - min;
	} while (std::next_permutation(route, route+8));
	std::cout<<minimal<<' '<<maximal<<std::endl;;
	return 0;
}
