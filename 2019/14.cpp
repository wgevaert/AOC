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

struct unit {
    bool satisfied = false;
    bool used = false;
    long unsigned total_req=0;
    std::string name;
    long long unsigned ore_value=0;
    long long unsigned denom = 1;
    std::vector<jets> requirements={};
    std::vector<jets> used_in = {};
    unit(std::string nme) {name=nme;}
    unit(){
    //  std::cout<<"IM BEING UNINITIALISED"<<std::endl;
    }
};

void satisfy(unit& u, auto& map) {
    if(u.satisfied) {
        return;
    }
    else {
        for ( auto uit : u.requirements) {
            map[uit.thing_required].used_in.push_back(jets(uit.number_required, u.name));
            if (!map[uit.thing_required].satisfied) {
                satisfy(map[uit.thing_required],map);
            }
        }
        u.satisfied = true;
    }
}

void use(unit& u, auto& map) {
    if(u.used) {
        return;
    }
    else {
        for (auto uit : u.used_in) {
            if (!map[uit.thing_required].used) {
                use(map[uit.thing_required],map);
            }
            u.total_req += map[uit.thing_required].total_req * uit.number_required;
        }
        u.total_req = (u.total_req+u.denom - 1)/u.denom;// Round up.
        u.used = true;
    }
}

long long unsigned get_ore_required (int fuel_req, auto& map) {
    for (auto& it : map) {
        it.second.used = false;
        it.second.total_req = 0;
    }
    map["FUEL"].total_req = fuel_req;
    map["FUEL"].used = true;
    use(map["ORE"],map);
    return map["ORE"].total_req;
}

unsigned bin_search(unsigned top, unsigned bottom, auto& map) {
    while(top - bottom > 1) {
        if (get_ore_required((top+bottom)/2,map) > 1000000000000)
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
    myunits["ORE"].ore_value = 1;
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
    // Answer question 1.
    std::cout<<(dummy = get_ore_required(1,myunits))<<" ORE REQUIRED FOR 1 FUEL"<<std::endl;
    // Do a brute-force search, cuz I'm lazy.
    std::cout<<"CAN MAKE "<<bin_search(2*1000000000000/dummy, 1000000000000/dummy, myunits)<<" FUEL WITH 1000000000000 ORE";
    return 0;
}
