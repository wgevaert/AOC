#include <iostream>
#include <string>
#include <vector>

const long long unsigned numberofcards = 119315717514047;

struct linear_relation { // represents a*x+b
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

linear_relation recurse_and_invert(linear_relation pos,long long unsigned numberoftimes) {
	//a(a(ax+b)+b)+b = a**3x+b(a**2+a+1) = a**3 x + b* ((a**3-1)/(a-1)) =
	// a**-3 * (x-b*((a**3-1)/(a-1))) = a**(phi-3)*x - b*a**(phi-3)*(a**3-1)*(a-1)**(phi-1))
	long long unsigned phi = euler_phi(numberofcards);
	long long unsigned exp1 = numberoftimes % phi;
	long long unsigned exp0 = phi - exp1;
	long long unsigned exp2 = phi - 1;
	long long unsigned ans0=1,ans1=1,ans2=1;
	long long unsigned base0 = pos.a, base2 = pos.a-1;
	while (exp2 > 0) {// Calculate a**exp0, a**exp1 and (a-1)**exp2
	    if (exp0 %2) {
			ans0 = safe_mult(ans0,base0);
		}
	    if (exp1 %2) {
			ans1 = safe_mult(ans1,base0);
		}
	    if (exp2 %2) {
			ans2 = safe_mult(ans2,base2);
		}
		base0=safe_mult(base0,base0);
		base2=safe_mult(base2,base2);
		exp0 = (exp0 >>1);
		exp1 = (exp1 >>1);
		exp2 = (exp2 >>1);
	}
	return linear_relation(
	    ans0,
	    numberofcards - safe_mult(
	        pos.b,
	        safe_mult(
	            ans0,
	            safe_mult(
	                ans1 -1,
	                ans2
	            )
	        )
	    )
	);
}

int main(int argc, char* argv[]) {
	linear_relation pos_2019(1,0); // the position we're interested in.
	std::string line;
	while (true) {
		std::getline(std::cin,line);
		if (line.size() == 0)
		    break;
		if (line[0] == 'd') {
			if(line[5] == 'i') {
				pos_2019 = deal_new_stack(pos_2019);
				std::cout<<"NEW: "<<pos_2019.a<<"*x+"<<pos_2019.b<<std::endl;
			} else if (line[5] == 'w') {
				pos_2019 = deal_with_increment(
				    pos_2019,
				    std::stoull(line.substr(20,line.size()-20))
				);
				std::cout<<"INC: "<<pos_2019.a
				         <<"*x+"<<pos_2019.b<<" WITH "
				         <<std::stoull(line.substr(20,line.size()-20))
				         <<std::endl;
			} else std::cout<<"UNEXPECTED CHAR: "<<line[5]<<' '
			                <<static_cast<int>(line[5])<<std::endl;
		} else if (line[0] == 'c') {
			pos_2019 = cut(
			    pos_2019,
			    std::stoi(line.substr(4,line.size()-4))
			);
			std::cout<<"CUT: "<<pos_2019.a<<"*x+"<<pos_2019.b
			         <<" WITH "<<std::stoi(line.substr(4,line.size()-4))
			         <<std::endl;
		}
	}
	std::cout<<pos_2019.a<<"*x + "<<pos_2019.b<<" % "<<numberofcards<<std::endl;
	pos_2019 = recurse_and_invert(pos_2019,101741582076661);
	std::cout<<pos_2019.a<<"*x+"<<pos_2019.b<<std::endl;
	std::cout<<(safe_mult(pos_2019.a,2020) + pos_2019.b)%numberofcards<<std::endl;

	return 0;
}
