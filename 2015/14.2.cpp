#include <iostream>
#include <string>
#include <vector>

struct reindeer {
	int speed,run,wait,pos=0,score=0;
	reindeer(int spd, int rn, int wt) {speed=spd;run=rn;wait=wt;}
};

std::vector<reindeer> read() {
    std::string integer = "";
	char a;
	std::vector<reindeer> rtn;
    int speed,run,wait;
	while (true) {
        while ((a=std::cin.get())<='0'||a>'9')if(std::cin.eof())return rtn;
        while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		speed = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())<='0'||a>'9');
		while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		run = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())<='0'||a>'9');
		while (a>='0'&&a<='9'){integer+=a;a=std::cin.get();}
		wait = std::stoi(integer);
		integer="";
		while ((a=std::cin.get())!='\n');
		rtn.push_back(reindeer(speed,run,wait));
    }
}

int main() {
   const int time=2503;
   std::vector<reindeer> deers = read();

   for (int i=0;i<time;i++) {
	   bool init = false;
	   reindeer *max;
	   for (auto &it : deers) {
		   if (i%(it.run+it.wait)<it.run)
			   it.pos+=it.speed;
		   if (!init || max->pos < it.pos) {
			   max = &it;
			   init=true;
		   }
	   }
	   max->score++;
   }
   int winner=0;
   for (auto &it : deers) {
	   if(it.score > winner)
	       winner =it.score;
   }
   std::cout<<winner<<std::endl;

   return 0;
   
}
