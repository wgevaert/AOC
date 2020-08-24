#include <iostream>
#include <fstream>

const unsigned total_robots=1000;

struct robot{
    short low_goes_to,high_goes_to;
    bool initialised = false;
    static const unsigned no_value=-1;
    unsigned a=no_value,b=no_value,new_a=no_value,new_b=no_value;
    void step(){if(new_a!=no_value){if(a==no_value)a=new_a;else if(b==no_value)b=new_a;else std::cout<<"WHUT?"<<std::endl;}if(new_b!=no_value){if(b==no_value)b=new_b;else std::cout<<"WHUT?"<<std::endl;}new_a=new_b=no_value;}
    void give(unsigned value){if(new_a==no_value)new_a=value;else new_b=value;}
    void initialise(short low,short high){low_goes_to=low;high_goes_to=high;initialised=true;}
    unsigned give_low(){return (a<b?a:b);}
    unsigned give_high(){return (a>b?a:b);}
};

unsigned read_robots(std::ifstream& input_file,robot* robots) {
    unsigned a,b,c, robot_cnt=0;
    char x;
    while(!input_file.eof()){
    switch(input_file.get()) {
        case 'v':
            if(input_file.get()!='a'||input_file.get()!='l'||input_file.get()!='u'||input_file.get()!='e'||input_file.get()!=' '){std::cout<<"HELP!"<<std::endl;return 0;}
            input_file>>a;
            if(input_file.get()!=' '||input_file.get()!='g'||input_file.get()!='o'||input_file.get()!='e'||input_file.get()!='s'||input_file.get()!=' '||input_file.get()!='t'||input_file.get()!='o'||input_file.get()!=' '||input_file.get()!='b'||input_file.get()!='o'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"HELLEPIE!"<<std::endl;return 0;}
            input_file>>b;
            if(b>=total_robots){std::cout<<"TOO MANY ROBOTS: "<<b<<std::endl;return 0;}
            if(b>robot_cnt)robot_cnt=b;
            robots[b].give(a);
            break;
        case 'b':
            if(input_file.get()!='o'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"WHERE'S MY MOM?"<<std::endl;return 0;}
            input_file>>a;
            if(input_file.get()!=' '||input_file.get()!='g'||input_file.get()!='i'||input_file.get()!='v'||input_file.get()!='e'||input_file.get()!='s'||input_file.get()!=' '||input_file.get()!='l'||input_file.get()!='o'||input_file.get()!='w'||input_file.get()!=' '||input_file.get()!='t'||input_file.get()!='o'||input_file.get()!=' '){std::cout<<"I FAILED"<<std::endl;return 0;}
            switch(input_file.get()) {
                case 'b':
                    if(input_file.get()!='o'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"KILL ME PLEASE"<<std::endl;return 0;}
                    input_file>>b;
                    break;
                case 'o':
                    if(input_file.get()!='u'||input_file.get()!='t'||input_file.get()!='p'||input_file.get()!='u'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"I CANT LIVE LIKE THIS"<<std::endl;return 0;}
                    input_file>>b;
                    b=-1-b;
                    break;
                default:std::cout<<"WEIRDNESS"<<std::endl;return 0;
            }
            if((x=input_file.get())!=' '||(x=input_file.get())!='a'||(x=input_file.get())!='n'||(x=input_file.get())!='d'||(x=input_file.get())!=' '||(x=input_file.get())!='h'||(x=input_file.get())!='i'||(x=input_file.get())!='g'||(x=input_file.get())!='h'||(x=input_file.get())!=' '||(x=input_file.get())!='t'||(x=input_file.get())!='o'||(x=input_file.get())!=' '){std::cout<<"STOP THIS"<<x<<std::endl;return 0;}
            switch(input_file.get()) {
                case 'b':
                    if(input_file.get()!='o'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"KILL ME PLEASE"<<std::endl;return 0;}
                    input_file>>c;
                    break;
                case 'o':
                    if(input_file.get()!='u'||input_file.get()!='t'||input_file.get()!='p'||input_file.get()!='u'||input_file.get()!='t'||input_file.get()!=' '){std::cout<<"I CANT LIVE LIKE THIS"<<std::endl;return 0;}
                    input_file>>c;
                    c=-1-c;
                    break;
                default:std::cout<<"WEIRDNESs"<<std::endl;return 0;
            }
            if(a>=total_robots){std::cout<<"TOO many ROBOTS: "<<a<<std::endl;return 0;}
            if(a>robot_cnt)robot_cnt=a;
            robots[a].initialise(b,c);
            break;
        default:if(!input_file.eof()){std::cout<<"WEIRDNEss"<<std::endl;return 0;}
    }if(input_file.get()!='\n'&&!input_file.eof()){std::cout<<"FINISH YOUR FILES WITH NEWLINE PLEASE"<<std::endl;return 0;}}
    return robot_cnt+1;
}

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}

    robot robots[total_robots];

    unsigned robot_cnt = read_robots(input_file,robots);
    if(!robot_cnt){std::cout<<"READING ERROR"<<std::endl;return 1;}
    input_file.close();

    bool something_happened;
    std::cout<<" THERE ARE "<<robot_cnt<<" ROBOTS"<<std::endl;
    do {
        for(unsigned i=0;i<robot_cnt;i++){if(!robots[i].initialised)std::cout<<i<<" HAS PROBLEMS"<<std::endl;robots[i].step();}
        something_happened=false;
        for(unsigned i=0;i<robot_cnt;i++){
            if(robots[i].a!=robot::no_value&&robots[i].b!=robot::no_value){
//std::cout<<i<<" has "<<robots[i].give_low()<<','<<robots[i].give_high()<<" and gives to "<<robots[i].low_goes_to<<','<<robots[i].high_goes_to<<std::endl;
                if(robots[i].give_low()==17&&robots[i].give_high()==61)std::cout<<"ANSWER 1 IS "<<i<<std::endl;
                if(robots[i].low_goes_to<0)
                    std::cout<<"OUTPUT "<<-1-robots[i].low_goes_to<<" RECEIVES "<<robots[i].give_low()<<" FROM "<<i<<std::endl;
                else
                    robots[robots[i].low_goes_to].give(robots[i].give_low());
                if(robots[i].high_goes_to<0)
                    std::cout<<"OUTPUT "<<-1-robots[i].high_goes_to<<" RECEIVES "<<robots[i].give_high()<<" FROM "<<i<<std::endl;
                else
                    robots[robots[i].high_goes_to].give(robots[i].give_high());
                robots[i].a=robots[i].b=robot::no_value;
                something_happened=true;
            }
        }
        for(unsigned i=0;i<robot_cnt;i++)robots[i].step();
    }while(something_happened);
    for(unsigned i=1;i<robot_cnt;i++){if(robots[i].a!=robot::no_value)std::cout<<i<<" still has "<<robots[i].a<<std::endl;if(robots[i].b!=robot::no_value)std::cout<<i<<" also has "<<robots[i].b<<std::endl;}

    return 0;
}
