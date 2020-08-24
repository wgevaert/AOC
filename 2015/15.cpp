#include <iostream>
#include <vector>
#include <fstream>

struct ingredient {
    int properties[5];
    unsigned short amount=0;
};

const unsigned short total_amount=100;

void read_ingredients(std::ifstream& input_file,std::vector<ingredient>& ingredients) {
    while(!input_file.eof()){
        ingredients.push_back(ingredient());
        for(int i=0;i<5;i++){
            for(int iter=0;iter<2;iter++)while(input_file.get()!=' ')if(input_file.eof())return;
            input_file>>ingredients.back().properties[i];
        }
        input_file.get();input_file.get();
    }
}


std::pair<unsigned,unsigned> get_score(std::vector<ingredient>& ings,unsigned cur_index,unsigned short leftover_amount) {
    unsigned static max_score = 0, max_with_500_cal;
    if(cur_index == ings.size()-1){
        ings[cur_index].amount = leftover_amount;
        unsigned total_score = 1;
        for(int i=0;i<4;i++){int score=0;for(unsigned j=0;j<ings.size();j++)score+=ings[j].properties[i]*ings[j].amount;if(score<0)score=0;total_score*=score;}
        int calories = 0;
        for(unsigned j=0;j<ings.size();j++)calories+=ings[j].properties[4]*ings[j].amount;
        if (calories == 500 && total_score > max_with_500_cal)max_with_500_cal=total_score;
//        std::cout<<total_score<<" with amounts ";for(int j=0;j<ings.size();j++)std::cout<<ings[j].amount<<',';std::cout<<std::endl;
        if (total_score > max_score)max_score=total_score;
    } else {
        for(int i=0;i<=leftover_amount;i++){
            ings[cur_index].amount=i;
            get_score(ings,cur_index+1,leftover_amount-i);
        }
    }
    return std::make_pair(max_score,max_with_500_cal);
}

int main(int argc,char**argv) {

    std::ifstream input_file;
    if(argc<2){std::cout<<"Usage: {program_name} {input_file}"<<std::endl;return 0;}
    input_file.open(argv[1]);
    if (!input_file.good()) {
        std::cout<<argv[1]<<" did not open correctly"<<std::endl;return 1;
    }

    std::vector<ingredient> ingredients={};
    read_ingredients(input_file,ingredients);
    for(ingredient a:ingredients)std::cout<<"Ingredient has capacity "<<a.properties[0]<<", durability "<<a.properties[1]<<", flavor "<<a.properties[2]<<", texture "<<a.properties[3]<<" and calories "<<a.properties[4]<<std::endl;

    std::pair<unsigned,unsigned> answer=get_score(ingredients,0,total_amount);
    std::cout<<answer.first<<' '<<answer.second<<std::endl;
    return 0;
}
