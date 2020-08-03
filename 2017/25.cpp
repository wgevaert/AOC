#include <iostream>
#include <vector>

struct tape {
    std::vector<bool> positive={},negative={};
    bool has(int);
    bool get(int);
    void set(int,bool);
};

bool tape::has(int a){return (a<0&&-1-a<negative.size())||(a>=0&&a<positive.size());}
bool tape::get(int a){if(a<0){if(negative.size()>-1-a)return negative[-1-a];negative.push_back(0);return negative.back();}else if(positive.size()>a)return positive[a]; else {positive.push_back(0);return positive.back();}}
void tape::set(int a,bool value){
    if(!has(a)){std::cout<<"OOPS"<<std::endl;exit(1);}if(a<0)negative[-1-a]=value;else positive[a]=value;
}

const unsigned a=0,b=1,c=2,d=3,e=4,f=5;
const int right=-1,left=1;

unsigned A(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=right;
    return b;
  case 1:
    the_tape.set(position,0);
    position+=left;
    return c;
}}
unsigned B(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=left;
    return a;
  case 1:
    the_tape.set(position,1);
    position+=right;
    return d;
}}
unsigned C(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=right;
    return a;
  case 1:
    the_tape.set(position,0);
    position+=left;
    return e;
}}
unsigned D(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=right;
    return a;
  case 1:
    the_tape.set(position,0);
    position+=right;
    return b;
}}
unsigned E(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=left;
    return f;
  case 1:
    the_tape.set(position,1);
    position+=left;
    return c;
}}
unsigned F(tape& the_tape,int& position){switch(the_tape.get(position)){
  case 0:
    the_tape.set(position,1);
    position+=right;
    return d;
  case 1:
    the_tape.set(position,1);
    position+=right;
    return a;
}}

typedef unsigned (*state)(tape&,int&);

int main(){
    state turing_machine[6]={A,B,C,D,E,F};
    unsigned cur_state=0;
    int position=0;
    const unsigned long checksum_steps=12173597;
    tape the_tape;
    for(unsigned long i=0;i<checksum_steps;i++){
//        std::cout<</*'\r'<<std::flush;//*/std::endl<<cur_state<<','<<position<<','<<the_tape.get(position)<<std::endl;
//        for(int i=-static_cast<int>(the_tape.negative.size());i<static_cast<int>(the_tape.positive.size());i++)if(i==position)std::cout<<(the_tape.get(i)?'@':'O');else std::cout<<(the_tape.get(i)?'#':'.');

        cur_state=turing_machine[cur_state](the_tape,position);
    }
unsigned long total=0;
    for(int i=1-static_cast<int>(the_tape.negative.size())-1;i<static_cast<int>(the_tape.positive.size());i++)
        if(the_tape.get(i))total++;
    for(int i=-static_cast<int>(the_tape.negative.size());i<static_cast<int>(the_tape.positive.size());i++)if(i==position)std::cout<<(the_tape.get(i)?'@':'O');else std::cout<<(the_tape.get(i)?'#':'.');
std::cout<<total<<std::endl;
    return 0;
}
