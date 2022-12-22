#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>/*
#include <unordered_map>*/

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

void read_or_die(std::string pattern, std::istream& input) {
    for (auto a:pattern) {
        if (a != input.get()) {
            std::string parsed_pattern = "";
            for (auto b:pattern) {
                if (b < ' ' || b > 126)
                    parsed_pattern += '?';
                else
                    parsed_pattern += b;
            }
            std::cerr<<'\''<<parsed_pattern<<'\''<<" expected"<<std::endl;
            exit(1);
        }
    }
}

void turn_right(int& dx, int& dy) {
    if (dx==0) {
        dx = -1*dy;
        dy = 0;
    } else {
        dy = dx;
        dx = 0;
    }
}
void turn_left(int& dx, int& dy) {
    if (dx==0) {
        dx = dy;
        dy = 0;
    } else {
        dy = -1*dx;
        dx = 0;
    }
}
int turn_to_val(int dx, int dy) {
    if (dx==0) {
        if (dy<0)return 3;
        else return 1;
    } else if (dx<0)return 2;
    return 0;
}


bool walk(int& x, int& y, int& dx, int& dy, std::vector<std::string> field) {
    int oldX=x, oldY=y;
    if (dx==0) {
        if (dy<0) {
            if (y==0) y=field.size()-1;
            else y--;
            while (x>=field[y].size()||field[y][x]==' ')y=(y+field.size()-1)%field.size();
        } else {
            if (y==field.size()-1)y=0;
            else y++;
            while(x>=field[y].size()||field[y][x]==' ')y=(y+1)%field.size();
        }
    } else if (dx<0) {
        if (x==0)x=field[y].size()-1;
        else x--;
        while(field[y][x]==' ')x=(x+field[y].size()-1)%field[y].size();
    } else {
        if (x==field[y].size()-1)x=0;
        else x++;
        while(field[y][x]==' ')x=(x+1)%field[y].size();
    }
    if(field[y][x]=='#'){x=oldX;y=oldY;return false;}
    return true;
}
bool walk2(int& x, int& y, int& dx, int& dy, std::vector<std::string> field) {
    int oldX=x, oldY=y, oldDx=dx, oldDy=dy;
    if (dx==0) {
        if (dy<0) {// Walking up
            if (y==0||field[y-1][x]==' '){
                if (x<50) {
                    if (y!=100){std::cerr<<"Expected y=100 but got "<<y<<std::endl;exit(1);}
                    dy=0;dx=1;
                    x=50;
                    y=50+oldX;
                } else if (x<100) {
                    if (y!=0){std::cerr<<"Expected y=0 but got "<<y<<std::endl;exit(1);}
                    dy=0;dx=1;
                    x=0;
                    y=100+oldX;
                } else {
                    if (y!=0){std::cerr<<"Expected y=0 but got "<<y<<std::endl;exit(1);}
                    y=199;
                    x-=100;
                }
            }
            else y--;
        } else {
            if (y==field.size()-1||field[y+1].size()<=x) {// Walking down
                if (x<50) {
                    if (y!=199){std::cerr<<"Expected y=199 but got "<<y<<std::endl;exit(1);}
                    x+=100;
                    y=0;
                } else if (x<100) {
                    if (y!=149){std::cerr<<"Expected y=149 but got "<<y<<std::endl;exit(1);}
                    dy=0;dx=-1;
                    x=49;
                    y=100+oldX;
                } else {
                    if (y!=49){std::cerr<<"Expected y=49 but got "<<y<<std::endl;exit(1);}
                    dy=0;dx=-1;
                    x=99;
                    y=oldX-50;
                }
            }
            else y++;
        }
    } else if (dx<0) {
        if (x==0||field[y][x-1]==' ') {
            if (y<50) {
                if (x!=50){std::cerr<<"Expected x=50 but got "<<x<<','<<y<<std::endl;exit(1);}
                dx=1;
                x=0;
                y=149-oldY;
            } else if (y<100) {
                if (x!=50){std::cerr<<"Expected x=50 but got "<<x<<','<<y<<'.'<<dx<<','<<dy<<std::endl;exit(1);}
                dx=0;dy=1;
                x=oldY-50;
                y=100;
            } else if (y<150) {
                if (x!=0){std::cerr<<"Expected x=0 but got "<<x<<std::endl;exit(1);}
                dx=1;
                x=50;
                y=149-oldY;
            } else {
                if (x!=0){std::cerr<<"Expected x=0 but got "<<x<<std::endl;exit(1);}
                dx=0;dy=1;
                x=oldY-100;
                y=0;
            }
        }
        else x--;
    } else {
        if (x==field[y].size()-1) {
            if (y<50) {
                if (x!=149){std::cerr<<"Expected x=149 but got "<<x<<std::endl;exit(1);}
                dx=-1;
                x=99;
                y=149-oldY;
            } else if (y<100) {
                if (x!=99){std::cerr<<"Expected x=99 but got "<<x<<std::endl;exit(1);}
                dx=0;dy=-1;
                x=oldY+50;
                y=49;
            } else if (y<150) {
                if (x!=99){std::cerr<<"Expected x=99 but got "<<x<<std::endl;exit(1);}
                dx=-1;
                x=149;
                y=149-oldY;
            } else {
                if (x!=49){std::cerr<<"Expected x=49 but got "<<x<<std::endl;exit(1);}
                dx=0;dy=-1;
                x=oldY-100;
                y=149;
            }
        }
        else x++;
    }
    if(field[y][x]=='#'){x=oldX;y=oldY;dx=oldDx;dy=oldDy;return false;}
    return true;
} 

int real_main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr<<"Usage: "<<argv[0]<<" [-v {verbosity_level}] {input_file}"<<std::endl;
        exit(1);
    }
    verb_lvl = argc > 3 && argv[1][0] == '-' && argv[1][1] == 'v' ? std::stoul(argv[2]) : 0;

    std::ifstream input(argv[argc - 1]);

    if (!input.good()) {
        std::cerr<<"Input file "<<argv[argc - 1]<<" did not open correctly"<<std::endl;
        exit(1);
    }

    if (verb_lvl > 0) {
        std::cout<<"Running in verbosity mode "<<verb_lvl<<std::endl;
    }


    std::vector<std::string> field;
    std::string line;
    std::getline(input,line);
    while(line!="") {
        field.emplace_back(line);
        std::getline(input,line);
    }

    int x=0,y=0,dx=1,dy=0,x1=0,y1=0,dx1=1,dy1=0;
    while(field[y][x]==' ')x++;
    while(field[y1][x1]==' ')x1++;
    while (std::isdigit(input.peek())) {
        u_t times;
        input>>times;
        for (u_t i=0;i<times;i++) {
            if(!walk(x1,y1,dx1,dy1,field))break;
        }
        for (u_t i=0;i<times;i++) {
            if(!walk2(x,y,dx,dy,field))break;
        }
        switch(input.get()) {
            case 'L':turn_left(dx1,dy1);turn_left(dx,dy);break;
            case 'R':turn_right(dx1,dy1);turn_right(dx,dy);break;
        }
    }
    input.close();

    std::cout<<(1000*(y1+1)+4*(x1+1)+turn_to_val(dx1,dy1))<<std::endl;
    std::cout<<(1000*(y+1)+4*(x+1)+turn_to_val(dx,dy))<<std::endl;
    // do things without input
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
