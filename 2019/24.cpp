#include <iostream>
#include <unordered_map>
#include <string>

const int size_x=5,size_y=5;

bool get_tile(int x,int y,uint32_t board) {
    if (x>=size_x || y >=size_y || x<0 || y<0)
        return 0;
    return (board >> (size_x*y+x))%2;
}

void set_tile(int x,int y,uint32_t & board,bool value) {
    if (value) {
         board |= (1u<<(size_x*y+x));
    } else {
        board &= ~(1u<<(size_x*y+x));
    }
}

uint32_t read() {
    std::string line;
    uint32_t rtr=0;
    std::getline(std::cin,line);
    int x,y=0;
    while(line.size()) {
        x=0;
        for (char a : line) {
            set_tile(x,y,rtr,a=='#');
            x++;
        }
        y++;
        std::getline(std::cin,line);
    }
    return rtr;
}

int main() {
    uint32_t board = read(),new_board;
    int steps = 0;
    std::unordered_map<uint32_t,int> prev_positions;
    while (prev_positions.find(board) == prev_positions.end()) {
        prev_positions[board] = steps;
        std::cout<<"\nAFTER "<<steps<<" STEPS:"<<std::endl;
        for(int j=0;j<size_y;j++) {
            for (int i=0;i<size_x;i++) {
                int neighbours = get_tile(i+1,j,board) + get_tile(i,j-1,board) + get_tile(i,j+1,board) + get_tile(i-1,j,board);
                if (neighbours == 1)
                    set_tile(i,j,new_board,true);
                else if (neighbours == 2 && !get_tile(i,j,board))
                    set_tile(i,j,new_board,true);
                else set_tile(i,j,new_board,false);
                std::cout<<(get_tile(i,j,board) ? '#' : '.');
            }std::cout<<std::endl;
        }
        board = new_board;
        steps++;
    }
    std::cout<<"THE FOLLOWING LAYOUT OCCURED TWICE:"<<std::endl;
    for(int j=0;j<size_y;j++){for(int i=0;i<size_x;i++)std::cout<<(get_tile(i,j,board) ? '#' : '.');std::cout<<std::endl;}
    std::cout<<board<<std::endl;
    return 0;
}
