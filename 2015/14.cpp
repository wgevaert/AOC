#include <iostream>
#include <string>

int main() {
   std::string integer = "";
   char a;
   const int time=2503;
   int max=0,speed,run,wait;
   while (true) {
        while ((a=std::cin.get())<='0'||a>'9'){if(std::cin.eof()){std::cout<<"I CAN DO THINGS!"<<std::endl;return 0;}std::cout<<a;}
        while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		speed = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())<='0'||a>'9')std::cout<<a;
		while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		run = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())<='0'||a>'9')std::cout<<a;
		while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		wait = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())!='\n');
		int result = (time/(run+wait))*run*speed + (time%(run+wait) < run ? time%(run+wait) : run)*speed;
		if (result > max)max=result;
        std::cout<<std::endl<<speed<<' '<<run<<' '<<wait<<' '<<result<<' '<<max<<std::endl;
	}
   return 1;
   
}
