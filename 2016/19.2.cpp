#include <iostream>

struct ll_elt{
    ll_elt* next;
    unsigned number;
    ll_elt(ll_elt*_next,unsigned _number){
        next=_next;number=_number;
    }
};

struct linked_list{
private:
    ll_elt *thief,*poor_guy=NULL,*first;
    size_t thief_index,poor_guy_index;
    friend std::ostream& operator<<(std::ostream& out,linked_list& list);
    ll_elt *at(size_t a){ll_elt* rtr=first;for(size_t i=0;i<a;i++)rtr=rtr->next;return rtr;}
public:
    size_t size;
    linked_list(const size_t _size){
        size = _size;
        thief_index=0;
        poor_guy_index = size/2 ? size/2-1:0;
        ll_elt* cur=first=thief=new ll_elt(NULL,1);
        for(size_t i=size;i>1;i--){
            cur=new ll_elt(cur,i);
            if(i-1==poor_guy_index)poor_guy=cur;
        }
        thief->next = cur;
    }
    ~linked_list(){
        ll_elt* next=thief;
        do {ll_elt* tmp=next;next=next->next;delete tmp;}while(next!=thief);
    }
    void steal_and_advance(){
        while((poor_guy_index+1)%size != (thief_index +size/2)%size){
            poor_guy=poor_guy->next;
            poor_guy_index=(poor_guy_index+1)%size;
        }
        if(poor_guy->next == thief){std::cout<<"HELP!!"<<poor_guy_index<<' '<<thief_index<<','<<poor_guy->number<<' '<<thief->number<<std::endl;return;}
        ll_elt* tmp=poor_guy->next->next;
        if(first==poor_guy->next)first=poor_guy->next->next;
        delete poor_guy->next;
        poor_guy->next=tmp;
        size--;
        if(poor_guy_index==size){poor_guy_index=0;poor_guy=first;}
        if(thief_index==size)thief_index=0;
        else if(thief_index <= poor_guy_index)thief_index=(thief_index+1)%size;
        thief=thief->next;
    }
    void test(){
        ll_elt* unit=thief->next;while(true)unit=unit->next;
    }
};

std::ostream& operator<<(std::ostream& out,linked_list& list){
    if(list.size == 0)return out;
    ll_elt* cur=list.thief;
    while(cur->next!=list.thief){out<<cur->number<<' ';cur=cur->next;}
    std::cout<<cur->number;
    return out;
}

int main(int argc, char** argv) {
    if(argc<2){std::cout<<"usage: {program} {input}"<<std::endl;return 0;}
    size_t size = std::stoull(argv[1]);
    linked_list elves(size);
    while(elves.size > 1){
        elves.steal_and_advance();
    }
    std::cout<<elves<<std::endl;
    return 0;
}
