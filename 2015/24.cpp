#include <iostream>
#include <vector>
#include <fstream>

struct package{
    unsigned weight;
    short group=-1;
    package(unsigned _weight){weight=_weight;}
};

unsigned entangle(std::vector<package>& packages,unsigned* const grp_total,int cur_index_0,int cur_index_1,int cur_index_2,const unsigned goal_weight){
    static unsigned long grp_0_min_size=-1,grp_0_min_entanglement=-1,depth=0,grp_0_size=0;
    static bool done_0;
    ++depth;
    int* cur_index[3]={&cur_index_0,&cur_index_1,&cur_index_2};

    for(unsigned short grp=0;grp<3;grp++){
        while(*(cur_index[grp])>=0){
            if(grp_total[grp]>=goal_weight)break;
            if(grp==0&&done_0)done_0=false;
            if(packages[(*(cur_index[grp]))].group==-1&&grp_total[grp]+packages[(*(cur_index[grp]))].weight <= goal_weight){
                if(grp==0&&++grp_0_size>grp_0_min_size){/*std::cout<<"size of "<<*/grp_0_size--/*<<" is too big"<<std::endl*/;depth--;return grp_0_min_entanglement;}
                packages[(*(cur_index[grp]))].group=grp;
                grp_total[grp]+=packages[(*(cur_index[grp]))].weight;
                (*(cur_index[grp]))--;
                if(grp==0||!done_0)entangle(packages,grp_total,cur_index_0,cur_index_1,cur_index_2,goal_weight);
                ((*(cur_index[grp])))++;
                if(grp==0)grp_0_size--;
                packages[*(cur_index[grp])].group=-1;
                grp_total[grp]-=packages[(*(cur_index[grp]))].weight;
            }
            (*(cur_index[grp]))--;
        }
            unsigned short test_group_total[3]={0,0,0};
            for(auto a:packages){std::cout<<(a.group==1?'A':(a.group==0?'O':(a.group==2?'^':',')));if(a.group==0||a.group==1)test_group_total[a.group]+=a.weight;else if(a.group!=-1)std::cout<<"HELP!!"<<std::endl;}std::cout<<" => "<<grp_total[0]<<'='<<test_group_total[0]<<','<<grp_total[1]<<'='<<test_group_total[1]<<std::endl;
        if(grp_total[0]!=goal_weight){/*std::cout<<"SOMETHING IS WRONG HERE"<<cur_index<<<<std::endl;*/--depth;return grp_0_min_entanglement;}
    }
//    done_0=true;
    grp_total[0]=0;grp_total[1]=0;grp_0_size=0;
    unsigned long grp_0_entanglement=1;
    for(unsigned i=0;i<packages.size();i++){
        if(packages[i].group!=-1) {
            grp_total[packages[i].group]+=packages[i].weight;
            if(packages[i].group==0){
                grp_0_size++;
std::cout<<grp_0_entanglement<<"*="<<packages[i].weight<<std::endl;
                grp_0_entanglement*=packages[i].weight;
            } else {
                packages[i].group=-1;
            }
        }
    }
    if(grp_0_size<grp_0_min_size) {
        grp_0_min_size=grp_0_size;
        grp_0_min_entanglement=grp_0_entanglement;
        std::cout<<" A SMALLER GROUP OF SIZE "<<grp_0_size<<" WITH ENTANGLEMENT "<<grp_0_entanglement<<" FOUND"<<std::endl;
    } else if(grp_0_entanglement<grp_0_min_entanglement){std::cout<<"NEW ENTANGLEMENT OF "<<(grp_0_min_entanglement=grp_0_entanglement)<<" FOUND"<<std::endl;}else{std::cout<<std::endl<<':'<<grp_0_entanglement<<"<\n"<<grp_0_min_entanglement<<':'<<std::endl;}
    depth--;
    return grp_0_entanglement;
}

int main(int argc,char** argv){
    if(argc<2){std::cout<<"Usage: {program} {input_file} [{a-value}]"<<std::endl;return 0;}
    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.good()){std::cout<<"Input file "<<argv[1]<<" is not good"<<std::endl;return 1;}

    std::vector<package> packages={};
    unsigned input,total,group_total[2]={0,0};
    input_file>>input;
    while(!input_file.eof()) {
        total+=input;
        packages.emplace_back(input);
        input_file>>input;
    }
    int cur_index = packages.size()-1;
    std::cout<<" A TOTAL OF "<<total<<"=3*"<<total/3<<" IS FOUND "<<std::endl;
    std::cout<<"THE ANSWER IS "<<entangle(packages,group_total,cur_index,cur_index,cur_index,total/3)<<std::endl<<'{';
std::cout<<10439961859<<"DONE"<<std::endl;
    return 0;
}

