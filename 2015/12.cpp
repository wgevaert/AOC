#include <iostream>

int main() {
    char a = std::cin.get();
    static bool in_array[100];
    static bool valid[100];
    valid[0]=true;
    int depth=0,dummy,sign;
    bool get;
    static long long int subtotal[100];
    while (a != '\n') {
		get=true;
         switch (a) {
			 case '{':
			     valid[++depth]=true;
			     subtotal[depth]=0;
			     in_array[depth]=false;
			     break;
			 case '[':
			     valid[++depth]=true;
			     subtotal[depth]=0;
			     in_array[depth]=true;
			     break;
			 case '}':
			 case ']':
			     depth--;
			     if(valid[depth+1]) {
					 subtotal[depth]+=subtotal[depth+1];
				 }
			     break;
			 default:
			     if (!in_array[depth] && a=='r'&&std::cin.get()=='e'&&std::cin.get()=='d') {
					 valid[depth]=false;
				 }
			     if (a=='-'||('0'<=a&&a<='9')) {
					 if (a=='-') {
						 sign=-1;
						 a=std::cin.get();
					 } else
					     sign = 1;
					 dummy=0;
					 while('0'<=a&&a<='9') {
						 dummy=10*dummy+a-'0';
						 a=std::cin.get();
					 }
					 subtotal[depth] += sign*dummy;
					 get=false;
				 }
		 }
		 if(get)
		     a=std::cin.get();
    }
    std::cout<<subtotal[0]<<std::endl;
}
