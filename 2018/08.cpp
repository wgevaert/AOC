#include <iostream>
#include <fstream>
#include <vector>

const unsigned long no_value=-1;

struct tree_node {
    std::vector<tree_node> childs = {};
    std::vector<unsigned> metadata = {};
    unsigned long score=no_value;
};


tree_node read_node(std::ifstream& input_file) {
    static unsigned long total=0;
    tree_node rtr;
    unsigned child_cnt,meta_cnt;
    input_file>>child_cnt;
    if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;exit(1);}
    input_file>>meta_cnt;
    for(unsigned i=0;i<child_cnt;i++){
        rtr.childs.push_back(read_node(input_file));
    }
    for(unsigned i=0;i<meta_cnt;i++){
        input_file>>child_cnt;
        std::cout<<"1 could be "<<(total+=child_cnt)<<std::endl;
        rtr.metadata.push_back(child_cnt);
    }
    return rtr;
}

unsigned long get_score(tree_node node){
    unsigned long rtr=0;
    if(node.childs.size()) {
        for(auto a:node.metadata)if(a<=node.childs.size()){
            if(node.childs[a-1].score == no_value)
                node.childs[a-1].score = get_score(node.childs[a-1]);
            rtr +=node.childs[a-1].score;
        }
    } else {
        for(auto a : node.metadata)
            rtr+=a;
    }
    return rtr;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    tree_node root = read_node(input_file);
    std::cout<<" the answer to 2 is "<<get_score(root)<<std::endl;
    return 0;
}
