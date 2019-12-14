#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

struct jets {
    unsigned int number_required;
    std::string thing_required;
    jets(unsigned number,std::string thing) {
        number_required = number;
        thing_required = thing;
    }
    jets(){};
};

struct meta_jets;

struct unit {
    bool satisfied = false;
    bool used = false;
    long unsigned total_req=0;
    std::string name;
    long long unsigned denom = 1;
    std::vector<jets> requirements={};
    std::vector<meta_jets> used_in = {};
    unit(std::string nme) {name=nme;}
    unit(){
    //  std::cout<<"IM BEING UNINITIALISED"<<std::endl;
    }
};

struct meta_jets {
    unsigned int number_required;
    unit* thing_required;
    meta_jets(unsigned number,unit* thing) {
        number_required = number;
        thing_required = thing;
    }
    meta_jets(){};
};

void satisfy(unit& u, auto& map) {
    if(u.satisfied) {
        return;
    }
    else {
        for ( auto uit : u.requirements) {
            map[uit.thing_required].used_in.push_back(meta_jets(uit.number_required, &u));
            if (!map[uit.thing_required].satisfied) {
                satisfy(map[uit.thing_required],map);
            }
        }
        u.satisfied = true;
    }
}

void top_sort(std::vector<unit*>& top_sorted, auto& map, unit& u) {
    if(u.used) {
        return;
    }
    else {
        for (auto uit : u.used_in) {
            if (!uit.thing_required->used) {
                top_sort(top_sorted, map, *uit.thing_required);
            }
        }
        top_sorted.push_back(&u);
        u.used = true;
    }
}

long long unsigned get_ore_required (int fuel_req, std::vector<unit*>& top_sorted) {
    for (unit* unt : top_sorted) {
        unt->total_req = 0;
        if (unt->name == "FUEL") {
            unt->total_req = fuel_req;
            continue;
        }
        for (auto in : unt->used_in ) {
            unt->total_req += in.number_required * in.thing_required->total_req;
        }
        unt->total_req = (unt->total_req+unt->denom - 1)/unt->denom;
    }
    std::cout<<"FOR "<<fuel_req<<" YOU NEED "<<top_sorted[top_sorted.size()-1]->total_req<<" ORE"<<std::endl;
    return top_sorted[top_sorted.size()-1]->total_req;
}

unsigned bin_search(unsigned top, unsigned bottom, auto& top_sorted) {
    while(top - bottom > 1) {
        if (get_ore_required((top+bottom)/2,top_sorted) > 1000000000000)
            top = (top+bottom)/2;
        else
            bottom = (top+bottom)/2;
    }
    return (top+bottom)/2;
}

int main() {
    unsigned dummy;
    std::unordered_map<std::string,unit> myunits;
    std::string d_name, d_name2;//dummy_names
    std::string line;
    myunits["ORE"] = unit("ORE");
    myunits["ORE"].satisfied = true;
    std::cout<< "SANTA CAN MAKE THE FOLLOWING CHEMICALS:"<<std::endl;
    while (true) {// READ the input.
        std::getline(std::cin,line);
        if (line.length()==0)break;

        unsigned i=line.length()-1;
        d_name = "";
        while(line[i] != ' ') {
            d_name =line[i] + d_name;
            --i;
        }
        --i;
        myunits[d_name]=unit(d_name);
        int pow10=1;dummy=0;
        while (line[i] != ' ') {
            dummy += (line[i]-'0')*pow10;
            pow10*=10;
            --i;
        }
        std::cout<<dummy<<' '<<d_name<<std::endl;
        myunits[d_name].denom = dummy;

        i=0;
        while (line[i] != '=') {// Read requirements for d_name.
            dummy = 0;
            while (line[i] <= '9' && line[i] >= '0') {
                dummy = dummy*10+line[i]-'0';
                i++;
            }
            i++;
            d_name2="";
            while (line[i] <= 'Z' && line[i] >= 'A'){
                d_name2+=line[i];
                i++;
            }
            myunits[d_name].requirements.push_back(jets(dummy,d_name2));
            while(line[i]!=' ')i++;// Gobble comma.
            i++;
        }
    }
    std::cout<<"DONE READING"<<std::endl;
    // Process which unit needs which unit.
    satisfy(myunits["FUEL"],myunits);
    std::vector<unit*> top_sorted={};
    myunits["FUEL"].used = true;
    top_sorted.push_back(&myunits["FUEL"]);
    top_sort(top_sorted, myunits, myunits["ORE"]);
    for (auto i : top_sorted) {
        std::cout<<i->name<<"  <  ";
    }std::cout<<std::endl;
    // Answer question 1.
    std::cout<<(dummy = get_ore_required(1,top_sorted))<<" ORE REQUIRED FOR 1 FUEL"<<std::endl;
    // Do a brute-force search, cuz I'm lazy.
    std::cout<<"CAN MAKE "<<bin_search(2*1000000000000/dummy, 1000000000000/dummy, top_sorted)<<" FUEL WITH 1000000000000 ORE";
    return 0;
}
