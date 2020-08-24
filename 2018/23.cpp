#include <iostream>
#include <fstream>
#include <vector>

const unsigned size=1000;

unsigned long long abs(long long a){return a<0 ? -1*a:a;}

struct nanobot {
    long long x,y,z;
    unsigned long long r;
    nanobot(long long _x,long long _y,long long _z,unsigned long long _r):x(_x),y(_y),z(_z),r(_r){};
    bool in_range(long long _x,long long _y,long long _z){return abs(x-_x)+abs(y-_y)+abs(z-_z)<=r;}
    bool in_range(nanobot b){return in_range(b.x,b.y,b.z);}
    bool is_adj(nanobot b){return abs(x-b.x)+abs(y-b.y)+abs(z-b.z)<=r+b.r;}
};

std::vector<nanobot> part1(std::istream& input_file);


/*
 * implementation of Bron-Kerbosh, finds the good clique, but then does not finish in my lifetime with the given input.
 */
std::vector<std::vector<unsigned>> BK(std::vector<unsigned>& R,std::vector<unsigned>& P,std::vector<unsigned>& X, bool** adj_mat){
    static std::vector<std::vector<unsigned>> max_cliques = {};
    static unsigned long recur_depth = 0;
    recur_depth++;
if(recur_depth<=960){for(auto a:max_cliques)std::cout<<a.size()<<' ';std::cout<<"at "<<recur_depth<<' '<<max_cliques.size()<<std::endl;}
    if(!P.size()&&!X.size())max_cliques.push_back(R);
    else while(P.size()) {
//std::cout<<"HEY!"<<std::flush;
        std::vector<unsigned> new_P={},new_X={},new_R=R;
//std::cout<<"HEy!"<<P.back()<<std::flush;
        bool push=true;
        for(auto a:R)if(a==P.back())push=false;
        if(push){
            new_R.push_back(P.back());
//            std::cout<<"pushed it!"<<std::endl;
        }
//std::cout<<"HeY!"<<std::flush;
        for(auto a:P)if(a!=P.back()&&adj_mat[a][P.back()])new_P.push_back(a);
//std::cout<<"hEY!"<<std::flush;
        for(auto a:X)if(a!=P.back()&&adj_mat[a][P.back()])new_X.push_back(a);
//std::cout<<"Hey!"<<std::flush;
        BK(new_R,new_P,new_X,adj_mat);
//std::cout<<"heY!"<<std::flush;
        X.emplace_back(P.back());
//std::cout<<"hEy!"<<std::flush;
        P.pop_back();
    }
//std::cout<<max_cliques.size()<<std::endl;
    recur_depth--;
    return max_cliques;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    std::vector<nanobot> nanobots = part1(input_file);

    bool **adj_mat = new bool*[nanobots.size()];
    for(unsigned i=0;i<nanobots.size();i++){
        adj_mat[i] = new bool[nanobots.size()];
        for(unsigned j=0;j<nanobots.size();j++){
            adj_mat[i][j]=nanobots[i].is_adj(nanobots[j]);
//            std::cout<<(adj_mat[i][j] ? '#' : '.');
        }//std::cout<<'\n';
    }

    std::vector<unsigned> R={},P={},X={};
    for(int i=0;i<nanobots.size();i++)P.emplace_back(i);
std::cout<<P.size()<<std::endl;
    std::vector<std::vector<unsigned>> units=BK(R,P,X,adj_mat);

std::cout<<units.size()<<std::endl;

    return 0;
}

std::vector<nanobot> part1(std::istream& input_file) {
    std::vector<nanobot> nanobots = {};unsigned long long max_r = 0,max_index;for(;;){while(!input_file.eof()&&input_file.get()!='<');if(input_file.eof())break;long long x,y,z;unsigned long long r;input_file>>x;if(input_file.get()!=','){std::cout<<", expected"<<std::endl;exit(1);}input_file>>y;if(input_file.get()!=','){std::cout<<", expected"<<std::endl;exit(1);}input_file>>z;if(input_file.get()!='>'){std::cout<<"> expected"<<std::endl;exit(1);}while(!input_file.eof()&&input_file.get()!='=');if(input_file.eof()){std::cout<<"Weird..."<<std::endl;exit(1);}input_file>>r;if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;exit(1);}if(max_r<r){max_r=r;max_index=nanobots.size();}nanobots.emplace_back(x,y,z,r);}unsigned answer1 = 0;for(unsigned i=0;i<nanobots.size();i++)if(nanobots[max_index].in_range(nanobots[i]))answer1++;
    std::cout<<answer1<<std::endl;
    return nanobots;
}
