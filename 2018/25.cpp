#include <iostream>
#include <vector>
#include <string>
#include <fstream>

unsigned long abs(long a){return static_cast<unsigned>(a<0?-1*a:a);}

struct point {
    long c[4];
    unsigned const_nr=0;
    std::vector<unsigned> connected_to;
    unsigned long dist(point b){unsigned long result=0; for(int i=0;i<4;i++)result+=abs(c[i]-b.c[i]); return result;}
};

std::ostream& operator<<(std::ostream& out,point p) {
    for(int i=0;i<4;i++)out<<p.c[i]<<(i==3?'(':',');out<<p.const_nr<<')';
    return out;
}

void read(std::string check,std::istream& input){
    char A;
    for(auto a:check)if((A=input.get())!=a){
       if(input.eof()){std::cout<<"Unexpected eof found instead of"<<check<<std::endl;exit(1);}
       std::cout<<check<<" expected, '"<<A<<"' found instead of '"<<a<<"' (value "<<static_cast<int>(A)<<')'<<std::endl;exit(1);
    }
}

void fill_const(std::vector<point>& p, size_t i,unsigned nr) {
    p[i].const_nr=nr;
    for(auto& a:p[i].connected_to)if(!p[a].const_nr)fill_const(p,a,nr);
}

int main(int argc,char** argv) {
    if(argc<2){std::cout<<"Usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<point> points={};

    while(input_file.peek()!=static_cast<char>(-1)) {
        points.emplace_back();
        input_file>>points.back().c[0];
        read(",",input_file);
        input_file>>points.back().c[1];
        read(",",input_file);
        input_file>>points.back().c[2];
        read(",",input_file);
        input_file>>points.back().c[3];
        read("\n",input_file);
    }

    for(size_t i=1;i<points.size();i++)
        for(size_t j=0;j<i;j++)
            if(points[i].dist(points[j])<=3) {
                points[i].connected_to.emplace_back(j);
                points[j].connected_to.emplace_back(i);;
            }

    unsigned const_nr = 0;

    for(unsigned i=0;i<points.size();i++) {
       if(!points[i].const_nr)fill_const(points,i,++const_nr);
    }
    std::cout<<const_nr<<std::endl;
    return 0;
}
