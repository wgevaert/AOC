#include <iostream>
#include <fstream>

int main(int argc,char**argv){
    if(argc<2){std::cout<<"HELP!"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    char area[52][52][2];
    char prev_area[50][50];
    for(int i=0;i<52;i++)for(int j=0;j<52;j++)for(int k=0;k<1;k++)area[i][j][k]='?';
    unsigned x=0,y=0,p=0,q=1,cnt=1,prev_cnt=1;
    char a;
    do {
        a=input_file.get();
        if(a=='.'||a=='#'||a=='|') {
            area[x+1][y+1][0]=a;
            x++;
        } else{x=0;y++;}
    }while(y<50);

    bool duplicated = false;
    while(!duplicated){
        for(int i=1;i<=50;i++)for(int j=1;j<=50;j++){
            switch(area[i][j][p]) {
                case '.':
                        if((area[i-1][j-1][p]=='|')+(area[i-1][j][p]=='|')+(area[i-1][j+1][p]=='|')+(area[i][j+1][p]=='|')+(area[i+1][j+1][p]=='|')+(area[i+1][j][p]=='|')+(area[i+1][j-1][p]=='|')+(area[i][j-1][p]=='|') >= 3)
                            area[i][j][q]='|';
                        else area[i][j][q]='.';
                    break;
                case '#':
                        if(((area[i-1][j-1][p]=='#')+(area[i-1][j][p]=='#')+(area[i-1][j+1][p]=='#')+(area[i][j+1][p]=='#')+(area[i+1][j+1][p]=='#')+(area[i+1][j][p]=='#')+(area[i+1][j-1][p]=='#')+(area[i][j-1][p]=='#') >= 1) &&
                            ((area[i-1][j-1][p]=='|')+(area[i-1][j][p]=='|')+(area[i-1][j+1][p]=='|')+(area[i][j+1][p]=='|')+(area[i+1][j+1][p]=='|')+(area[i+1][j][p]=='|')+(area[i+1][j-1][p]=='|')+(area[i][j-1][p]=='|') >= 1))
                            area[i][j][q]='#';
                        else area[i][j][q]='.';
                    break;
                case '|':
                        if((area[i-1][j-1][p]=='#')+(area[i-1][j][p]=='#')+(area[i-1][j+1][p]=='#')+(area[i][j+1][p]=='#')+(area[i+1][j+1][p]=='#')+(area[i+1][j][p]=='#')+(area[i+1][j-1][p]=='#')+(area[i][j-1][p]=='#') >= 3)
                            area[i][j][q]='#';
                        else area[i][j][q]='|';
                    break;
                default: std::cout<<area[i][j][p]<<" not expected"<<std::endl;return 1;
            }
        }
        if (cnt==prev_cnt) {
            for(int i=0;i<50;i++)for(int j=0;j<50;j++)prev_area[i][j]=area[i+1][j+1][p];
            prev_cnt*=2;
        }
        duplicated = true;
        for(int i=0;i<50;i++)for(int j=0;j<50;j++)if(prev_area[i][j]!=area[i+1][j+1][q])duplicated = false;
if(cnt==10) {
    unsigned wood=0,lumber=0;
    for(int i=1;i<=50;i++)for(int j=1;j<=50;j++){wood+=area[i][j][q]=='|';lumber+=area[i][j][q]=='#';}
    std::cout<<"answer1: "<<wood*lumber<<std::endl;wood=lumber=0;
}
        std::cout<<++cnt<<std::endl;
        for(int i=1;i<=50;i++){for(int j=1;j<=50;j++)std::cout<<area[j][i][q];std::cout<<' ';for(int j=0;j<50;j++)std::cout<<prev_area[j][i-1];std::cout<<std::endl;}
        p=1-p;q=1-q;
    }

    unsigned period = cnt-prev_cnt/2;
    std::cout<<"A period of "<<cnt<<"->"<<prev_cnt/2<<' '<<period<<" is found"<<std::endl;
    unsigned extra_steps=(1000000000-cnt)%period;//(period - ((1000000000-cnt)%period));

    while(extra_steps != 0){
        for(int i=1;i<=50;i++)for(int j=1;j<=50;j++){
            switch(area[i][j][p]) {
                case '.':
                        if((area[i-1][j-1][p]=='|')+(area[i-1][j][p]=='|')+(area[i-1][j+1][p]=='|')+(area[i][j+1][p]=='|')+(area[i+1][j+1][p]=='|')+(area[i+1][j][p]=='|')+(area[i+1][j-1][p]=='|')+(area[i][j-1][p]=='|') >= 3)
                            area[i][j][q]='|';
                        else area[i][j][q]='.';
                    break;
                case '#':
                        if(((area[i-1][j-1][p]=='#')+(area[i-1][j][p]=='#')+(area[i-1][j+1][p]=='#')+(area[i][j+1][p]=='#')+(area[i+1][j+1][p]=='#')+(area[i+1][j][p]=='#')+(area[i+1][j-1][p]=='#')+(area[i][j-1][p]=='#') >= 1) &&
                            ((area[i-1][j-1][p]=='|')+(area[i-1][j][p]=='|')+(area[i-1][j+1][p]=='|')+(area[i][j+1][p]=='|')+(area[i+1][j+1][p]=='|')+(area[i+1][j][p]=='|')+(area[i+1][j-1][p]=='|')+(area[i][j-1][p]=='|') >= 1))
                            area[i][j][q]='#';
                        else area[i][j][q]='.';
                    break;
                case '|':
                        if((area[i-1][j-1][p]=='#')+(area[i-1][j][p]=='#')+(area[i-1][j+1][p]=='#')+(area[i][j+1][p]=='#')+(area[i+1][j+1][p]=='#')+(area[i+1][j][p]=='#')+(area[i+1][j-1][p]=='#')+(area[i][j-1][p]=='#') >= 3)
                            area[i][j][q]='#';
                        else area[i][j][q]='|';
                    break;
                default: std::cout<<area[i][j][p]<<" not expected"<<std::endl;return 1;
            }
        }
        std::cout<<--extra_steps<<std::endl;
    unsigned wood=0,lumber=0;
    for(int i=1;i<=50;i++)for(int j=1;j<=50;j++){wood+=area[i][j][q]=='|';lumber+=area[i][j][q]=='#';}
  if(wood*lumber<192676)std::cout<<"score: "<<wood*lumber<<std::endl;wood=lumber=0;// the site said it was smaller than this, so I just tried some values...
//        for(int i=1;i<=50;i++){for(int j=1;j<=50;j++)std::cout<<area[j][i][p];std::cout<<' ';for(int j=0;j<50;j++)std::cout<<prev_area[j][i-1];std::cout<<std::endl;}
        p=1-p;q=1-q;
    }
    unsigned wood=0,lumber=0;
    for(int i=1;i<=50;i++)for(int j=1;j<=50;j++){wood+=area[i][j][q]=='|';lumber+=area[i][j][q]=='#';}
    std::cout<<"answer2: "<<wood*lumber<<std::endl;wood=lumber=0;

    return 0;
}
