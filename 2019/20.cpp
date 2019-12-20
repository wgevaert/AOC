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
    int px,py,size;
    flower(int pos_x,int pos_y,int sze) {px=pos_x;py=pos_y;flowing=true;size = sze;}
    flower(){};
};

struct tile {
	char type,p1,p2;
	int dist=-1,x,y;
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
	tile robot_pos[300][300];
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
					portals.push_back({i,line+2});
				}
	            else if (x < x_max-2 && robot_pos[i+1][line].type <='Z' && robot_pos[i+1][line].type >='A' && robot_pos[i-1][line].type == PATH) {
					robot_pos[i-1][line].p1 = robot_pos[i][line].type;
					robot_pos[i-1][line].p2 = robot_pos[i+1][line].type;
					portals.push_back({i-1,line});
				}
				else if (x < x_max-3 && robot_pos[i+1][line].type <='Z' && robot_pos[i+1][line].type >='A' && robot_pos[i+2][line].type == PATH) {
					robot_pos[i+2][line].p1 = robot_pos[i][line].type;
					robot_pos[i+2][line].p2 = robot_pos[i+1][line].type;
					portals.push_back({i+2,line});
				}
				else if (line < y_max && robot_pos[i][line+1].type <='Z' && robot_pos[i][line+1].type >='A' && robot_pos[i][line-1].type == PATH) {
					robot_pos[i][line-1].p1 = robot_pos[i][line].type;
					robot_pos[i][line-1].p2 = robot_pos[i][line+1].type;
					portals.push_back({i,line-1});
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
				std::cout<< "Connecting "<<robot_pos[portals[i].first][portals[i].second].p1<<robot_pos[portals[i].first][portals[i].second].p2<<" on "<<portals[i].first<<','<<portals[i].second<<" to "<<portals[j].first<<','<<portals[j].second<<std::endl;
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

    flower myflowers[2048];
    bool flowed;
    int flower_cnt=1,px,py,dx,dy;
    myflowers[0]=flower(x_start,y_start,0);
    unsigned cnt=0;
    while (robot_pos[x_end][y_end].type != USED) {
        int dummy=flower_cnt;
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
            flowed = false;
            for (int i=1;i<=4;i++) {
                get_direction(i,dx,dy);
                if (robot_pos[px+dx][py+dy].type==PATH || robot_pos[px+dx][py+dy].type==PORTAL) {
                    int new_x=px+dx,new_y=py+dy;
                    bool should_wait = false;
                    if (robot_pos[new_x][new_y].type==PORTAL) {
						new_x = robot_pos[px+dx][py+dy].x;
						new_y = robot_pos[px+dx][py+dy].y;
						should_wait=true;
					}
                    if (!flowed) {
                        myflowers[flwr].px = new_x;
                        myflowers[flwr].py = new_y;
                        myflowers[flwr].size++;
                        myflowers[flwr].waiting = should_wait;
                    } else {
						std::cout<<"SIZE: "<<myflowers[flwr].size;
                        myflowers[flower_cnt]=flower(new_x,new_y,myflowers[flwr].size);
                        std::cout<<','<<myflowers[flower_cnt].size<<std::endl;
                        myflowers[flower_cnt].waiting = should_wait;
                        flower_cnt++;
                    }
                    robot_pos[px+dx][py+dy].type = robot_pos[new_x][new_y].type = USED;
                    robot_pos[px+dx][py+dy].dist = myflowers[flwr].size;
                    flowed = true;
                }
            }
            if (!flowed) {
                myflowers[flwr].flowing = false;
            }
        }
        std::cout<<"\n\nAFTER "<<cnt+1<<" MINUTES:"<<std::endl;
        for(int j=0;j<=y_max;j++){
           for(int i=0;i<=x_max;i++){
               if (i==x_end && j==y_end)
                   std::cout<<'X';
               else
                   std::cout<<robot_pos[i][j].type;
           }std::cout<<std::endl;
        }
        ++cnt;
    }
    std::cout<<robot_pos[x_end][y_end].dist<<std::endl;
    return 0;
}

