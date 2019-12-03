#include <iostream>

int main () {
long long floor=0;
char a;int c=0;
while((a = std::cin.get()) == '(' || a == ')' ) {
   switch (a) {
       case '(': ++floor;break;
       case ')': --floor;break;
    }c++;if ( floor < 0 ){ std::cout<< floor<< " at " << c << std::endl;return 0;} else {std::cout<<floor<<std::endl;}
}std::cout<<static_cast<int>(a)<<std::endl;
std::cout<< floor;
return 0;
}
