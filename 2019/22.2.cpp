#include <iostream>
#include <string>
#include <vector>

const long long unsigned numberofcards = 119315717514047;

#define DEAL_NEW 'N'
#define DEAL_INC 'I'
#define CUT 'C'

struct linear_relation {
	long long unsigned a,b;
	linear_relation(long long unsigned new_a,long long unsigned new_b){a=new_a;b=new_b;}
};

long long unsigned safe_mult(long long unsigned a,long long unsigned b) {
	long long unsigned res = 0;
	while (b>0) {
		if (b%2)res = (res+a)%numberofcards;
		a = (a+a) %numberofcards;
		b/=2;
	}
	return res;
}

long long unsigned euler_phi(long long unsigned a) {
	unsigned long long b=2,phi=1;
	while (b*b<a) {
		if (!(a%b)) {
			a/=b;
			phi*=(b-1);
			while (!(a%b)) {
				a/=b;
				phi*=b;
			}
		}
		if (b==2)b++;else b+=2;
	}
	if (a>1)phi*=(a-1);
	return phi;
}

linear_relation deal_new_stack(linear_relation pos) {
	return linear_relation((numberofcards - pos.a),numberofcards - pos.b - 1);
}

linear_relation cut (linear_relation pos, long long unsigned cut_dist) {
	cut_dist = (cut_dist + numberofcards) % numberofcards;// Positive cuts are easier.
	return linear_relation(pos.a,(pos.b - cut_dist + numberofcards) %numberofcards);
}

linear_relation deal_with_increment(linear_relation pos, long long unsigned increment) {
	return linear_relation(safe_mult(pos.a,increment),safe_mult(pos.b,increment));
}


linear_relation recurse(linear_relation pos,long long unsigned numberoftimes) {
	//a(a(ax+b)+b)+b = a**3x+b(a**2+a+1) = a**3 x + b* ((a**3-1)/(a-1));
	numberoftimes %=numberofcards -1;
	linear_relation rtr(1,0);
	long long unsigned apown = pos.a;
	while (numberoftimes >0) {
	    if (numberoftimes %2) {
			rtr.a = safe_mult(rtr.a,apown);
		}
		apown=safe_mult(apown,apown)%numberofcards;
		numberoftimes = (numberoftimes >>1);
	}
	long long unsigned ainv = 1,apowinv=1,apowpow=rtr.a;
	numberoftimes = euler_phi(numberofcards)-1;//(a-1)^{this number} = (a-1)^-1 mod numberofcards
	std::cout<<numberoftimes<<'~'<<std::endl;
	apown=pos.a-1;
	while (numberoftimes > 0) {
	    if (numberoftimes % 2) {
			ainv = safe_mult(ainv,apown);
			apowinv = safe_mult(apowinv,apowpow);
		}
		apown=safe_mult(apown,apown);
		apowpow = safe_mult(apowpow,apowpow);
		numberoftimes = (numberoftimes >>1);
	}
	rtr.b = safe_mult(safe_mult(rtr.a-1,ainv),pos.b);
	std::cout<<rtr.a<<"*x+"<<rtr.b<<std::endl;
	unsigned long long final_ans = safe_mult(apowinv,2020-rtr.b+numberofcards);
	std::cout<<rtr.a<<'*'<<final_ans<<'+'<<rtr.b<<'='<<(safe_mult(rtr.a,final_ans)+rtr.b)%numberofcards<<" mod "<<numberofcards<<std::endl;
	
	return rtr;
}

int main(int argc, char* argv[]) {
	linear_relation pos_2019(1,0); // the position we're interested in.
	std::string line;
	//std::cout<<safe_mult(6,6)<<std::endl;return 1;
	while (true) {
		std::getline(std::cin,line);
		if (line.size() == 0)
		    break;
		if (line[0] == 'd') {
			if(line[5] == 'i') {
				pos_2019 = deal_new_stack(pos_2019);
				std::cout<<"NEW: "<<pos_2019.a<<"*x+"<<pos_2019.b<<std::endl;
			} else if (line[5] == 'w') {
				pos_2019 = deal_with_increment(pos_2019,std::stoull(line.substr(20,line.size()-20)));
				std::cout<<"INC: "<<pos_2019.a<<"*x+"<<pos_2019.b<<" WITH "<<std::stoull(line.substr(20,line.size()-20))<<std::endl;
			} else std::cout<<"UNEXPECTED CHAR: "<<line[5]<<' '<<static_cast<int>(line[5])<<std::endl;
		} else if (line[0] == 'c') {
			pos_2019 = cut(pos_2019,std::stoi(line.substr(4,line.size()-4)));
			std::cout<<"CUT: "<<pos_2019.a<<"*x+"<<pos_2019.b<<" WITH "<<std::stoi(line.substr(4,line.size()-4))<<std::endl;
		}
	}
	std::cout<<pos_2019.a<<"*x + "<<pos_2019.b<<" % "<<numberofcards<<std::endl;
	pos_2019 = recurse(pos_2019,101741582076661);
	std::cout<<pos_2019.a<<"*x+"<<pos_2019.b<<std::endl;

	return 0;
}
