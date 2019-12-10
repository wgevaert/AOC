#include <iostream>
#include <algorithm>

#define ASTEROIDS in_sight[position_x][position_y]

const bool verbose = false;
const int init_rot = 1;

struct coord {
    int x;int y;coord(int a1,int b1){x=a1;y=b1;}coord(){x=y=0;}
};

int gcd(int a,int b) {
    if(b==0) return a;
    if(b==0)std::cerr<<"HELP"<<std::endl;
    return gcd(b, a%b);
}

int abs(int a){return a>0?a:-1*a;}

bool my_greater(int a,int b){
   return a>b ? 1:0;
}

bool my_less(int a,int b) {
   return a<b ? 1:0;
}

int get_quadrant(coord a) {
    if (a.x>0 && a.y >=0) return (1 + init_rot)%4+1;
    if (a.x>=0 && a.y <0) return (2 + init_rot)%4+1;
    if (a.x<0 && a.y<=0) return (3 + init_rot)%4+1;
    if (a.x<=0 && a.y >0) return (4 + init_rot)%4+1;
    std::cerr<<"WISKUNDE IS KAPOT"<<std::endl;
    return -1;
}

bool my_compare(coord a,coord b) {
    int qa=get_quadrant(a),qb=get_quadrant(b);
    if (qa != qb) {
        return !(qa < qb);
    }
    switch (qa) {
        case (1+init_rot)%4+1:
        case (3+init_rot)%4+1:
        case (2+init_rot)%4+1:
        case (4+init_rot)%4+1:
            return !(a.x*b.y < a.y*b.x);
        default : std::cerr<<"OEI, ERG KAPOT"<<std::endl;return false;
    }
}

void cout_name(bool (*name)(int,int)) {
    if (name == &my_greater) {
        std::cout<<"GREATER";
    } else if (name == &my_less) {
        std::cout<<"LESS";
    } else std::cerr<<(void*)name<<" UNRECOGNISED"<<std::endl;
}

bool is_in_sight(int i, int j, int k, int l, bool asteroids[][30]) {
    if (k==i) {
        if (j<l) {
			for(int m=j+1;m<l;m++){
				if (asteroids[k][m]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<k<<' '<<m<<std::endl;
                    return false;
                }
            }
        } else {
            for(int m=j-1;m>l;m--){
                if (asteroids[k][m]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<k<<' '<<m<<std::endl;
                    return false;
                }
            }
        }
    } else if (l==j) {
        if (i<k) {
            for(int m=i+1;m<k;++m){
                if (asteroids[m][j]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<m<<' '<<j<<std::endl;
                    return false;
                }
            }
        } else {
            for(int m=i-1;m>k;m--){
                if (asteroids[m][j]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<m<<' '<<j<<std::endl;
                    return false;
                }
            }
        }
    } else {
        int my_gcd = gcd(abs(k-i),abs(j-l));// Is non-zero
        if (my_gcd == 0)std::cerr<<"FOUND 0 gcd of"<<k-i<<','<<j-l<<std::endl;
        int step_x = (k-i)/my_gcd,step_y = (l-j)/my_gcd;
        int m=i+step_x,n=j+step_y;
if(verbose)std::cout<<m<<' '<<n<<'('<<my_gcd<<')'<<step_x<<' '<<step_y<<':';
        if(m>0&&n>0){
            bool (*x_comp)(int,int)=(k<i ? &my_greater : &my_less);
            bool (*y_comp)(int,int)=(l<j ? &my_greater : &my_less);
            while (x_comp(m,k) && y_comp(n,l)){
               if (asteroids[m][n]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<m<<' '<<n<<std::endl;
                   return false;
               }
               m += step_x;
               n += step_y;
            }
        }
    }
    return true;
}

void read(int&width,int&height,bool asteroids[][30]) {
    char a=std::cin.get();
    while(!std::cin.eof()) {
        width=0;
        while(a!='\n') {
           switch(a) {
              case '.':asteroids[width][height]=false;break;
              case '#':asteroids[width][height]=true;break;
              default :std::cerr<<"UNKNOWN VALUE ENCOUNTERED:"<<a<<'='<< static_cast<int>(a)<<std::endl;
           }
           a=std::cin.get();
           ++width;
        }
        a=std::cin.get();
        ++height;
    }
    std::cout<<width<<' '<<height<<std::endl;
}


