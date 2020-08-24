#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <bitset>

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
    bool flowing = true;
    unsigned px,py,length=0,name=0,depth;
    unsigned keys=0;
    flower(int pos_x,int pos_y,int depth) {px=pos_x;py=pos_y;flowing=true;keys = depth;}
    flower(){};
};

bool is_walkable(char a) {
	return a=='.'||(a<='z' && 'a'<=a)||(a<='Z' && 'A'<=a);
}

std::vector<std::pair<int,int>> walk_back(std::unordered_map<uint32_t,int> maze[][82], char input[][200], flower myflowers[], int px, int py, int x_max, int y_max, uint32_t keys) {
	for (int j=0;j<y_max;j++){
		 for(int i=0;i<y_max;i++)
			 std::cout<<(maze[i][j].find(keys) != maze[i][j].end() ? 
						 std::to_string(maze[i][j].at(keys)) :
						 std::string("   ") + std::string(1,(input[i][j] == '#' ? ' ' : input[i][j])));
		 std::cout<<std::endl;
	}
	std::vector<std::pair<int,int>> rtr = {{px,py}};
	int cur_size=maze[px][py].at(keys);
	while (cur_size > 2) {
		uint32_t old_keys = keys;
		if (input[px][py]<='z' && input[px][py]>='a') {
			uint32_t new_keys = keys & ~(1u << (input[px][py] - 'a'));
			if (maze[px][py].find(new_keys) == maze[px][py].end())
			    std::cout<<" Already picked up "<<input[px][py]<<std::endl;
			else {
				std::cout<<" Picked up "<<input[px][py]<<" only just now"<<std::endl;
			    keys =new_keys;
			}
		}
		if (maze[px][py].find(keys) == maze[px][py].end()){
			std::cout<<"AN UNKNOWN ERROR OCCURED!"<<std::endl;
			return rtr;
		}
		cur_size = maze[px][py].at(keys);
		std::cout<<cur_size<<' '<<std::flush;
		bool direction_found = false;
		int dx,dy;
		for (int i=1;i<=4;i++) {
			get_direction(i,dx,dy);
			if (maze[px+dx][py+dy].find(keys) != maze[px+dx][py+dy].end() && maze[px+dx][py+dy].at(keys) <=cur_size) {
				direction_found = true;
				break;
			}
		}
		if (!direction_found) {
			std::cout<<"A DIFFERENT UNKNOWN ERROR OCCURED! Could not find less than "<<maze[px][py].at(keys)<<std::endl;
			if (old_keys == keys)
				std::cout<<"NO NEW KEY PICKED UP JUST YET"<<std::endl;
			else {
			    std::cout<<"PICKED UP A NEW KEY"<<std::endl;
			    for(int j=0;j<y_max;j++){
			        for(int i=0;i<x_max;i++)
			            std::cout<<(i==px && j==py ? "@@@@" : 
			                (maze[i][j].find(old_keys) != maze[i][j].end() ? 
    						 (maze[i][j].at(old_keys) < 10 ? "   " : (maze[i][j].at(old_keys) < 100 ? "  " : (maze[i][j].at(old_keys) < 1000 ? " " : ""))) + std::to_string(maze[i][j].at(old_keys)) :
	    					 std::string("   ") + std::string(1,(input[i][j] == '#' ? ' ' : input[i][j]))));
		    	    std::cout<<std::endl;
				}
			}
			for(int j=0;j<y_max;j++){
				for(int i=0;i<x_max;i++)
					std::cout<<(i==px && j==py ? "@@@@" : 
						(maze[i][j].find(old_keys) != maze[i][j].end() ? 
						 (maze[i][j].at(old_keys) < 10 ? "   " : (maze[i][j].at(old_keys) < 100 ? "  " : (maze[i][j].at(old_keys) < 1000 ? " " : ""))) + std::to_string(maze[i][j].at(old_keys)) :
						 std::string("   ") + std::string(1,(input[i][j] == '#' ? ' ' : input[i][j]))));
				std::cout<<std::endl;
			}
			return rtr;
		}
		rtr.push_back({px+=dx,py+=dy});
	}
	
	
	return rtr;
}

