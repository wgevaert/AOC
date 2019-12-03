#include <iostream>

int main() {
    const int ox=32768, oy=32768; // position of origin.
    long unsigned* field[2*ox];
    for (int i=0;i<2*ox;i++) {
        field[i]=new long unsigned[2*oy];
    }

    int px=ox, py=oy, step;
    long long unsigned min_score=2*ox*ox;
    long unsigned dist=0, score;
    char direction, dummy;
    do {
        direction = std::cin.get();std::cin>>step; // get input.
        for(int i=1;i<=step;i++) {
            switch(direction) {
                case 'U':++py;break;
                case 'D':--py;break;
                case 'R':++px;break;
                case 'L':--px;break;
                default: std::cout<<"WARNING: unknown direction "<<direction<<" encountered in first cable";for(int j=0;j<2*ox;j++)delete[] field[j];return 1;
            }
            if (px<0 || py<0 || px > 2*ox || py>2*oy) {
                std::cout<<"ERROR: bounding box too small in first cable";
for(int j=0;j<2*ox;j++)delete[] field[j];
return 1;
            }
            dist++;
            if (!field[px][py]) {
                field[px][py] = dist;
            }
        }
    } while((dummy = std::cin.get()) == ',');

    px=ox;py=oy;dist=0;
    do {
        direction = std::cin.get();std::cin>>step; // get input.
        for(int i=1;i<=step;i++) {
            switch(direction) {
                case 'U':++py;break;
                case 'D':--py;break;
                case 'R':++px;break;
                case 'L':--px;break;
                default: std::cout<<"WARNING: unknown direction "<<direction<<" encountered in second cable";for(int j=0;j<2*ox;j++)delete[] field[j];return 1;
            }
            if (px<0 || py<0 || px > 2*ox || py>2*oy) {
                std::cout<<"ERROR: bounding box too small in second cable";
for(int j=0;j<2*ox;j++)delete[] field[j];
return 1;
            }
            dist++;
            if (field[px][py] && (score = field[px][py]+dist) < min_score) {
                min_score = score;
            }
        }
    } while(std::cin.get() == ',');
    std::cout<<"Minimal score found: "<<min_score<<std::endl;
    for(int j=0;j<2*ox;j++)delete[] field[j];return 0;
}