int main() {
    int width=0, height=0, position_x,position_y;
    bool asteroids[30][30];
    bool in_sight[30][30][30][30];
    int in_sight_max=0, in_sight_total;
    read(width,height,asteroids);
    std::cout<<get_quadrant(coord(1,0))<<' '<<get_quadrant(coord(1,1))<<my_compare(coord(1,0),coord(1,1))<<' '<<my_compare(coord(1,0),coord(-1,0))<<' '<<my_compare(coord(1,0),coord(0,1))<<std::endl;
    for(int i=0;i<width;i++){std::cout<<std::endl;
        for(int j=0;j<height;j++){std::cout<<(asteroids[i][j]?'#':'.');
            if(!asteroids[i][j])continue;//should be placed on an asteroid
            in_sight_total=0;
            for(int k=0;k<width;k++)for(int l=0;l<height;l++){//should we place it here?
                if (!asteroids[k][l] || (k==i && j==l))continue;
                if (is_in_sight(i,j,k,l,asteroids)) {
if(verbose)std::cout<<i<<' '<<j<<" CAN SEE "<<k<<' '<<l<<std::endl;
                    in_sight[i][j][k][l]=true;
                    ++in_sight_total;
                }else in_sight[i][j][k][l]=false;
            }
if(verbose)std::cout<<in_sight_total;
        if (in_sight_total > in_sight_max) {
            in_sight_max=in_sight_total;
            position_x=i;
            position_y=j;
        }
        }
    }std::cout<<std::endl<<"POSITION:"<<position_x<<' '<<position_y<<std::endl<<"REMOVING UNSEEN ASTEROIDS";
    if (in_sight_max <200)std::cerr<<"LASER HAS TO TURN MORE THAN ONCE, PROGRAMMER WAS TOO LAZY FOR THAT"<<std::endl;
    coord coordinates [900];
    coord origin(position_x,position_y);
    int numberofcoords=0;
    for(int i=0;i<width;i++){
        std::cout<<std::endl;
        for(int j=0;j<height;j++){
            if (i==position_x && j==position_y)
                std::cout<<'O';
            else if (ASTEROIDS[i][j]) {
                coordinates[numberofcoords++] = coord(i-position_x,j-position_y);
                std::cout<<'#';
            } else std::cout<<'.';
         }
    }
    std::sort(coordinates, coordinates + numberofcoords, my_compare);
    for(int k=0;k<numberofcoords;k++) {
    std::cout<<std::endl<<std::endl<<"shooting down "<<coordinates[k].x<<','<<coordinates[k].y<<'='<<coordinates[k].x+origin.x<<','<<coordinates[k].y+origin.y<<" in quadrant "<<get_quadrant(coordinates[k]);
    for(int i=0;i<width;i++){
        std::cout<<std::endl;
        for(int j=0;j<height;j++){
            if (i==position_x && j==position_y)
                std::cout<<'O';
            else if (i==coordinates[k].x +origin.x&&j==coordinates[k].y+origin.y)
                std::cout<<'X';
            else if (asteroids[i][j]) {
                std::cout<<'#';
            } else std::cout<<'.';
         }
    }
    asteroids[coordinates[k].x+position_x][coordinates[k].y+position_y] = false;
    }

    std::cout<<std::endl;
    std::sort(coordinates, coordinates + numberofcoords, my_compare);
    for (int i=0;i<numberofcoords;i++)std::cout<<(i==198?"(":"")<<coordinates[i].x + origin.x<<','<<coordinates[i].y+origin.y<<(i==198?")":"")<<" < ";
    std::cout<<std::endl;
    std::cout<<"ANSWER:"<<(coordinates[199].x + origin.x)*100 + (coordinates[199].y + origin.y)<<std::endl;
    return 0;
}
