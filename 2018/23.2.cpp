#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>

const unsigned size=1000;

unsigned long long abs(long long a){return a<0 ? -1*a:a;}

struct field {
    long long x_min=0,x_max=0,y_min=0,y_max=0,z_min=0,z_max=0;
    field(){x_min=x_max=y_min=y_max=z_min=z_max=0;}
    field(long long _x_min,long long _x_max,long long _y_min,long long _y_max,long long _z_min, long long _z_max) : x_min(_x_min),x_max(_x_max),y_min(_y_min),y_max(_y_max),z_min(_z_min),z_max(_z_max) {}
    unsigned long min_dist(){
        unsigned long long x,y,z,max = -1;
        if((x_min<=0&&x_max>=0)||(x_min>=0&&x_max<=0))x=0;else x=std::min(abs(x_min),abs(x_max));
        if((y_min<=0&&y_max>=0)||(y_min>=0&&y_max<=0))y=0;else y=std::min(abs(y_min),abs(y_max));
        if((z_min<=0&&z_max>=0)||(z_min>=0&&z_max<=0))z=0;else z=std::min(abs(z_min),abs(z_max));
        if(x+y > max-z){std::cout<<"HELP ME!"<<std::endl<<x<<','<<y<<','<<z<<std::endl;exit(1);}
//std::cout<<'\n'<<std::flush;//std::cout<<x;//<<','<<y<<','<<z<<"   "<<x+y+z<<" <-"<<max<<std::endl;
        return x+y+z;
    }
};

std::ostream& operator<<(std::ostream& out,field f){
    return (out<<std::setw(4)<<std::setfill(' ')<<f.x_min<<','<<std::setw(4)<<std::setfill(' ')<<f.x_max<<';'<<std::setw(4)<<std::setfill(' ')<<f.y_min<<','<<std::setw(4)<<std::setfill(' ')<<f.y_max<<';'<<std::setw(4)<<std::setfill(' ')<<f.z_min<<','<<std::setw(4)<<std::setfill(' ')<<f.z_max);
}

struct nanobot {
    long long x,y,z;
    unsigned long long r;
    nanobot(long long _x,long long _y,long long _z,long long _r):x(_x),y(_y),z(_z),r(_r){};
    bool in_range(long long _x,long long _y,long long _z) const {return abs(x-_x)+abs(y-_y)+abs(z-_z)<=r;}
    bool in_range(nanobot b) const {return in_range(b.x,b.y,b.z);}
    bool in_field(field f) {
        f.x_min -= x;f.x_max-=x;
        f.y_min -= y;f.y_max-=y;
        f.z_min -= z;f.z_max-=z;
        return f.min_dist()<=r;
    }
};

std::vector<nanobot> part1(std::istream& input_file, field& f);


