#include <iostream>
#include <fstream>
#include <vector>

struct grid_node{
    unsigned size,
    used,
    avail,
    use_perc,
    is_goal=false;
};

const size_t grid_size = 50;

bool can_move(grid_node a,grid_node b){
   return a.used > 0 && a.used <= b.avail;
}

bool could_move(grid_node a,grid_node b){
   return a.used > 0 && a.used <= b.size;
}

void print(size_t x_max, size_t y_max, grid_node grid[][grid_size], bool (*move)(grid_node,grid_node)) {
    for(size_t a=0;a<y_max;a++){
        for(size_t b=0;b<x_max;b++){
            std::cout<<(grid[b][a].is_goal ? 'G' : (grid[b][a].used >= 100 ? '@' : (grid[b][a].used >= 50 ? '#' : '_')));
            if(b<x_max -1){
                std::cout<<' '<<(move(grid[b+1][a],grid[b][a])  ? '<' : ' ')<<(move(grid[b][a],grid[b+1][a])?'>':' ')<<' ';
            }
        }
        std::cout<<std::endl;
        if(a<y_max -1) {
            for(size_t b=0;b<x_max;b++) {
                std::cout<<(move(grid[b][a], grid[b][a+1]) ? 'v' : ' ')<<(move(grid[b][a+1], grid[b][a]) ? '^' : ' ')<<"   ";
            }
            std::cout<<std::endl;
        }
    }
}

void move_data(grid_node& a,grid_node &b) {
    a.used = b.used;
    a.is_goal = b.is_goal;
    b.is_goal = false;
    b.used = 0;
    b.avail = b.size;
    a.avail=a.size-a.used;
}

int main(int argc, char**argv){
    if(argc<2){std::cout<<" usage: {program} {input_file}"<<std::endl;return 0;}

    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad "<<std::endl;return 0;}

    while(input_file.get()!='\n'&&!input_file.eof());//two boring lines are eaten
    while(input_file.get()!='\n'&&!input_file.eof());

    grid_node grid[grid_size][grid_size];
    size_t x,y,x_max,y_max;
    char a;
    for(;;) {
        while(input_file.get()!='x'&&!input_file.eof());
        if(input_file.eof())break;
        input_file>>x;
        if(input_file.get()!='-'||input_file.get()!='y'){std::cout<<":("<<std::endl;return 1;}
        input_file>>y;
        if(x>x_max)x_max=x;
        if(y>y_max)y_max=y;
        while(input_file.get()==' ');
        input_file.unget();
        input_file>>grid[x][y].size;
        if((a=input_file.get())!='T'){std::cout<<x<<','<<y<<" HAS A DIFFERENT SIZE UNIT "<<grid[x][y].size<<' '<<a<<std::endl;return 1;}
        while(input_file.get()==' ');
        input_file.unget();
        input_file>>grid[x][y].used;
        if(input_file.get()!='T'){std::cout<<x<<','<<y<<" HAS A DIFFERENT USED UNIT"<<std::endl;return 1;}
        while(input_file.get()==' ');
        input_file.unget();
        input_file>>grid[x][y].avail;
        if(input_file.get()!='T'){std::cout<<x<<','<<y<<" HAS A DIFFERENT AVAIL UNIT"<<std::endl;return 1;}
        while(input_file.get()==' ');
        input_file.unget();
        input_file>>grid[x][y].use_perc;
        if(input_file.get()!='%'||input_file.get()!='\n'){std::cout<<x<<','<<y<<" HAS A DIFFERENT PERC UNIT"<<std::endl;return 1;}
    }input_file.close();
    grid[x_max][0].is_goal = true;
    x_max++;y_max++;
    unsigned cnt=0, moves = 0;
    for(size_t a=0;a<x_max;a++)for(size_t b=0;b<y_max;b++)for(size_t c=0;c<x_max;c++)for(size_t d=0;d<y_max;d++)if((a!=c||b!=d)&&grid[a][b].used>0&&grid[a][b].used<=grid[c][d].avail)cnt++;
    for(size_t a=0;a<x_max;a++)for(size_t b=0;b<y_max;b++)if(!grid[a][b].used){x=a;y=b;}
    print(x_max,y_max,grid,could_move);
    do {
        std::cout<<"After "<<moves<<" moves, current position is "<<x<<','<<y<<"\nWhat should we do?\n print c(a)n, print c(o)uld, move (u)p, move (d)own, move (r)ight, move (l)eft, (q)uit"<<std::endl;
        do {a=std::cin.get();}while(a=='\n');
        switch (a) {
            case 'a':print(x_max,y_max,grid,can_move);break;
            case 'o':print(x_max,y_max,grid,could_move);break;
            case 'u':if(y==0 || !can_move(grid[x][y-1],grid[x][y]) ){std::cout<<"Cannot move up"<<std::endl;break;}move_data(grid[x][y],grid[x][y-1]);y--;moves++;break;
            case 'd':if(y>=y_max || !can_move(grid[x][y+1],grid[x][y]) ){std::cout<<"Cannot move down"<<std::endl;break;}move_data(grid[x][y],grid[x][y+1]);y++;moves++;break;
            case 'l':if(x==0 || !can_move(grid[x-1][y],grid[x][y]) ){std::cout<<"Cannot move left"<<std::endl;break;}move_data(grid[x][y],grid[x-1][y]);x--;moves++;break;
            case 'r':if(x>=x_max-1 || !can_move(grid[x+1][y],grid[x][y]) ){std::cout<<"Cannot move right"<<std::endl;break;}move_data(grid[x][y],grid[x+1][y]);x++;moves++;break;
        }
    } while(a!='q'&&!grid[0][0].is_goal);
    std::cout<<cnt<<' '<<moves<<std::endl;
    return 0;
}
