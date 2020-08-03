#include <iostream>
#include <fstream>
#include <vector>

const unsigned size = 2000;

struct id {
    std::vector<unsigned> neighbours={};
    int group=-1;
};

void flow(unsigned pos, id* connections, int group_number) {
    static unsigned short count=0;
    if(group_number==0)std::cout<<"FOUND "<<++count<<" IDS IN GROUP 0"<<std::endl;
    for (auto i:connections[pos].neighbours) {
        if (connections[i].group == -1) {
            connections[i].group = group_number;
            flow(i,connections,group_number);
        } else if(connections[i].group !=group_number) {
            std::cout<<"SOMETHING IS WRONG HERE"<<std::endl;exit(1);
        }
    }
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

    id connections[size];
    unsigned cur_int;
    for(unsigned i=0;i<size;i++){
        input_file>>cur_int;
        if(cur_int!=i||input_file.get()!=' '||input_file.get()!='<'||input_file.get()!='-'||input_file.get()!='>'||input_file.get()!=' ') {
            std::cout<<"SOMETHING WRONG HERE"<<std::endl;return 1;
        }
        do {
           input_file>>cur_int;
           connections[i].neighbours.push_back(cur_int);
        } while(input_file.get()==','&&input_file.get()==' ');
    }
    unsigned group_count=0;
    for (unsigned i=0;i<size;i++)
        if (connections[i].group ==-1){
            group_count++;
            flow(i,connections,i);
        }
    std::cout<<"THERE ARE "<<group_count<<" GROUPS"<<std::endl;
    return 0;
}
