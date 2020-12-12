#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <array>
#include <string>
#include <tuple>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

const std::array<std::pair<u_t,u_t>,8> directions = {
    std::make_pair(-1,-1),
    std::make_pair(-1,0),
    std::make_pair(-1,1),
    std::make_pair(0,1),
    std::make_pair(1,1),
    std::make_pair(1,0),
    std::make_pair(1,-1),
    std::make_pair(0,-1)
};

typedef struct field_elt {
    u_t i,j, n_cnt[2]={0,0};
    bool occupied = false;
    field_elt(u_t I, u_t J): i{I},j{J} {};
    size_t neighbours[2][8];
} field_elt;

const field_elt border = {static_cast<u_t>(-1),static_cast<u_t>(-1)};

std::vector<field_elt> read(std::istream& in);

void print(u_t ind, const std::vector<field_elt>& field) {
    size_t index = 1, i = 0;
    while (index < field.size()) {
        u_t j=0;
        while (index < field.size() && field[index].i == i) {
            while (j != field[index].j) {
                // use space instead of '.', nobody will notice those missing at end of lines
                std::cout<<' ';
                j++;
            }
            j++;
            std::cout<<(field[index].occupied ? static_cast<char>('0' + field[index].n_cnt[ind]) : '.');
            index++;
        }
        std::cout<<'\n';
        if (index < field.size())
            i = field[index].i;
    }
    std::cout<<std::endl;
}

u_t simulate (const u_t part, const u_t lim, std::vector<field_elt> field)
{
    bool changed;
    u_t ind = 0;
    do {
        changed = false;
        for (int i=1;i<field.size();i++)
            field[i].n_cnt[1-ind] = field[i].n_cnt[ind];

        for (int i=1;i<field.size();i++) {
            if (field[i].occupied && field[i].n_cnt[ind] >= lim) {
                for (u_t k=0;k<8;k++) {
                    field[field[i].neighbours[part][k]].n_cnt[1-ind] --;
                }
                changed = true;
                field[i].occupied = false;
            } else if (!field[i].occupied && field[i].n_cnt[ind] == 0) {
                for (u_t k=0;k<8;k++) {
                    field[field[i].neighbours[part][k]].n_cnt[1-ind] ++;
                }
                changed = true;
                field[i].occupied = true;
            }
        }
        ind = 1-ind;
        if (verb_lvl > 3) {
            print(ind, field);
        }
    } while(changed);
    u_t ans=0;
    for (int i=1;i<field.size();i++)
        if (field[i].occupied)
            ans++;
    return ans;
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
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<field_elt> field = read(input);
    input.close();

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Input duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;

    start = std::chrono::high_resolution_clock::now();

    auto ans1=simulate(0, 4, field);

    stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Part 1 duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;

    start = std::chrono::high_resolution_clock::now();

    auto ans2=simulate(1, 5, field);

    stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Part 2 duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;

    std::cout<<ans1<<' '<<ans2<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}

// Put the i/o-noise all the way over here
std::vector<field_elt> read(std::istream& input) {
    std::vector<std::string> input_field;

    std::vector<std::vector<u_t>> pos;

    while(!input.eof()) {
        std::getline(input, input_field.emplace_back());
        pos.emplace_back(input_field.back().size(), 0);
    }

    if (!input_field.back().size()) {
        input_field.pop_back();
        pos.pop_back();
    }

    std::vector<field_elt> field = {border};

    std::vector<std::tuple<u_t, std::pair<u_t,u_t>, std::pair<u_t,u_t>>> connections1, connections2;

    for (u_t i=0;i<input_field.size();i++) {
        for (u_t j=0;j<input_field[i].size();j++) {
            if (input_field[i][j] == 'L') {
                pos[i][j] = field.size();
                field.emplace_back(i,j);
                for (u_t k=0;k<8;k++) {
                    u_t I=i + directions[k].first, J=j + directions[k].second;
                    if (
                        I >= input_field.size() ||
                        J >= input_field[I].size() ||
                        input_field[I][J] == '.'
                    ) {
                        field.back().neighbours[0][k] = 0;
                    } else {
                        connections1.emplace_back(k, std::pair(i,j), std::pair(I,J));
                    }
                    while (
                        I < input_field.size() &&
                        J < input_field[I].size() &&
                        input_field[I][J] == '.'
                    ) {
                        I += directions[k].first;
                        J += directions[k].second;
                    }
                    if (I < input_field.size() && J < input_field[I].size() && input_field[I][J] == 'L') {
                        connections2.emplace_back(k, std::pair(i,j), std::pair(I,J));
                    } else {
                        field.back().neighbours[1][k] = 0;
                    }
                }
            } else if (input_field[i][j] != '.') {
                std::cerr<<"only 'L' or '.' expected, "<<input_field[i][j]<<'('<<static_cast<int>(input_field[i][j])<<')'<<" received"<<std::endl;
                exit(1);
            }
        }
    }

    for (auto a: connections1) {
        u_t k = std::get<0>(a);
        size_t index = pos[std::get<1>(a).first][std::get<1>(a).second], second_index = pos[std::get<2>(a).first][std::get<2>(a).second];
        field[index].neighbours[0][k] = second_index;
    }

    for (auto a: connections2) {
        u_t k = std::get<0>(a);

        size_t index = pos[std::get<1>(a).first][std::get<1>(a).second], second_index = pos[std::get<2>(a).first][std::get<2>(a).second];
        if (verb_lvl > 10)
            std::cout<<"Connecting "<<field[index].i<<','<<field[index].j<<" to "<<field[second_index].i<<','<<field[second_index].j<<" via "<<directions[k].first<<','<<directions[k].second<<std::endl;

        field[index].neighbours[1][k] = second_index;
    }

    return field;
}
