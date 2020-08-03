#include <iostream>
#include <string>

const unsigned mid=256,size = 2*mid+1;
const long unsigned max=-1,min=0;



int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {input}"<<std::endl;
        return 0;
    }
    unsigned long long input = std::stoull(argv[1]);
    std::cout<<max<<' '<<input<<std::endl;
    long unsigned field[size][size];
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++)
            field[i][j]=min;
    unsigned cur_x=mid,cur_y=mid;
    field[cur_x][cur_y]=1;
    cur_x++;
    field[cur_x][cur_y]=1;

    while (field[cur_x][cur_y] < input) {
        if (field[cur_x-1][cur_y] != min && field[cur_x][cur_y-1] == min) {
            cur_y--;
        } else if(field[cur_x][cur_y+1] != min && field[cur_x-1][cur_y] == min) {
            cur_x--;
        } else if(field[cur_x+1][cur_y] != min && field[cur_x][cur_y+1] == min) {
            cur_y++;
        } else {
            cur_x++;
        }
        field[cur_x][cur_y]=
            field[cur_x+1][cur_y] +
            field[cur_x+1][cur_y+1] +
            field[cur_x][cur_y+1] +
            field[cur_x-1][cur_y+1] +
            field[cur_x-1][cur_y] +
            field[cur_x-1][cur_y-1] +
            field[cur_x][cur_y-1] +
            field[cur_x+1][cur_y-1];
        std::cout<<cur_x<<','<<cur_y<<':'<<field[cur_x][cur_y]<<std::endl;
    }
    std::cout<<"ANSWER: "<<field[cur_x][cur_y]<<std::endl;

    return 0;
}
