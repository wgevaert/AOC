#include <iostream>

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

void cout_name(bool (*name)(int,int)) {
    if (name == &my_greater) {
        std::cout<<"GREATER";
    } else if (name == &my_less) {
        std::cout<<"LESS";
    } else std::cerr<<(void*)name<<" UNRECOGNISED"<<std::endl;
}

int main() {
    int width=0, height=0, position_x,position_y;
    bool asteroids[30][30];
    bool verbose = false;
    int in_sight_max=0, in_sight_total;
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
    for(int i=0;i<width;i++){std::cout<<std::endl;
        for(int j=0;j<height;j++){std::cout<<(asteroids[i][j]?'#':'.');
            if(!asteroids[i][j])continue;//should be placed on an asteroid
            in_sight_total=0;
            for(int k=0;k<width;k++)for(int l=0;l<height;l++){//should we place it here?
                if (!asteroids[k][l] || (k==i && j==l))continue;
                bool in_sight = true;
                if (k==i) {
                    if (j<l) {
                        for(int m=j+1;m<l;m++){
                            if (asteroids[k][m]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<k<<' '<<m<<std::endl;
                                in_sight = false;break;
                            }
                        }
                    } else {
                        for(int m=j-1;m>l;m--){
                            if (asteroids[k][m]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<k<<' '<<m<<std::endl;
                                in_sight = false;break;
                            }
                        }
                    }
                } else if (l==j) {
                    if (i<k) {
                        for(int m=i+1;m<k;++m){
                            if (asteroids[m][j]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<m<<' '<<j<<std::endl;
                                in_sight = false;break;
                            }
                        }
                    } else {
                        for(int m=i-1;m>k;m--){
                            if (asteroids[m][j]){
if(verbose)std::cout<<i<<' '<<j<<" TO "<<k<<' '<<l<<" IS BLOCKED BY "<<m<<' '<<j<<std::endl;
                                in_sight = false;break;
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
                               in_sight = false;break;
                           }
                           m += step_x;
                           n += step_y;
                        }
                    }
                }
                if (in_sight) {
if(verbose)std::cout<<i<<' '<<j<<" CAN SEE "<<k<<' '<<l<<std::endl;
                    ++in_sight_total;
                }
            }
if(verbose)std::cout<<in_sight_total;
        if (in_sight_total > in_sight_max) {
            in_sight_max=in_sight_total;
            position_x=i;
            position_y=j;
        }
        }
    }std::cout<<std::endl<<"ANSWER:"<<in_sight_max<<" AT "<<position_x<<' '<<position_y<<std::endl;
}