std::pair<unsigned long,unsigned long> solve2(field f,const std::vector<nanobot>& nanobots) {
    static unsigned rd=0; // recursion depth
    static std::pair<unsigned long,unsigned long> min={static_cast<unsigned long>(-1),static_cast<unsigned long>(-1)};

    std::vector<field> new_chunks;
    std::vector<std::pair<long long,long long>> xs,ys,zs;
    std::vector<std::pair<std::pair<unsigned,unsigned>,size_t>> units = {};
    std::vector<std::pair<unsigned long,unsigned long>> results ={};

    rd++;

    if (f.x_min==f.x_max&&f.y_min==f.y_max&&f.z_min==f.z_max){
        unsigned long cnt=0;
        for (auto a:nanobots)
            if(a.in_range(f.x_min,f.y_min,f.z_min))cnt++;
        std::pair<unsigned long,unsigned long> ans = {nanobots.size()-cnt,abs(f.x_min)+abs(f.y_min)+abs(f.z_min)};
        if(ans<min){min=ans;std::cout<<nanobots.size()-min.first<<" bots found at distance "<<min.second<<std::endl;}
        rd--;
//std::cout<<" returning "<<cnt<<" at "<<f<<std::endl;
        return ans;
    }


    if(f.x_min<f.x_max)
        xs={{f.x_min,(f.x_max+f.x_min-1)/2},{(f.x_min+f.x_max+1)/2,f.x_max}};
    else xs = {{f.x_min,f.x_max}};
    if(f.y_min<f.y_max)
        ys={{f.y_min,(f.y_max+f.y_min-1)/2},{(f.y_min+f.y_max+1)/2,f.y_max}};
    else ys = {{f.y_min,f.y_max}};
    if(f.z_min<f.z_max)
        zs={{f.z_min,(f.z_max+f.z_min-1)/2},{(f.z_min+f.z_max+1)/2,f.z_max}};
    else zs = {{f.z_min,f.z_max}};
    for(auto x:xs)for(auto y:ys)for(auto z:zs)new_chunks.emplace_back(x.first,x.second,y.first,y.second,z.first,z.second);
//for(field f:new_chunks)std::cout<<f<<'('<<f.min_dist()<<")    ";std::cout<<"->";
    std::sort(new_chunks.begin(),new_chunks.end(),[](field a,field b){return a.min_dist()<b.min_dist();});
//for(field f:new_chunks)std::cout<<f<<'('<<f.min_dist()<<")\n";std::cout<<std::endl;

    for(size_t i=0;i<new_chunks.size();i++) {
        unsigned long cnt=0;
        for(auto b:nanobots)if(b.in_field(new_chunks[i]))cnt++;
//std::cout<<new_chunks[i]<<'('<<new_chunks[i].min_dist()<<"), ["<<cnt<<"]\n";
        if(std::make_pair(nanobots.size()-cnt,new_chunks[i].min_dist()) < min)units.emplace_back(std::piecewise_construct,std::make_tuple(nanobots.size()-cnt,new_chunks[i].min_dist()),std::make_tuple(i));
//else std::cout<<"SKIPPING "<<cnt<<','<<new_chunks[i].min_dist()<<" IN FAVOR OF "<<nanobots.size()-min.first<<','<<min.second<<std::endl;
    }
//std::cout<<min.first<<','<<min.second<<std::endl;
    if(units.size())std::sort(units.begin(),units.end());
//for(auto a:units)std::cout<<a.first<<','<<a.second<<' ';std::cout<<std::endl;
//std::cout<<results.size()<<" <="<<std::endl;
//if(results.size()>10){std::cout<<"HELP!!"<<std::endl;return {1/0,1/0};}
    for(auto a : units) {
//std::cout<<a.second<<' '<<new_chunks.size()<<std::endl;
        results.push_back(solve2(new_chunks[a.second],nanobots));
//std::cout<<results.size()<<" <="<<std::endl;
    }

    if(results.size())std::sort(results.begin(),results.end());
//std::cout<<f<<std::endl;
//for(auto a:results)std::cout<<a.first<<','<<a.second<<std::endl;
    rd --;
    if(results.size())
        return results[0];//.back();
    return min;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    field f;

    std::vector<nanobot> nanobots = part1(input_file, f);

    if(f.x_max<f.x_min||f.y_max<f.y_min||f.z_max<f.z_min){std::cout<<"HELP!"<<std::endl;exit(1);}

    std::cout<<solve2(f,nanobots).second<<std::endl;

    return 0;
}

std::vector<nanobot> part1(std::istream& input_file,field& f) {
    std::vector<nanobot> nanobots = {};
    unsigned long long max_r = 0,max_index;
    for(;;){
        while(!input_file.eof()&&input_file.get()!='<');
        if(input_file.eof())break;
        long long x,y,z,r;
        input_file>>x;
        if(input_file.get()!=','){std::cout<<", expected"<<std::endl;exit(1);}
        input_file>>y;
        if(input_file.get()!=','){std::cout<<", expected"<<std::endl;exit(1);}
        input_file>>z;
        if(input_file.get()!='>'){std::cout<<"> expected"<<std::endl;exit(1);}
        while(!input_file.eof()&&input_file.get()!='=');
        if(input_file.eof()){std::cout<<"Weird..."<<std::endl;exit(1);}
        input_file>>r;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;exit(1);}
        if(max_r<static_cast<unsigned long long>(r)){max_r=r;max_index=nanobots.size();}nanobots.emplace_back(x,y,z,r);
        if(f.x_min > x-r)f.x_min=x-r;
        if(f.y_min > x-r)f.y_min=y-r;
        if(f.z_min > z-r)f.z_min=z-r;
        if(f.x_max < x+r)f.x_max=x+r;
        if(f.y_max < y+r)f.y_max=y+r;
        if(f.z_max < z+r)f.z_max=z+r;
    }
    unsigned answer1 = 0;for(unsigned i=0;i<nanobots.size();i++)if(nanobots[max_index].in_range(nanobots[i]))answer1++;
    std::cout<<answer1<<std::endl;
    return nanobots;
}
