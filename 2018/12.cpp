#include <iostream>
#include <fstream>
#include <vector>

struct dll_elt {
    dll_elt*prev,*next;
    long pot;
    bool has_plant=false,gets_plant=false;
    dll_elt(dll_elt*_prev,dll_elt*_next,long _pot,bool _gets_plant){prev=_prev;next=_next;pot=_pot;gets_plant=_gets_plant;}
};

struct dll {
    dll_elt *first,*last;
    dll(bool _plant){
        first=last=new dll_elt(NULL,NULL,0,_plant);
    }
    void push_back(bool _plant){
        last = new dll_elt(last,NULL,last->pot+1,_plant);
        last->prev->next=last;
    }
    void push_front(bool _plant){
        first = new dll_elt(NULL,first,first->pot-1,_plant);
        first->next->prev = first;
    }
    void consolidate() {
        dll_elt* cur = first;
        do {
             cur->has_plant = cur->gets_plant;
             cur = cur->next;
        } while(cur!= NULL);
    }
    void print(std::ostream& out) {
        dll_elt* cur=first;
        do {
             out<<(cur->has_plant ? '#':'.');
             cur = cur->next;
        } while(cur!= NULL);
        out<<std::endl;
    }
    void Print(std::ostream& out) {
        dll_elt* cur=first;
        do {
             out<<cur->pot<<(cur->has_plant ? '#':'.')<<' ';
             cur = cur->next;
        } while(cur!= NULL);
        out<<std::endl;
    }
    long step(bool *rules) {
        if(rules[0]){std::cout<<"This will escalate too much"<<std::endl;exit(1);}
        // resize if necessary;
        if(rules[1] && first->has_plant){
            push_front(false);
            push_front(true);
        } else if((rules[2] && first->has_plant && !first->next->has_plant) || (rules[3] && first->has_plant && first->next->has_plant))
            push_front(true);

        if(rules[16] && last->has_plant){
            push_back(false);
            push_back(true);
        } else if((rules[8] && last->has_plant && !last->prev->has_plant) || (rules[24] && last->has_plant && last->prev->has_plant))
            push_back(true);
        dll_elt* cur = first;
        do {
            unsigned score = 16*(cur->prev && cur->prev->prev && cur->prev->prev->has_plant) + 8*(cur->prev && cur->prev->has_plant) + 4*cur->has_plant + 2*(cur->next && cur->next->has_plant) + (cur->next && cur->next->next && cur->next->next->has_plant);
            cur->gets_plant = rules[score];
            cur = cur->next;
        } while(cur!=NULL);
        consolidate();
        long rtr = 0;
        cur=first;
        do {
            if(cur->has_plant)rtr+=cur->pot;
            cur = cur->next;
        }while(cur!=NULL);
        return rtr;
    }
};

int main(int argc, char** argv){
    if(argc<2){std::cout<<"usage: {program} {input_file} [{rounds}]"<<std::endl;return 0;}
    unsigned rounds = (argc < 3 ? 20ul : std::stoul(argv[2]));
    std::ifstream input_file(argv[1]);
    if(!input_file.good()){std::cout<<argv[1]<<" is bad"<<std::endl;return 1;}

    while(input_file.get()!=':');
    if(input_file.get()!=' '){std::cout<<"  expected"<<std::endl;return 1;}
    dll pots(input_file.get()=='#');
    bool done;
    do {
        done=false;
        switch(input_file.get()) {
            case '#':
                pots.push_back(true);
                break;
            case '.':
                pots.push_back(false);
                break;
            case '\n':
                done=true;
                break;
            default: std::cout<<"one of #.\\n expected"<<std::endl;return 1;
        }
    }while(!done);
    pots.consolidate();
    pots.print(std::cout);

    bool rules[32];
    for(int i=0;i<32;i++)rules[i]=false;
    if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
    for(;;){
        char a=input_file.get();
        if (a!='.'&&a!='#')break;
        unsigned index=0;
        while(a!=' '){
            index = index*2 + (a=='#');
            a=input_file.get();
        }
        if(input_file.get()!='='||input_file.get()!='>'||input_file.get()!=' '){std::cout<<"=>  expected"<<std::endl;return 1;}
        rules[index]=(input_file.get()=='#');
std::cout<<index<<"->"<<rules[index]<<std::endl;
        if(input_file.get()!='\n'){std::cout<<"newline expected"<<std::endl;return 1;}
    }

    for(int i=1;i<=rounds;i++){
        std::cout<<i<<'('<<pots.step(rules)<<')'<<':';
        pots.print(std::cout);
    }
    pots.Print(std::cout);// for part 2, give big number of rounds, and extrapolate

    return 0;
}
