#include <iostream>

int main() {
   unsigned a,b,c,min_1,min_2;
   long long unsigned total=0;
   do {
      std::cin>>a;std::cin.get();std::cin>>b;std::cin.get();std::cin>>c;std::cin.get();
      if (a>=b&& a>=c) {min_1=b;min_2=c;}else if (b>= c) {min_1=a;min_2=c;}else{min_1=a;min_2=b;}
      total += 2*min_1+2*min_2+a*b*c;
   } while (std::cin.eof() == false);
   std::cout<<total - (2*min_1+2*min_2+a*b*c)<<std::endl;
   return 0;
}
