#include <iostream>
#include <fstream>
#include <string>

long abs(long a){return a>0?a:-1*a;}

class vect {
public:
    long a,b;
    vect(int ga,int gb) {
        a=ga;b=gb;
    };

    vect operator+=(vect x){a+=x.a;b+=x.b;return *this;};
    ~vect(){};
    long dist(){
        if ((a>=0&&b<=0)||(a<=0&&b>=0))
            return abs(a)+abs(b);
        else if (abs(a) >= abs(b))
            return abs(a);
        else return abs(b);
    }
};


const vect n(1,0), ne(1,1), se(0,1), s(-1,0),sw(-1,-1),nw(0,-1);

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {name of input file}"<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }

    vect pos(0,0);
    char a;
    bool done=false;
    int max_dist = 0;
    while(!done) {
        switch(a=input_file.get()) {
            case '\n':
                done = true;
                break;
            case 'n':
                switch(a=input_file.get()) {
                     case '\n':
                         done=true;
                         // no break
                     case ',':
                         pos += n;
                         break;
                     case 'e':
                         pos += ne;
                         break;
                     case 'w':
                         pos += nw;
                         break;
                     default:
                         std::cout<<"HELP!!!"<<a<< static_cast<int>(a)<<std::endl;return 1;
                }
                break;
            case 's':
                switch(a=input_file.get()) {
                     case '\n':
                         done=true;
                         // no break
                     case ',':
                         pos += s;
                         break;
                     case 'e':
                         pos += se;
                         break;
                     case 'w':
                         pos += sw;
                         break;
                     default:
                         std::cout<<"HELP!!"<<a<< static_cast<int>(a)<<std::endl;return 1;
                }
                break;
            case ',':
                break;
            default: std::cout<<"HELP ME!!"<<a<< static_cast<int>(a)<<std::endl;return 1;
        };
        if(max_dist<pos.dist())max_dist=pos.dist();
    }
    std::cout<<pos.a<<','<<pos.b<<'='<<pos.dist()<<' '<<max_dist<<std::endl;

    return 0;
}
