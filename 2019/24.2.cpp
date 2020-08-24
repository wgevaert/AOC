#include <iostream>
#include <string>
#include <bitset>

const int size_x=5,size_y=5,mid_x=(size_x-1)/2,mid_y=(size_y-1)/2;

bool get_tile(int x,int y,uint32_t*boards,int index) {
    if (x>=size_x)
        return get_tile(mid_x+1,mid_y,boards,index-1);
    if (y >=size_y)
        return get_tile(mid_x,mid_y+1,boards,index-1);
    if (x<0)
        return get_tile(mid_x-1,mid_y,boards,index-1);
    if (y<0)
        return get_tile(mid_x,mid_y-1,boards,index-1);
    if(x==mid_x&&y==mid_y)return 0;
    return (boards[index] >> (size_x*y+x))%2;
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
    const int board_offset = 150;
    static uint32_t boards[2*board_offset];
    boards[board_offset] = read();
    static uint32_t new_boards[2*board_offset+1];
    int steps = 0,max_depth=0,min_depth=0,new_max_depth,new_min_depth;
    while (steps<200) {
        new_max_depth = max_depth;
        new_min_depth = min_depth;
        for(int j=0;j<size_y;j++) {
            for (int depth = min_depth-1;depth<=max_depth+1;depth++) {
                bool lives = false;
                for (int i=0;i<size_x;i++) {
                    int neighbours =
                        get_tile(i+1,j,boards,depth+board_offset) +
                        get_tile(i,j-1,boards,depth+board_offset) +
                        get_tile(i,j+1,boards,depth+board_offset) +
                        get_tile(i-1,j,boards,depth+board_offset);
                    if (i==mid_x) {
                        if (j==mid_y - 1) {
                            for (int k=0;k<size_y;k++)
                                neighbours += get_tile(k,0,boards,depth+1+board_offset);
                        } else if (j==mid_y+1) {
                            for (int k=0;k<size_y;k++)
                                neighbours += get_tile(k,size_y-1,boards,depth+1+board_offset);
                        } else if (j==mid_y)
                            continue;
                    }
                    else if (j==mid_y) {
                        if (i==mid_x - 1) {
                            for (int k=0;k<size_y;k++)
                                neighbours += get_tile(0,k,boards,depth+1+board_offset);
                        } else if (i==mid_x+1) {
                            for (int k=0;k<size_y;k++)
                                neighbours += get_tile(size_x-1,k,boards,depth+1+board_offset);
                        }
                    }
                    if (neighbours == 1 || (neighbours == 2 && !get_tile(i,j,boards,depth+board_offset))) {
                        set_tile(i,j,new_boards[depth+board_offset],true);
                        lives = true;
                    } else set_tile(i,j,new_boards[depth+board_offset],false);
                }
                if (depth==min_depth-1 && lives)
                    new_min_depth=min_depth-1;
                else if(depth==max_depth+1 && lives)
                    new_max_depth=max_depth+1;
            }
        }
        min_depth = new_min_depth;
        max_depth = new_max_depth;
        if (min_depth+board_offset <= 0 || max_depth >= board_offset) {
            std::cout<<"BOARD BUFFER TOO SMALL"<<std::endl;return 1;
        }
        for (int d=min_depth;d<=max_depth;d++){
           boards[d+board_offset] = new_boards[d+board_offset];
           new_boards[d+board_offset]=0;
        }
        steps++;
    }
    long long unsigned total_bugs = 0;
    std::cout<<"FINAL CONFIGURATION:"<<std::endl;
    for (int d=min_depth;d<=max_depth;d++) {std::cout<<"\n\nDEPTH "<<d<<std::endl;
        for (int j=0;j<size_y;j++){
            for (int i=0;i<size_x;i++)
                if(i==mid_x&&j==mid_y)std::cout<<'?';
                else if(get_tile(i,j,boards,d+board_offset)) {
                    total_bugs++;
                    std::cout<<'#';
                } else std::cout<<'.';
            std::cout<<std::endl;
        }
    }
    std::cout<<" TOTAL BUGS: "<<total_bugs<<std::endl;
    return 0;
}
