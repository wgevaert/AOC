#include <iostream>
#include <algorithm>
#include <bitset>

int main() {
	const int eggnog=150;
	const int size=20;
	int unit[size] = {3,4,4,6,9,10,17,17,21,27,31,34,36,38,41,43,44,44,46,47};
	int possibilities=0;
	unsigned min_cont_cnt=-1;
	for (uint32_t i=0;i<(1<<size);i++){
		int total =0;
		unsigned cont_cnt=0;
		for (int j=size-1;j>=0;j--) {
			if ((i/(1<<j))%2) {
				total +=unit[j];
				cont_cnt++;
			}
			if (total > eggnog) {
			    break;
			}
		}
		if (total == eggnog) {
		    
		    if (cont_cnt == min_cont_cnt) {
				possibilities++;
			}
			else if (cont_cnt < min_cont_cnt) {
				possibilities = 1;
				min_cont_cnt = cont_cnt;
			}
		}
	}
	std::cout<<possibilities<<std::endl;
	return 0;
}
