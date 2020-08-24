#include <iostream>

// Outputs pgm image to cout, total_lights or error messages to cerr
int main() {
    static long int lights[1000][1000];//static is initialised at 0;
    long long int total_lights = 0;
    char a,dummy;
    int command;
    int x1,x2,y1,y2;
    long int max_brightness=0;
    std::cout<<"P2\n1000 1000\n";
    while(!std::cin.eof()) {
        while(!std::cin.eof() && (a=std::cin.get())<=32); //Gobble whitespace
        if (std::cin.eof())break;
        if (a!='t'){std::cerr<<"Error reading line: "<<a<<" instead of 't'"<<std::endl;return 1;}
        command = (a=std::cin.get()) == 'o' ? 2:0;
        while ((a=std::cin.get())!=' ');//gobble command
        if (!command) {if ((a=std::cin.get())!='o'){std::cerr<<"Error reading line: "<<a<<" instead of 'o'"<<std::endl;return 1;}
            command = (a=std::cin.get()) == 'n' ? 1 : 0;
            while((a=std::cin.get())!=' ');//gobble command
        }
        if(a!=' ')while((a=std::cin.get())!=' ')std::cerr<<"found "<<a<<'='<<static_cast<int>(a)<<std::endl;;//gobble command
        std::cin>>x1;if((a=std::cin.get())!=','){std::cerr<<"Error reading unit: "<<a<<'='<< static_cast<int>(a)<<" instead of ','"<<x1<<std::endl;return 1;}
        std::cin>>y1;
        while((a=std::cin.get())== ' ');//gobble space
        if (a!='t'){std::cerr<<"ERROR: "<<a<<" instead of t"<<std::endl;return 1;}
        if ((a=std::cin.get())!=(dummy='h')|| (a=std::cin.get())!=(dummy='r')|| (a=std::cin.get())!=(dummy='o')|| (a=std::cin.get())!=(dummy='u')|| (a=std::cin.get())!=(dummy='g')|| (a=std::cin.get())!=(dummy='h') || (a=std::cin.get())!=(dummy=' '))
{std::cerr<<"OEI!"<<std::endl;return 1;}
        std::cin>>x2;if((a=std::cin.get())!=','){std::cerr<<"Error reading unit: "<<a<<" instead of ','"<<std::endl;return 1;}
        std::cin>>y2;

        if (x2<x1||y2<y1){std::cerr<<"HUH?"<<std::endl;return 1;}
        for (int i=x1;i<=x2;i++)for(int j=y1;j<=y2;j++)switch(command){case 2:lights[i][j]+=2;break;case 1:lights[i][j]+=1;break;case 0:if(lights[i][j])--lights[i][j];break;default: std::cerr<<"IK SNAP "<<command<<" NIET"<<std::endl;return 1;}
    }
    for (int i=0;i<1000;i++)for(int j=0;j<1000;j++){ if(max_brightness<lights[i][j])max_brightness=lights[i][j];}
    std::cout<<max_brightness<<'\n'<<std::endl;
    for (int i=0;i<1000;i++)for(int j=0;j<1000;j++){ std::cout<<lights[i][j]<<' ';total_lights+=lights[i][j];}
    std::cerr<<total_lights<<std::endl;
    return 0;
}
