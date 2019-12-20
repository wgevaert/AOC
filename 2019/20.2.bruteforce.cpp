#include <iostream>
#include <vector>

#define NORTH 1
#define SOUTH 2
#define WEST 3
#define EAST 4

void get_direction(int command, int& dx, int & dy) {
    switch(command) {
        case NORTH: dx=0;dy=1;break;
        case SOUTH: dx=0;dy=-1;break;
        case WEST: dx=-1;dy=0;break;
        case EAST: dx=1;dy=0;break;
        default:std::cout<<command<<" IS NOT A VALID DIRECTION"<<std::endl;
    }
}

struct flower {
    bool flowing = true,waiting =false;
    int px,py,size,depth;
    flower(int pos_x,int pos_y,int sze) {px=pos_x;py=pos_y;flowing=true;size = sze;}
    flower(){};
};

struct tile {
	char type,p1,p2;
	int dist=-1,x,y;
	bool inner;
};

#define WALL '#'
#define PATH '.'
#define PORTAL 'p'
#define USED 'O'
#define NOTHING ' '

int main() {
	int x=0,x_max=0,y_max=0;
	int x_start,y_start,x_end,y_end;
	char a=std::cin.get();
	tile robot_pos[150][150];
	std::vector<int> lines_with_letters ={0};
	std::vector<std::pair<int,int>> portals={};
	while (!std::cin.eof()) {
		if (a!='\n') {
			if ('A'<=a&&a<='Z') {
				robot_pos[x][y_max].type = a;
				if (lines_with_letters.back() != y_max)lines_with_letters.push_back(y_max);
			} else if (a=='.') {
				robot_pos[x][y_max].type = a;
			} else if (a=='#') {
				robot_pos[x][y_max].type = a;
			} else if (a!=' ') {
				std::cout<<"UNEXPECTED CHAR: "<<a<<" (VALUE "<<static_cast<int>(a)<<")\n"<<std::flush;
			} else robot_pos[x][y_max].type = a;
			x++;
		} else {
			if (x>x_max)x_max = x;
			y_max++;
			x=0;
		}
		a=std::cin.get();
	}
	std::cout<<--x_max<<' '<<--y_max<<std::endl;
	for (int j=0;j<=y_max;j++) {
		for (int i=0;i<=x_max;i++) {
			std::cout<<robot_pos[i][j].type<<' ';
		}std::cout<<'\t'<<j<<"\n";
	}std::cout<<std::flush;
	for (int line : lines_with_letters) {
		for (int i=0;i<=x_max;i++) {
			if (robot_pos[i][line].type <= 'Z' && 'A' <= robot_pos[i][line].type) {
				if (line < y_max-3 && robot_pos[i][line+1].type <='Z' && robot_pos[i][line+1].type >='A' && robot_pos[i][line+2].type == PATH) {
					robot_pos[i][line+2].p1 = robot_pos[i][line].type;
					robot_pos[i][line+2].p2 = robot_pos[i][line+1].type;
					robot_pos[i][line+2].inner = (line > 3);
					portals.push_back({i,line+2});
//					std::cout<<'a'<<i<<','<<line+2<<':'<<robot_pos[i][line+2].inner<<std::endl;	
				}
	            else if (i < x_max && robot_pos[i+1][line].type <='Z' && robot_pos[i+1][line].type >='A' && robot_pos[i-1][line].type == PATH) {
					robot_pos[i-1][line].p1 = robot_pos[i][line].type;
					robot_pos[i-1][line].p2 = robot_pos[i+1][line].type;
					robot_pos[i-1][line].inner = (i < x_max-3);
					portals.push_back({i-1,line});
//					std::cout<<'b'<<i-1<<','<<line<<':'<<robot_pos[i-1][line].inner<<std::endl;
				}
				else if (i < x_max-1 && robot_pos[i+1][line].type <='Z' && robot_pos[i+1][line].type >='A' && robot_pos[i+2][line].type == PATH) {
					robot_pos[i+2][line].p1 = robot_pos[i][line].type;
					robot_pos[i+2][line].p2 = robot_pos[i+1][line].type;
					robot_pos[i+2][line].inner = (i + 2 > 3);
					portals.push_back({i+2,line});
//					std::cout<<'c'<<i+2<<','<<line<<':'<<robot_pos[i+2][line].inner<<std::endl;
				}
				else if (line < y_max && robot_pos[i][line+1].type <='Z' && robot_pos[i][line+1].type >='A' && robot_pos[i][line-1].type == PATH) {
					robot_pos[i][line-1].p1 = robot_pos[i][line].type;
					robot_pos[i][line-1].p2 = robot_pos[i][line+1].type;
					robot_pos[i][line-1].inner = (line + 2 < y_max - 3);
					portals.push_back({i,line-1});
//					std::cout<<'d'<<i<<','<<line-1<<':'<<robot_pos[i][line-1].inner<<std::endl;
				}
				robot_pos[i][line].type=NOTHING;
			}
		}
	}
	lines_with_letters.clear();
	int st,ed;
	for (int i=0;i<portals.size();i++){
		if (robot_pos[portals[i].first][portals[i].second].p1 == 'A' && robot_pos[portals[i].first][portals[i].second].p2 == 'A'){
			x_start = portals[i].first;
			y_start = portals[i].second;
			st = i;
		} else if(robot_pos[portals[i].first][portals[i].second].p1 == 'Z' && robot_pos[portals[i].first][portals[i].second].p2 == 'Z') {
			x_end = portals[i].first;
			y_end = portals[i].second;
			ed = i;
		} else for(int j=i+1;j<portals.size();j++)
		    if(robot_pos[portals[i].first][portals[i].second].p1 == robot_pos[portals[j].first][portals[j].second].p1 && robot_pos[portals[i].first][portals[i].second].p2 == robot_pos[portals[j].first][portals[j].second].p2) {
				std::cout<< "Connecting "<<(robot_pos[portals[i].first][portals[i].second].inner ? "inner":"outer")<<" portal "<<robot_pos[portals[i].first][portals[i].second].p1<<robot_pos[portals[i].first][portals[i].second].p2<<" on "<<portals[i].first<<','<<portals[i].second<<" to "<<(robot_pos[portals[j].first][portals[j].second].inner ? "inner":"outer")<<" portal "<<portals[j].first<<','<<portals[j].second<<std::endl;
				robot_pos[portals[i].first][portals[i].second].x = portals[j].first;
				robot_pos[portals[i].first][portals[i].second].y = portals[j].second;
				robot_pos[portals[j].first][portals[j].second].x = portals[i].first;
				robot_pos[portals[j].first][portals[j].second].y = portals[i].second;
				robot_pos[portals[i].first][portals[i].second].type = PORTAL;
				robot_pos[portals[j].first][portals[j].second].type = PORTAL;
		    }
	}
	for (int i=0;i<portals.size();i++) {
		if (i!=st && i!= ed && robot_pos[portals[i].first][portals[i].second].type != PORTAL) {
			std::cout<<"ERROR: "<<robot_pos[portals[i].first][portals[i].second].p1<<robot_pos[portals[i].first][portals[i].second].p2<<" ON "<< portals[i].first<<','<<portals[i].second<<" IS NOT CONNECTED"<<std::endl;
		}
	}
	std::cout<<"SEARCHING ROUTE FROM "<<x_start<<','<<y_start<<" TO "<<x_end<<','<<y_end<<std::endl;
	for (int j=0;j<y_max;j++) {
		for (int i=0;i<x_max;i++) {
			std::cout<<robot_pos[i][j].type<<' ';
		}std::cout<<"\n";
	}std::cout<<std::flush;

    const int layer_nr = 600, flowers_max = 1000000;
    flower *myflowers = new flower[flowers_max];
    auto layers = new tile[150][150][layer_nr];
    for (int i=0;i<=x_max;i++)for (int j=0;j<=y_max;j++)for(int k=0;k<layer_nr;k++)layers[i][j][k]=robot_pos[i][j];
    for (auto i :portals) if (!robot_pos[i.first][i.second].inner && !(i.first == x_start && i.second == y_start) && !(i.first == x_end && i.second == y_end)) layers[i.first][i.second][0].type = WALL;
    for (int i=1;i<layer_nr;i++) layers[x_start][y_start][i].type = layers[x_end][y_end][i].type = WALL;
    bool flowed, on_0;
    int flower_cnt=1,px,py,dx,dy,max_depth;
    myflowers[0]=flower(x_start,y_start,0);
    unsigned cnt=0;
    while (layers[x_end][y_end][0].type != USED) {
        int dummy=flower_cnt;
        on_0=false;
        for (int flwr=0;flwr<dummy;flwr++) {
            if (!myflowers[flwr].flowing){
                continue;
            }
            if (myflowers[flwr].waiting) {
				myflowers[flwr].waiting = false;
				myflowers[flwr].size++;
			}
            px=myflowers[flwr].px;
            py=myflowers[flwr].py;
            int depth = myflowers[flwr].depth;
            if (depth == 0)
            on_0=true;
            flowed = false;
            for (int i=1;i<=4;i++) {
                get_direction(i,dx,dy);
                if (layers[px+dx][py+dy][depth].type==PATH || layers[px+dx][py+dy][depth].type==PORTAL) {
                    int new_x=px+dx,new_y=py+dy,new_depth=depth;
                    bool should_wait = false;
                    if (layers[new_x][new_y][depth].type==PORTAL) {
						if (layers[new_x][new_y][depth].inner && !layers[layers[new_x][new_y][depth].x][layers[new_x][new_y][depth].y][depth].inner)
						    new_depth++;
						else if (!layers[new_x][new_y][depth].inner && layers[layers[new_x][new_y][depth].x][layers[new_x][new_y][depth].y][depth].inner)
						    new_depth--;
						if (new_depth > max_depth){max_depth=new_depth;std::cout<<max_depth<<" REACHED"<<std::endl;if(max_depth>=layer_nr-2){std::cout<<" LAYER BUFFER ALMOST EXHAUSTED"<<std::endl;return 1;}}
						new_x = layers[px+dx][py+dy][depth].x;
						new_y = layers[px+dx][py+dy][depth].y;
						should_wait=true;
					}
                    if (!flowed) {
                        myflowers[flwr].px = new_x;
                        myflowers[flwr].py = new_y;
                        myflowers[flwr].depth = new_depth;
                        myflowers[flwr].size++;
                        myflowers[flwr].waiting = should_wait;
                    } else {
                        myflowers[flower_cnt]=flower(new_x,new_y,myflowers[flwr].size);
                        myflowers[flower_cnt].waiting = should_wait;
                        myflowers[flower_cnt].depth = new_depth;
                        flower_cnt++;
                    }
                    layers[px+dx][py+dy][depth].type = layers[new_x][new_y][new_depth].type = USED;
                    layers[px+dx][py+dy][depth].dist = myflowers[flwr].size;
                    flowed = true;
                }
            }
            if (flower_cnt > flowers_max - 5){std::cout<<flower_cnt<<" Flowers spwaned, oops "<<std::endl;return 1;}
            if (!flowed) {
                myflowers[flwr].flowing = false;
            }
        }
        if (on_0) {
        std::cout<<"\n\nAFTER "<<cnt+1<<" MINUTES:"<<std::endl;
        for (int d=0;d<=0;d++){std::cout<<"LAYER "<<d<<std::endl;
        for(int j=2;j<=y_max;j++){
           for(int i=0;i<=x_max;i++){
               if (false && i==x_end && j==y_end)
                   std::cout<<'X';
               else
                   std::cout<<layers[i][j][d].type;
           }std::cout<<std::endl;
        }}}
        ++cnt;
    }
    std::cout<<layers[x_end][y_end][0].dist<<std::endl;
    delete[] layers;
    delete[] myflowers;
    return 0;
}

