#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

void read_or_die(std::string pattern, std::istream& input) {
    for (auto a:pattern) {
        if (a != input.get()) {
            std::string parsed_pattern = "";
            for (auto b:pattern) {
                if (b < ' ' || b > 126)
                    parsed_pattern += '?';
                else
                    parsed_pattern += b;
            }
            std::cerr<<'\''<<parsed_pattern<<'\''<<" expected"<<std::endl;
            exit(1);
        }
    }
}

int real_main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr<<"Usage: "<<argv[0]<<" [-v {verbosity_level}] {input_file}"<<std::endl;
        exit(1);
    }
    verb_lvl = argc > 3 && argv[1][0] == '-' && argv[1][1] == 'v' ? std::stoul(argv[2]) : 0;

    std::ifstream input(argv[argc - 1]);

    if (!input.good()) {
        std::cerr<<"Input file "<<argv[argc - 1]<<" did not open correctly"<<std::endl;
        exit(1);
    }

    if (verb_lvl > 0) {
        std::cout<<"Running in verbosity mode "<<verb_lvl<<std::endl;
    }

    std::unordered_map<std::string, uint32_t> ingredients;
    std::unordered_map<std::string, uint32_t> allergens;

    std::vector<std::vector<bool>> meal_has_ingredients;

    std::vector<std::vector<uint32_t>> allergen_in_meal;

    uint32_t index = 0;

    while (!input.eof() && input.peek()!=-1) {
        std::string ingredient, all_allergens, allergen;

        std::getline(input, ingredient, ' ');

        if (input.eof()) {
            break;
        }

        meal_has_ingredients.emplace_back(ingredients.size(), false);
        do {
            if (ingredients.find(ingredient) == ingredients.end()) {
                ingredients[ingredient] = ingredients.size();
                for (auto& v : meal_has_ingredients) {
                    v.resize(ingredients.size(), false);
                }
            }
            meal_has_ingredients[index][ingredients[ingredient]] = true;
            std::getline(input, ingredient, ' ');
        } while(ingredient != "(contains");

        std::getline(input, all_allergens, ')');
        uint32_t prev=0;

        do {
            allergen = "";
            while(all_allergens[prev]>='a'&&'z'>=all_allergens[prev]) {
                allergen += all_allergens[prev++];
            }
            while (prev < all_allergens.size() && all_allergens[prev]!=')'&&(all_allergens[prev] < 'a' || all_allergens[prev] > 'z'))
                prev++;
            if (allergens.find(allergen) == allergens.end()) {
                allergens[allergen] = allergen_in_meal.size();
                allergen_in_meal.emplace_back(0);
            }
            if (verb_lvl > 7)
                std::cout << "setting "<<index<<" into "<<allergens[allergen]<<" of "<<allergen<<std::endl;
            allergen_in_meal[allergens[allergen]].emplace_back(index);

        } while (prev < all_allergens.size() && all_allergens[prev]!=')');

        read_or_die("\n", input);

        index++;
    }

    input.close();

    if (verb_lvl > 4) {
        std::cout<<"Read the following ingredients:\n";
        for (auto a: ingredients) {
            std::cout<<a.first<<'('<<a.second<<")\n";
        }
        std::cout<<"Read the following allergens:\n";
        for (auto a:allergens) {
            std::cout<<a.first<<'('<<a.second<<")\n";
        }
        std::cout<<"Meals have the following ingredients:"<<std::endl;
        uint32_t i=0;
        for (auto a:meal_has_ingredients) {
            std::cout<<i++<<':';
            for (u_t i=0;i<a.size();i++)
                if (a[i])
                    std::cout<<i<<' ';
            std::cout<<std::endl;
        }
        std::cout<<"Allergens are in the following meals:"<<std::endl;
        i=0;
        for (auto a:allergen_in_meal) {
            std::cout<<i++<<':';
            for (auto b:a)
                std::cout<<b<<' ';
            std::cout<<std::endl;
        }
    }

    std::vector<std::pair<std::vector<std::string>,std::string>> dangerous_ings={};

    uint64_t ans1=0;

    for (auto ingredient:ingredients) {
        uint32_t i=0;
        std::vector<bool> has_allergens(allergens.size(), true);
        for (const auto& a: allergen_in_meal) {
            for (const auto b:a)
                if (!meal_has_ingredients[b][ingredient.second]) {
                    has_allergens[i] = false;
                }
            ++i;
        }
        std::vector<std::string> alls = {};
        for (const auto all:allergens)
            if (has_allergens[all.second]) {
                alls.emplace_back(all.first);
            }
       if (!alls.size()) {
           for (auto a:meal_has_ingredients)
               if (a[ingredient.second])
                   ans1++;
       } else {
           dangerous_ings.emplace_back(alls, ingredient.first);
       }
    }

    do {
        for (const auto& d:dangerous_ings) {
            if (d.first.size() == 1) {
                for (auto& e : dangerous_ings) {
                    if (d.second != e.second && e.first.size()>1){
                        for (auto a=e.first.begin();a!=e.first.end();a++) {
                            if (*a == d.first.back()) {
                                e.first.erase(a);
                                break;
                            }
                        }
                    }
                }
            }
        }
        std::sort(
            dangerous_ings.begin(),
            dangerous_ings.end(),
            [](auto& a, auto& b){
                return a.first.size() < b.first.size() || (a.first.size() == b.first.size() && a.first.back() < b.first.back());
            }
        );
    } while (dangerous_ings.back().first.size() > 1);

    if (verb_lvl > 2) {
        std::cout<<" Found the following dangerous ingredients:"<<std::endl;
        for (auto a:dangerous_ings){
            std::cout<<a.second<<':';
            for (auto b:a.first) {
                std::cout<<b<<',';
            }
        }
    }

    std::cout<<ans1<<std::endl;
    for (auto a:dangerous_ings) {
        std::cout<<a.second<<(a.second==dangerous_ings.back().second?"":",");
    }std::cout<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
