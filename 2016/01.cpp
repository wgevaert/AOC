#include <iostream>
#include <fstream>

struct me {
    int x=0,y=0,dx=1,dy=0;
};

int abs(int a){return (a<0?-1*a:a);}

int main(int argc, char** argv){
    std::ifstream input_file;
    if (argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}
    int unit;
    me bunny;

    static bool visited[1025][1025];
    bool first = true, verbose=(argc>=3);
    int x_min=512,x_max=512,y_min=512,y_max=512;
    for(;;){
        switch (input_file.get()){
            case 'L':
                unit=bunny.dx;
                bunny.dx=-1*bunny.dy;
                bunny.dy=unit;
                break;
            case 'R':
                unit=-1*bunny.dx;
                bunny.dx=bunny.dy;
                bunny.dy=unit;
                break;
            default:
                std::cout<<"WEIRD..."<<std::endl;
                return 1;
        }
        input_file>>unit;
        for(int i=0;i<unit;i++){
            bunny.x+=bunny.dx;bunny.y+=bunny.dy;
            if (bunny.x+512<x_min)x_min=bunny.x+512;
            else if (bunny.x+512>x_max)x_max=bunny.x+512;
            else if (bunny.y+512<y_min)y_min=bunny.y+512;
            else if (bunny.y+512>y_max)y_max=bunny.y+512;
            if(first&&visited[bunny.x+512][bunny.y+512]){
                first=false;
                std::cout<<abs(bunny.x)+abs(bunny.y)<<std::endl;
            }
            visited[bunny.x+512][bunny.y+512]=true;
            if(verbose) {
                std::cout<<' '<<x_min<<','<<x_max<<';'<<y_min<<','<<y_max<<':'<<bunny.x+512<<','<<bunny.y+512<<std::endl;
                for(int i=y_min;i<=y_max;i++){for(int j=x_min;j<=x_max;j++)std::cout<<((j==bunny.x+512&&i==bunny.y+512)?'@':(visited[j][i]?'X':'.'));std::cout<<std::endl;}
            }
        }
        if((input_file.get()!=','||input_file.get()!=' ')&&(input_file.get()&&input_file.get()&&!input_file.eof())){std::cout<<"Weird.."<<std::endl;return 1;}

        if(input_file.eof())break;
    }
    std::cout<<abs(bunny.x)+abs(bunny.y)<<std::endl;
    return 0;
}
