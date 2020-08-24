#include <iostream>
#include <fstream>

struct ll_elt{
    ll_elt* prev=NULL;
    char val;
    ll_elt(ll_elt*_prev,char _val){val=_val;prev=_prev;}
};

struct ll{
    ll_elt* last=NULL;
    char back(){if(last==NULL)std::cout<<"HELP!"<<std::flush;return last->val;}
    char push_back(char val){last = new ll_elt(last,val);size++;return val;}
    char pop_back(){char ans=last->val;ll_elt* prev=last->prev;delete last;last=prev;size--;return ans;}
    unsigned long size=0;
    ~ll(){while(last!=NULL)pop_back();}
};

unsigned test(std::ifstream& input_file,unsigned removed){
    char a=input_file.get();
    ll list;
    while(a!='\n'){
        if (removed>=26 || (a-'a'!=removed && a-'A'!=removed)) {
            if(list.last!=NULL && (list.last->val^a)==('a'^'A')){
                list.pop_back();
            } else {
                list.push_back(a);
            }
        }
        a=input_file.get();
    }
    input_file.seekg(0);
    return list.size;
}

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file}"<<std::endl;return 0;}
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}
    std::cout<<" answer to 1 is "<<test(input_file,26)<<std::endl;
    unsigned min=-1;
    for(unsigned i=0;i<26;i++){
        unsigned test_result = test(input_file,i);
        if(test_result<min)min=test_result;
    }
    std::cout<<" answer to 2 is "<<min<<std::endl;;

    return 0;
}
