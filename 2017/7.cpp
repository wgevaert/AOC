#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

struct unit {
    std::string parent="";
    unsigned weight=0;long unsigned cumul_weight=0;
    std::vector<std::string> childs={};
};

bool read_line(auto& input_file, auto& container, std::string& my_name) {
    input_file>>my_name;
    if (input_file.eof()) {
        return false;
    }

    if(input_file.get()!=' '||input_file.get()!='('){
        std::cerr<<"Something went wrong :("<<my_name.length()<<std::endl;return false;
    }

    // constructor is called if necessary
    input_file>>container[my_name].weight;
    if(!container[my_name].weight||input_file.get()!=')'){
        std::cerr<<"Something went wrong :P"<<std::endl;return false;
    }
    std::string temp_child;
    switch(input_file.get()) {
        case ' ':
            if (input_file.get()!='-'||input_file.get()!='>'||input_file.get()!=' ') {
                std::cerr<<"Something went wrong :O"<<std::endl;return false;
            }
            for(;;){
                input_file>>temp_child;
                bool last = temp_child.back()!=',';
                if(!last)
                    temp_child.pop_back();
                container[my_name].childs.push_back(temp_child);
                container[temp_child].parent = my_name;
                if (last) {
                    if (input_file.get()!='\n'&&!input_file.eof()) {
                        std::cerr<<"Something went wrong D:"<<temp_child<<std::endl;return false;
                    }
                    return true;
                }
            }
            break;
        case '\n':
            return true;
    }
    return false;
}

void calculate_weights(auto& container, std::string name) {
    unsigned answer = 0, balance=0;
    bool balanced = true;
    for (auto i : container[name].childs) {
        if (container[i].cumul_weight==0) {
            calculate_weights(container,i);
        }
        std::cout<<i<<','<<container[i].weight<<','<<container[i].cumul_weight<<std::endl;
        unsigned cur_weight = container[i].weight + container[i].cumul_weight;
        if(!balance)
            balance=cur_weight;
        else if(balance!=cur_weight) {
            std::cout<<"ON "<<name<<": "<<balance<<" IS NOT EQUAL TO "<<cur_weight<<std::endl;
            balanced=false;
        }
        answer +=container[i].weight + container[i].cumul_weight;
    }
    if (!balanced) {
        std::cout<<"WEIGHTS FOR "<<name<<": ";
        for (auto i :container[name].childs) {
            std::cout<<i<<'='<<container[i].weight+container[i].cumul_weight<<'('<<container[i].weight<<')'<<' ';
        }std::cout<<std::endl;
    }
    container[name].cumul_weight = answer;
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
    std::unordered_map<std::string,unit> my_units;
    std::string my_name;
    while(read_line(input_file, my_units, my_name));

    while(my_units[my_name].parent !=""){
        my_name=my_units[my_name].parent;
    }
    std::cout<<"The bottom program is "<<my_name<<std::endl;

    calculate_weights(my_units,my_name);
    std::cout<<my_units[my_name].cumul_weight<<std::endl;

    return 0;
}
