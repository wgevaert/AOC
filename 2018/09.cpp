#include <iostream>
#include <vector>

struct dll_elt {
    dll_elt *prev,*next;
    unsigned val;
    dll_elt(dll_elt* _prev,dll_elt* _next, unsigned _val){prev=_prev;next=_next;val=_val;}
};

struct dll {
    dll_elt *cur;
    dll(unsigned init_val){
        cur=new dll_elt(NULL,NULL,init_val);
        cur->prev=cur->next=cur;
    }
    ~dll(){
        dll_elt* unit=cur->next;
        while(unit!=cur){
            dll_elt* tmp=unit;unit=unit->next;delete tmp;
        }
        delete cur;
    }
    unsigned long apply(unsigned marble) {
        if(marble%23) {
            cur->next->next = new dll_elt(cur->next,cur->next->next,marble);
            cur = cur->next->next;
            cur->next->prev = cur;
            return 0;
        } else {
            dll_elt* to_be_removed = cur->prev->prev->prev->prev->prev->prev->prev;
            to_be_removed->prev->next = cur = to_be_removed->next;
            cur->prev = to_be_removed->prev;
            unsigned long rtr=to_be_removed->val + marble;
            delete to_be_removed;
            return rtr;
        }
    }
};

int main(int argc, char** argv){
    if(argc<3){std::cout<<"usage: {program} {players} {marbles}"<<std::endl;return 0;}
    unsigned player_cnt = std::stoul(argv[1]), marbles = std::stoul(argv[2]), cur_marble=0, cur_player=0;
    unsigned long* players=new unsigned long[player_cnt];
    for(unsigned i=0;i<player_cnt;i++)players[i]=0;
    dll circle(cur_marble);
    while(++cur_marble <= marbles) {
        players[cur_player]+=circle.apply(cur_marble);
        cur_player=(cur_player+1)%player_cnt;
    }
    unsigned max_score=0,winning_player;
    for(unsigned i=0;i<player_cnt;i++){
        if(players[i]>max_score){max_score=players[i];winning_player=i+1;}
    }
    delete[] players;
    std::cout<<" a max score of "<<max_score<<" can be achieved by player "<<winning_player<<std::endl;
    return 0;
}
