#include <iostream>
#include <fstream>
#include <vector>

const size_t field_size = 512;
const std::pair<short,short> directions[4]={std::make_pair(-1,0),std::make_pair(0,-1),std::make_pair(1,0),std::make_pair(0,1)};

struct field_element {
    bool is_wall;
    short number=-1;
    unsigned *seen_by;
    unsigned long numberofflowers=0;
};

struct flower {
    unsigned seen,x,y;// all have seen 1;
    unsigned long steps;
    void see(short i){
        seen |=(1u<<i);
    }
    flower(unsigned long _steps, unsigned _x,unsigned _y,unsigned _seen){seen=_seen;steps=_steps;x=_x;y=_y;}
};

char get_char(unsigned long numb){
    return ((numb-1)%26)+'a';
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    field_element field[field_size][field_size];
    size_t x=0,y=0,x_max=0,y_max=0;
    short max_number = 0;
    while(!input_file.eof()) {
        char a=input_file.get();
        if (a=='#')field[x][y].is_wall = true;
        else if(a=='.')field[x][y].is_wall = false;
        else if('0'<=a&&a<='9'){field[x][y].number=a-'0';field[x][y].is_wall=false;if(a-'0'>max_number)max_number=a-'0';}
        else if(a=='\n'){x=-1;y=++y_max;}
        else if(!input_file.eof()){std::cout<<a<<" was unexpected"<<std::endl;return 1;}
        x++;
        if(x > x_max)x_max=x;
    }
    max_number++;
    for(size_t b=0;b<y_max;b++){
        for(size_t a=0;a<x_max;a++){
//            std::cout<<(field[a][b].is_wall ? ' ' : (field[a][b].number ==-1 ? '.' : static_cast<char>(field[a][b].number + '0')));
            if(!field[a][b].is_wall) {
                field[a][b].seen_by=new unsigned[1ul<<max_number];
                for(size_t i=0;i<(1ul<<max_number);i++)field[a][b].seen_by[i]=0;
                if(field[a][b].number == 0){x=a;y=b;}
            }
        }
//        std::cout<<std::endl;
    }

    std::vector<flower> flowers[2];
    flowers[0].emplace_back(0,x,y,0);
    unsigned short p=0,q=1;
    unsigned goal = 0;for(int i=1;i<max_number;i++)goal|=(1ull<<i);
//    std::cout<<x_max<<','<<y_max<<','<<x<<','<<y<<','<<max_number<<','<<(1ul<<max_number)<<','<<goal<<std::endl;
    bool answered[2]={false,false};
    for(;;){
        for(size_t b=1;b<y_max;b++){for(size_t a=0;a<x_max;a++){std::cout<<(field[a][b].is_wall ? ' ' : (field[a][b].numberofflowers ? get_char(field[a][b].numberofflowers) : (field[a][b].number ==-1 ? '.' : static_cast<char>(field[a][b].number + '0'))));field[a][b].numberofflowers=0;}std::cout<<std::endl;}
        if(!flowers[p].size())break;
        while(flowers[p].size()) {
            flower a=flowers[p].back();flowers[p].pop_back();
            for(auto b:directions) {
                const unsigned new_x = a.x+b.first, new_y=a.y+b.second;
                if (new_x < field_size && new_y < field_size && !field[new_x][new_y].is_wall && !field[new_x][new_y].seen_by[a.seen]) {
                    field[new_x][new_y].numberofflowers++;
                    flowers[q].emplace_back(a.steps+1,new_x,new_y,a.seen);
                    if (1<=field[new_x][new_y].number) {
                        field[new_x][new_y].seen_by[flowers[q].back().seen]=flowers[q].back().steps;
                        flowers[q].back().see(field[new_x][new_y].number);
                        if(flowers[q].back().seen == goal){
                            if(!answered[0]){std::cout<<flowers[q].back().steps<<" is the answer to 1"<<std::endl;answered[0]=true;}
                            flowers[q].back().see(0);
                        }
                    } else if(field[new_x][new_y].number == 0 && (flowers[q].back().seen & 1u) && !answered[1]) {
                        answered[1]=true;
                        std::cout<<flowers[q].back().steps<<" is the answer to 2"<<std::endl;
                    }
                    field[new_x][new_y].seen_by[flowers[q].back().seen]=flowers[q].back().steps;
                }
            }
        }
        p=1-p;q=1-q;
    }

    unsigned cur_seen = goal | 1u;
    field[x][y].seen_by[0] = 0;
    do {
        for(size_t b=1;b<y_max;b++){
            for(size_t a=0;a<x_max;a++)
                std::cout<<(field[a][b].is_wall ? ' ' : (a==x&&b==y ? 'X' : (field[a][b].number ==-1 ? '.' : static_cast<char>(field[a][b].number + '0'))));
            std::cout<<std::endl;
        }
        for(auto a:directions) {
            unsigned new_x=x+a.first,new_y=y+a.second;
            if(!field[new_x][new_y].is_wall && field[new_x][new_y].seen_by[cur_seen] < field[x][y].seen_by[cur_seen]) {
                x=new_x;y=new_y;
                if(field[x][y].number !=-1){
                    cur_seen &= ~(1u<<field[x][y].number);
                    cur_seen &= ~(1u);
                }
                break;
            }
        }
    }while(cur_seen || field[x][y].number !=0);

    return 0;
}
