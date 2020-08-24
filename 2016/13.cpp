#include <iostream>
#include <fstream>
#include <vector>

constexpr bool even_number_of_digits(unsigned long input) {
    bool rtr = !(input%2);
    while(input/=2)if(input%2)rtr=!rtr;
    return rtr;
}

constexpr bool is_open(const unsigned long x,const unsigned long y,const unsigned long input) {
    return even_number_of_digits(x*(x+3+2*y)+y*(y+1) + input);
}

struct field_element{
    bool is_wall;
    unsigned steps=0;
};

struct flower {
    unsigned x,y,steps;
    bool is_flowing=true;
    flower(unsigned _x,unsigned _y, unsigned _steps){x=_x;y=_y;steps=_steps;}
};

int main(int argc,char**argv) {
    if(argc<2){std::cout<<"USAGE: {program} {input}"<<std::endl;return 0;}
    unsigned long input = std::stoi(argv[1]);
    const size_t field_size=512;
    field_element field[field_size][field_size];
    std::vector<flower> flowers={flower(1,1,0)};
    for(size_t i=0;i<field_size;i++)for(size_t j=0;j<field_size;j++)field[i][j].is_wall=!is_open(i,j,input);
    const std::vector<std::pair<int,int>> directions = {std::make_pair(1,0),std::make_pair(0,1),std::make_pair(-1,0),std::make_pair(0,-1)};
    bool flowing = true,answered=false;;
    while(flowing) {
        flowing = false;
        size_t cur_size = flowers.size();
        for(size_t i=0;i<cur_size;i++){
            // Let the flowing flowers flow.
            if(flowers[i].is_flowing) {
                flowing = true;
                int dx=0,dy=0;
                if(flowers[i].x==31&&flowers[i].y==39&&!answered){std::cout<<"the answer to part 1 is "<<flowers[i].steps<<std::endl;answered=true;}
                for(auto a:directions)
                    if (flowers[i].x+a.first < field_size && flowers[i].y+a.second < field_size && !field[flowers[i].x+a.first][flowers[i].y+a.second].is_wall) {
                        if(!dx && !dy){
                            dx=a.first;dy=a.second;
                        } else {
                            flowers.emplace_back(flowers[i].x+a.first,flowers[i].y+a.second,flowers[i].steps+1);
                        }
                    }
                field[flowers[i].x][flowers[i].y].is_wall=true;
                field[flowers[i].x][flowers[i].y].steps=flowers[i].steps;
                if(dx||dy) {
                    flowers[i].steps++;flowers[i].x+=dx;flowers[i].y+=dy;
                } else{flowers[i].is_flowing = false;}
            }
        }
    }
    unsigned int cnt_50_steps;
    for(size_t j=0;j<field_size;j++)for(size_t i=0;i<field_size;i++)if(field[i][j].steps&&field[i][j].steps<=50)cnt_50_steps++;

    std::cout<<"the answer to part 2 is "<<cnt_50_steps+1<<std::endl;//We missed 1,1
    return 0;
}
