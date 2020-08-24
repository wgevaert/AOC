#include <iostream>

struct empty {};

int main()
{
   int *hoi[4];
   empty *unit[4] = {0,0,0,0};
   std::cout<<typeid(unit).name()<<' '<<typeid(unit[4]).name()<<' '<<typeid(hoi).name()<<' '<<typeid(new int).name()<<' '<<typeid(new empty).name()<<std::endl;
   return 0;
}
