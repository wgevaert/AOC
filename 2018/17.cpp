#include <iostream>
#include <fstream>

const unsigned x_size=1024,y_size=2048;

int main(int argc, char** argv){
    if(argc < 2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    char ground[x_size][y_size];
    for(unsigned i=0;i<x_size;i++)for(unsigned j=0;j<y_size;j++)ground[i][j]='.';
    ground[500][0]='+';

    unsigned y_min=-1,y_max=0,x_min=-1,x_max=0;
    for(;;){
        bool xy;
        unsigned a,b,c;
        switch(input_file.get()) {
             case 'x':xy=true;break;
             case 'y':xy=false;break;
             default:if(!input_file.eof()){std::cout<<"weird..."<<std::endl;return 1;}
        }
        if(input_file.eof())break;
        if(input_file.get()!='='){std::cout<<"= expected"<<std::endl;return 1;}
        input_file>>a;
        if(input_file.get()!=','||input_file.get()!=' '){std::cout<<",  expected"<<std::endl;return 1;}
        if((xy&&input_file.get()!='y')||(!xy&&input_file.get()!='x')){std::cout<<"x or y expected"<<std::endl;return 1;}
        if(input_file.get()!='='){std::cout<<"= expected again"<<std::endl;return 1;}
        input_file>>b;
        if(input_file.get()!='.'||input_file.get()!='.'){std::cout<<".. expected"<<std::endl;return 1;}
        input_file>>c;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
        if(xy) {
            if(a<x_min)x_min=a;
            if(a>x_max)x_max=a;
            if(b<y_min)y_min=b;
            if(c>y_max)y_max=c;
            for(int i=b;i<=c;i++)ground[a][i]='#';
        } else {
            if(a<y_min)y_min=a;
            if(a>y_max)y_max=a;
            if(b<x_min)x_min=b;
            if(c>x_max)x_max=c;
            for(int i=b;i<=c;i++)ground[i][a]='#';
        }
    }
//    std::cout<<y_min<<' '<<y_max<<','<<x_min<<' '<<x_max<<std::endl;
    bool flowing;
    do {
        flowing =false;
        for(unsigned j=1;j<=y_max;j++)
            for(unsigned i=x_max+1;i>=x_min-1;i--)
                if((ground[i][j]=='.') && ((ground[i][j-1]=='|'||ground[i][j-1]=='+'||ground[i][j-1]=='-') || (ground[i-1][j]=='|' && (ground[i-1][j+1] == '~'||ground[i-1][j+1] == '#')) || (ground[i+1][j]=='|' && (ground[i+1][j+1] == '~'||ground[i+1][j+1] == '#')))) {
                    ground[i][j]='|';
                    flowing = true;
                } else if(ground[i][j]=='|' && (ground[i][j+1]=='#'||ground[i][j+1]=='~') && (ground[i-1][j]=='~'||ground[i+1][j]=='~')){
                    ground[i][j]='~';
                    flowing = true;
                } else if(ground[i][j]=='|' && (ground[i][j+1]=='#'||ground[i][j+1]=='~') && (ground[i-1][j]=='-'||ground[i+1][j]=='-')){
                    ground[i][j]='-';
                    flowing = true;
                } else if(ground[i][j]=='|'&&ground[i][j+1]!='|'&&ground[i][j+1]!='.'&&ground[i+1][j]=='#'){
                    bool make_dash=false,make_tilde=false;
                    for(unsigned k=i;k>=x_min-1;k--)if(ground[k-1][j]!='|'){
//                        std::cout<<ground[k-1][j]<<' '<<ground[k][j+1]<<":)"<<j<<std::endl;
                        if(ground[k][j+1]=='.'||ground[k][j+1]=='|'){
                            make_dash=true;break;
                        } else if((ground[k][j+1]=='#'||ground[k][j+1]=='~')&&ground[k-1][j]=='#'){
                            make_tilde=true;break;
                        } else break;
                    }
                    if(make_dash) {
//                        for(unsigned k=i-1;ground[k][j]=='|';k--)ground[k][j]='-';
//                        std::cout<<"making dash"<<std::endl;
//                        flowing = true;
                    } else if(make_tilde) {
                        for(unsigned k=i;ground[k][j]=='|';k--)ground[k][j]='~';
//                        std::cout<<"making tilde"<<ground[j][i]<<std::endl;
                        flowing = true;
                    }
                }
//        for(unsigned j=0;j<=y_max;j++){for(unsigned i=x_min-1;i<=x_max+1;i++)std::cout<<ground[i][j];std::cout<<' '<<j<<std::endl;}std::cout<<std::endl;
    }while(flowing);
        for(unsigned j=0;j<=y_max;j++){for(unsigned i=x_min-1;i<=x_max+1;i++)std::cout<<ground[i][j];std::cout<<std::endl;}std::cout<<std::endl;
    unsigned water_total=0,still_total;
    for(unsigned i=x_min-1;i<=x_max+1;i++)for(unsigned j=y_min;j<=y_max;j++)if(ground[i][j]=='|')water_total++;else if(ground[i][j]=='~'){water_total++;still_total++;}
    std::cout<<water_total<<' '<<still_total<<std::endl;
}

