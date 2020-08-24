#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>

typedef std::vector<std::pair<unsigned short, std::vector<unsigned short>>> operation;

// Overload << for fun.
std::ostream& operator<< (std::ostream& os,const std::vector<unsigned short>& val){
    if(!val.size()){os<<"HELP!"<<std::endl;return os;}
    for(unsigned i=0;i<val.size()-1;i++)os<<val[i]<<',';
    os<<val[val.size()-1];
    return os;
};

std::vector<unsigned short> read_file(char* file_name,std::vector<operation>& operations, unsigned short& e, unsigned short& element) {
    std::unordered_map<std::string,unsigned short> dictionary={};
    std::string line;
    element=-1;
    std::ifstream input_file;
    input_file.open(file_name);
    if(!input_file.good()){std::cout<<"BAD INPUT FILE"<<std::endl;exit(1);}
    do {
        std::getline(input_file, line);
        std::string fro, t;
        unsigned short from;
        unsigned i=0;
        if (line.size() == 0)break;
        if ('A'<=line[i]&&line[i]<='Z') {
            if ('a'<=line[i+1]&&line[i+1]<='z') {
                fro = line.substr(i,2);
                i+=2;
            } else {
                fro = line.substr(i,1);
                i++;
            }
        } else if(line[i]!='e') {
            std::cout<<"WEIRD CHAR "<<line[i]<<" FOUND AT "<<i<<std::endl;exit(1);
        } else {
            fro="e";
            i++;if(e!=element)e=element+1;
        }
        if(!dictionary.count(fro))dictionary[fro]=++element;
        from = dictionary[fro];

//std::cout<<std::endl<<fro<<'('<<from<<") => "<<std::flush;
        if (line[i++]!=' ' || line[i++]!='=' || line[i++]!='>' || line[i++]!=' ') {
            std::cout<<line[i-1]<<" WAS UNEXPECTED"<<std::endl;exit(1);
        }
        int size=0;
        unsigned short to;
        while(i<line.size()) {
             t="";
             if ('A'<=line[i]&&line[i]<='Z') {
                 if ('a'<=line[i+1]&&line[i+1]<='z') {
                     t = line.substr(i,2);
                     i+=2;
                 } else {
                     t = line.substr(i,1);
                     i++;
                 }
                 if(!dictionary.count(t))
                     dictionary[t]=++element;
                 if(!size) {
                     to=dictionary[t];
                 } else if (size==1) {
                     while(to>=operations.size())operations.emplace_back();
                     operations[to].emplace_back(std::piecewise_construct,std::make_tuple(from),std::make_tuple(1,dictionary[t]));
                 } else {
                     operations[to].back().second.push_back(dictionary[t]);
                 }
                 size++;
//std::cout<<t<<'('<<dictionary[t]<<") ";
             }
        };
    } while (line.size() > 0);
    std::getline(input_file, line);
    std::vector<unsigned short> goal={};
    unsigned short i=0;
    while ('A'<=line[i]&&line[i]<='Z') {
        std::string fro;
        if ('a'<=line[i+1]&&line[i+1]<='z') {
            fro = line.substr(i,2);
            i+=2;
        } else {
            fro = line.substr(i,1);
            i++;
        }
        if(!dictionary.count(fro)){std::cout<<"IMPOSSIBLE ELEMENT "<<fro<<" NEEDED"<<std::endl;exit(1);}
        goal.push_back(dictionary[fro]);
    };
    return goal;
}

struct dll_elt{
    unsigned short val=-1;
    dll_elt *next=NULL,*prev=NULL;
    dll_elt(){}
    dll_elt(const unsigned short _val){val=_val;}
    dll_elt(const unsigned short _val,dll_elt*const _prev){val=_val;prev=_prev;}
    dll_elt(const unsigned short _val,dll_elt*const _prev,dll_elt*const _next){val=_val;prev=_prev;next=_next;}
//    ~dll_elt(){std::cout<<"WHOOPS"<<val<<std::endl;}
};