int  main() {
    char input[200][200];
    std::string line;
    int j=0,x_a,y_a;
    unsigned x_max=0,y_max=0;
    while (true) {
        std::getline(std::cin,line);
        if(line.size()==0)
            break;
        if (x_max < line.size()) {
            x_max=line.size();
        }
        for (int i=0;i<line.size();i++) {
            input[i][j]=line[i];
            if (line[i]=='@') {
                x_a=i;y_a=j;
            }
        }
        j++;
        if (y_max < j)
            y_max=j;
    }
    for(unsigned j=0;j<y_max;j++){for(unsigned i=0;i<x_max;i++)std::cout<<(input[i][j]=='#'?' ':(input[i][j]=='.'?'%':input[i][j]));std::cout<<std::endl;}
    
    const int flower_nr = 4000000;
    std::unordered_map<uint32_t,int> maze[82][82];
    flower *myflowers = new flower[flower_nr];
    bool flowed,flowing=true;
    int flower_cnt=1,px,py;
    unsigned depth;
    myflowers[0]=flower(x_a,y_a,0);
    int cnt=0,dx,dy;
    std::vector<std::pair<int, int>> optimal_path;
    // Make a tree with the code from 15.2
    while (flowing) {
        flowing = false;
        int dummy=flower_cnt;
        for (int flwr=0;flwr<dummy;flwr++) {
            if (!myflowers[flwr].flowing){
                continue;
            }
            px=myflowers[flwr].px;
            py=myflowers[flwr].py;
            depth = myflowers[flwr].keys;
            flowed = false;
            for (int i=1;i<=4;i++) {
                get_direction(i,dx,dy);
                if (maze[px+dx][py+dy].find(depth) == maze[px+dx][py+dy].end() && is_walkable(input[px+dx][py+dy])) {
                     unsigned new_keys = myflowers[flwr].keys;
                     if ('A'<=input[px+dx][py+dy]&&input[px+dx][py+dy]<='Z') {
						if (!((myflowers[flwr].keys >> static_cast<int>(input[px+dx][py+dy] - 'A') ) %2)) {
							std::cout<<input[px+dx][py+dy]<<" DOES NOT OCCUR IN "<<std::bitset<26>(myflowers[flwr].keys)<<std::endl;
                            continue;
						} else {
							std::cout<<input[px+dx][py+dy]<<" OCCURS IN "<<std::bitset<26>(myflowers[flwr].keys)<<std::endl;
						}
                     }
                     else if ('a'<=input[px+dx][py+dy]&&input[px+dx][py+dy]<='z') {
						 unsigned key_nr = input[px+dx][py+dy] - 'a';
						 new_keys |= (1u << key_nr);
						 if (new_keys == ((1u << 26) - 1)) {
							 //found = true;
							 std::cout<<" PROGRAM FINISHED CORRECTLY"<<std::endl;
							 std::cout<<myflowers[flwr].length<<std::endl;
							 optimal_path = walk_back(maze,input,myflowers,px,py,x_max,y_max,myflowers[flwr].keys);
						     std::cout<<"OPTIMAL PATH:\n{";
						     for (auto i : optimal_path) std::cout<<'{'<<i.first<<','<<i.second<<"}, ";
						     std::cout<<'}'<<std::endl;
						    
						     
							 
                             delete[] myflowers;
							 return 0;
						 }
						     
                    }
                    if (!flowed) {
                        myflowers[flwr].px = px+dx;
                        myflowers[flwr].py = py+dy;
                        myflowers[flwr].length++;
                        maze[px+dx][py+dy][new_keys] = maze[px+dx][py+dy][myflowers[flwr].keys] = myflowers[flwr].length;
                        myflowers[flwr].keys = new_keys;
                    } else {
                        myflowers[flower_cnt]=flower(px+dx,py+dy,new_keys);
                        maze[px+dx][py+dy][new_keys] = maze[px+dx][py+dy][myflowers[flwr].keys] = myflowers[flwr].length;
                        myflowers[flower_cnt].length = myflowers[flwr].length;
                        myflowers[flower_cnt].name = flower_cnt;
                        myflowers[flwr].keys = new_keys;
                        flower_cnt++;
                        flowing = true;
                    }
                    flowed = true;
                }
            }
            if (!flowed) {
                myflowers[flwr].flowing = false;
            }
            if (myflowers[flwr].flowing) {
                flowing = true;
            }
        }
        if (!(cnt%100)) {
			dummy=0;
			for (int i=0;i<flower_cnt;i++)if (myflowers[i].flowing)dummy++;
			if (dummy == 0) {
				for (int k = 0;k<(1<< 26);k++){
					int total = 0;
					for(int i=0;i<x_max;i++)for(int j=0;j<y_max;j++)if (maze[i][j].find(k)!=maze[i][j].end())total++;
					if (total > 5) {
						for (int j=0;j<y_max;j++){for(int i=0;i<x_max;i++)std::cout<<(maze[i][j].find(k)!=maze[i][j].end() ? 'O' : input[i][j]);std::cout<<std::endl;}
					}
				}
				
			}
			//for(int j=0;j<y_max;j++){for (int i=0;i<x_max;i++)std::cout<<(maze[i][j].find(1<<) == maze[i][j].end() ? input[i][j] : 'O');std::cout<<std::endl;}
			std::cerr<<'\r'<<flower_cnt<<' '<<dummy<<' '<<cnt<<std::flush;
		}
        ++cnt;
    }
    delete[] myflowers;
    //int walker_x = x_a,walker_y=y_a,total_steps = 0, key_cnt=0;
    //std::cout<<"ANSWER: "<<total_steps<<'='<<(22+104*2+(118-2)*2+62*2+134*2+(86-58)*2+184*2+(242-122)*2+(70-16)*2+200*2+(258-194)*2+272*2+(212-186)*2+(164-110)*2+290*2+400)<<std::endl;

    return 1;
}
