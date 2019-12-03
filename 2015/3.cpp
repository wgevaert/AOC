#include <iostream>

int main() {
    char a;
    const int ox=1025,oy=1025;
    int px=ox,py=oy,rx=ox,ry=oy;
    long unsigned total = 1;
    bool field[2*ox][2*oy], odd=false;
    field[ox][oy]=true;
    while ((a=std::cin.get()) == '<' || a=='^' || a=='>' || a=='v') {
        if (odd) {// santa moves
            switch (a) {
                case '<': --px;break;
                case '^': ++py;break;
                case '>': ++px;break;
                case 'v': --py;break;
                default: std::cout<<"HUH"<<std::endl;
            }
            if (!field[px][py]){std::cout<<px<<' '<<py<<std::endl;total++;}
            field[px][py]=true;
        } else {// robo-santa moves
            switch (a) {
                case '<': --rx;break;
                case '^': ++ry;break;
                case '>': ++rx;break;
                case 'v': --ry;break;
                default: std::cout<<"HUH"<<std::endl;
            }
            if (!field[rx][ry]){std::cout<<rx<<' '<<ry<<std::endl;total++;}
            field[rx][ry]=true;
        }
        odd = !odd;
    }
    std::cout<<total;
    return 0;
}