struct dll {
    dll_elt *first=NULL,*last=NULL;
    dll(std::vector<unsigned short> values){
        last=first=new dll_elt;
        for(auto a:values)last=(last->next=new dll_elt(a,last));
        last->next=new dll_elt;
        last->next->prev = last;
        last = last->next;
    }
    ~dll(){std::cout<<"I AM BEING DESTRUCTED"<<std::endl;while(first!=NULL){dll_elt*next=first->next;delete first;first=next;}}// We just hope this is only called without replacements having occurred;
    void replace(const dll_elt*const first_to_be_replaced,const dll_elt*const last_to_be_replaced,const unsigned short replacement) const{
        first_to_be_replaced->prev->next =
        last_to_be_replaced->next->prev =
        new dll_elt(
            replacement,
            first_to_be_replaced->prev,
            last_to_be_replaced->next
        );
//std::cout<<'@'<<first_to_be_replaced<<' '<<last_to_be_replaced<<' '<<last_to_be_replaced->next->prev<<' '<<last_to_be_replaced->next<<' '<<last_to_be_replaced->next->prev->prev<<'='<<first_to_be_replaced->prev<<std::endl;
    }
    void reset(dll_elt*const old_first,dll_elt*const old_last) const {
if(old_last==old_last->next->prev)std::cout<<"REAL PANIC!"<<old_last<<std::endl;
        delete old_last->next->prev;
//std::cout<<old_last->val<<' '<<old_first->val<<std::endl;
        old_last->next->prev=old_last;
        old_first->prev->next=old_first;
    }
};

std::ostream& operator<< (std::ostream& os,const dll& mol){
    dll_elt* cur= mol.first->next;
    while(cur!=mol.last->prev&&cur!=mol.last){os<<cur->val<<',';cur=cur->next;}
    if(cur!=mol.last)os<<cur->val;
    return os;
};

void replace(const dll& molecule,const std::vector<operation> operations,dll_elt* cur_pos){
    static unsigned depth=0;
//    std::cout<<depth++<<':'<<std::flush;
    depth++;
//std::cout<<molecule<<std::endl;

    if(molecule.last->prev==molecule.first->next){
        std::cout<<"HURRAY!"<<molecule.first->next->val<<depth<<std::endl;
    } else {
        while(cur_pos!=molecule.last){
if(cur_pos->val >= operations.size()){std::cout<<"PANIC!"<<cur_pos->val<<std::endl;exit(1);}
//std::cout<<"HAY!"<<operations[cur_pos->val].size()<<std::flush;
            for(auto a:operations[cur_pos->val]){
                dll_elt* last=cur_pos;
                bool fits = true;
//std::cout<<"HEY!"<<a.second.size()<<std::flush;
                for(auto b:a.second){
                    last = last->next;
                    if(last->val != b){
                        fits = false;
                        break;
                    }
                }
                if(fits){
//std::cout<<" FITTING "<<a.first<<" IN FOR "<<cur_pos->val<<','<<a.second<<std::endl;
                    molecule.replace(cur_pos,last,a.first);
                    replace(molecule,operations,cur_pos->prev->next);
                    molecule.reset(cur_pos,last);
//std::cout<<"DOING A RESET "<<cur_pos<<','<<last<<" TO "<<molecule<<std::endl;
                }
            }
            cur_pos=cur_pos->next;
        }
    }
    depth--;
}



int main(int argc,char** argv) {
    if(argc<2){std::cout<<"USAGE WRONG"<<std::endl;return 0;}
    std::vector<operation> operations={};
    unsigned short e, numberOfElements;
    std::vector<unsigned short> goal = read_file(argv[1],operations,e,numberOfElements);
    std::cout<<"NEED TO GO FROM "<<e<<" TO "<<goal<<" OF SIZE "<<goal.size()<<" USING\n";
    for(int i=0;i<operations.size();i++)for(auto a:operations[i])std::cout<<i<<','<<a.second<<" <= "<<a.first<<std::endl;

    dll molecule(goal);
    std::cout<<molecule<<' '<<molecule.first<<std::endl;
    replace(molecule,operations,molecule.first->next);

    return 0;
}
