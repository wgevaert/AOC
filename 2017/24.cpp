#include <iostream>
#include <fstream>
#include <string>
#include <vector>


const unsigned port_max = 100;

struct port{
    std::vector<unsigned> components={};
    unsigned order=0;
};

bool in_bridge(std::vector<unsigned> bridge,unsigned a,unsigned b){
    for(unsigned i=0;i<bridge.size()-1;i++)if((a==bridge[i]&&b==bridge[i+1])||(a==bridge[i+1]&&b==bridge[i]))return true;
    return false;
}

unsigned get_strength(std::vector<unsigned> bridge,port* ports){
    unsigned strength=0;
    for(unsigned i=0;i<bridge.size()-1;i++){
        strength +=bridge[i]+bridge[i+1];
    }
    return strength;
}

std::pair<unsigned,unsigned> make_bridges(port* ports,unsigned cur_port, std::vector<unsigned>& cur_bridge) {
    static unsigned max_strength = 0,longest=0,longest_strength=0;
    for(unsigned a : ports[cur_port].components){
        if (!in_bridge(cur_bridge,cur_port,a)) {
            cur_bridge.push_back(a);
            unsigned strength = get_strength(cur_bridge,ports);
            if (cur_bridge.size()>=longest){if(longest_strength < strength || cur_bridge.size()>longest){longest_strength=strength;std::cout<<"NEW MAX STRENGTH "<<longest_strength<<" FOUND FOR BRIDGE ";for(auto a:cur_bridge)std::cout<<a<<' ';std::cout<<std::endl;}longest=cur_bridge.size();}
            else if(cur_bridge.size()==longest && longest_strength<strength)longest_strength=strength;
            if (strength>max_strength)max_strength = strength;
            make_bridges(ports,a,cur_bridge);
            cur_bridge.pop_back();
        }
    }
    return std::make_pair(max_strength,longest_strength);
}

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

    port ports[port_max];
    unsigned a,b;

    while(!input_file.eof()){
        input_file>>a;
        if(input_file.get()!='/'){if(input_file.eof())break;std::cout<<"OH NOES!"<<std::endl;return 1;}
        input_file>>b;
        if (a>=port_max||b>=port_max){std::cout<<"HELP ME! "<<a<<','<<b<<std::endl;return 1;}
        ports[a].order++;
        ports[b].order++;
        ports[a].components.push_back(b);
        ports[b].components.push_back(a);
    }
    std::vector<unsigned> cur_bridge={0};

    std::pair<unsigned,unsigned> answer = make_bridges(ports,0,cur_bridge);
    std::cout<<"MAX STRENGTH IS "<<answer.first<<", MAX STRENGTH OF LONGEST BRIDGE IS "<<answer.second<<std::endl;

    return 0;
}
