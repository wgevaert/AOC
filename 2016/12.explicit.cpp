#include <iostream>

int main(int argc,char**argv) {
if(argc<2){std::cout<<" USAGE: {program} {number}"<<std::endl;return 0;}
int a=0,b=0,c=argv[1][0]-'0',d=0;

a=1;
b=1;
d=26;
if(c){
c=7;
do {d++;
c--;
}while(c);}
do {c=a;
do {a++;
b--;
}while(b);
b=c;
d--;
}while(d);
c=18;
do {d=11;
do{a++;
d--;
}while(d);
c--;
}while (c);
    std::cout<<a<<std::endl;
    return 0;
}
