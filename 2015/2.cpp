#include <iostream>

int main() {
   unsigned a,b,c,x,y,z,min;
   long long unsigned total=0;
   do {
      std::cin>>a;std::cin.get();std::cin>>b;std::cin.get();std::cin>>c;std::cin.get(); std::cout<<a<<' '<<b<<' '<<c<<std::endl;
      x = a*b;y=a*c;z=b*c;if (x<=y && x<=z)min=x; else if (y<=z) min =y; else min = z;
      total += 2*x+2*y+2*z+min;
   } while (std::cin.eof() == false);
   std::cout<<total - (2*x+2*y+2*z+min);
   return 0;
}
