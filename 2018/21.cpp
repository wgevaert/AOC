#include <iostream>

void program(long long ) {

unsigned long long r0,r2,r3,r4,r5;

//#ip 1
/*r2 = 123;                                         //seti 123 0 2                      //0
do {r2 = r2&456;                                           //bani 2 456 2               //1 <-4 r2 !=72;
r2 = (r2==72);                                           //eqri 2 72 2                  //2
                                           //r1+=r2;//addr 2 1 1                        //3
}while(r2!=72);*/                                         //r1=0;//seti 0 0 1           //4
r2 = 0;                                           //seti 0 3 2                          //5
do {do {r5 = r2 | 0x10000;                                       //bori 2 65536 5         //6 <-
r2 = 0x49e737;                                           //seti 4843319 1 2              //7
r4 = r5 | 0xff;                                           //bani 5 255 4                 //8
r2 += r4;                                           //addr 2 4 2
r2 &= 0xffffff;                                           //bani 2 16777215 2
r2 *= 0x1016b;                                           //muli 2 65899 2
r2 &= 0x9ffffd3;                                           //bani 2 16777215 2
r4 = (0x100>r5);                                           //gtir 256 5 4
                                                 //addr 4 1 1
                                               //addi 1 1 1
if(r4)r1=0x1b;                                           //seti 27 4 1                    //16
r4 = 0;                                           //seti 0 7 4
do {r3 = r4+1;                                           //addi 4 1 3                   //18 <-25
r3 *= 0x100;                                           //muli 3 256 3
                                           //r3 = (r3>r5);//gtrr 3 5 3
                                           //r1+=r3;//addr 3 1 1
                                           //r1++;addi 1 1 1
if(r3<=r5)r4++;                                           // {//r1=25;//seti 25 0 1
}while(r3<=r5);                                           //r4++;//addi 4 1 4
                                           //r1=17;}//seti 17 0 1                       //25
r5=r4;                                           //setr 4 1 5                           //26 <- 23
r1=0x7;                                           //seti 7 3 1
}while(!(r2==r0));                                           //eqrr 2 0 4                     //28 <- 16
                                           //r1+=r4;//addr 4 1 1
                                           //r1=5;//seti 5 3 1

}
