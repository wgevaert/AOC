#include <iostream>

bool new_main(int _a) {
    bool was0=false;
    int a=_a,b=0,c=0,d=0;
    d=a+7*362;;//cpy a d
    //c=7;//cpy 7 c
    d+=7*362;/*do{b=362;//cpy 362 b
    do{d++;//inc d
    b--;//dec b
    }while(b);//jnz b -2
    c--;//dec c
    }while(c);//jnz c -5*/
    do {a=d;//cpy d a
        do {;//jnz 0 0
            b=a;//cpy a b
            a=0;//cpy 0 a
            a=b/2;c=b%2;/*do {c=2;//cpy 2 c
            //jnz b 2
            if(!b)break;//jnz 1 6
            b--;//dec b
            c--;//dec c
            jnz c -4
            if(!c)a++;//inc a
            }while(true);*///jnz 1 -7
            b=2;//cpy 2 b
            do {//if(c) goto a;//jnz c 2
            if(!c)break;//else goto b;jnz 1 4
            b--;//a: dec b
            c--;//dec c
            }while(true);//jnz 1 -4
            //b:;//jnz 0 0
            std::cout<<b<<' '<<std::flush;if ((was0&&b!=1)||(!was0&&b!=0))return false;was0=(b==0);//std::cout<<b<<' '<<std::flush;//;out b
        } while (a);//jnz a -19
    } while (true);//jnz 1 -21
}

int main(){
    for(size_t i=0;i<(1ull<<63);i++)
        if(!new_main(i))std::cout<<i<<" does not work"<<std::endl;else std::cout<<i<<" is ok "<<std::endl;
    return 0;
}
