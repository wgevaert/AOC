#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstring>
#include <vector>
#include <tuple>
#include <algorithm>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

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

    std::vector<u_t> reqs = {};
    std::vector<u_t> allowed[1000]; // an array of empty vectors.

    u_t size=0;

    // begin functin read_reqs
    do { // Read the fields
        std::string key;
        u_t l1, u1, l2, u2;
        std::getline(input, key, ':');
        if (!key.size()) {
            break;
        }
        if (input.get()!=' ') {
            std::cerr<<"' ' expected after "<<key<<'('<<key.size()<<')'<<std::endl;
            exit(1);
        }
        input >> l1;
        if (input.get()!='-') {
            std::cerr<<"'-' expected"<<std::endl;
            exit(1);
        }
        input >> u1;
        if (input.get()!=' '||input.get()!='o' ||input.get()!='r'||input.get()!=' ') {
            std::cerr<<"' or ' expected"<<std::endl;
            exit(1);
        }
        input >> l2;
        if (input.get()!='-') {
            std::cerr<<"'-' expected"<<std::endl;
            exit(1);
        }
        input>>u2;
        if (input.get()!='\n') {
            std::cerr<<"newline expected"<<std::endl;
            exit(1);
        }
        if (verb_lvl > 2)
            std::cout<<'('<<size<<')'<<key<<':'<<l1<<'-'<<u1<<','<<l2<<'-'<<u2<<std::endl;

        if (u1 >= 1000 || u2 >= 1000) {
            std::cerr<<u1<<','<<u2<<'>'<<1000<<std::endl;
            exit(1);
        }

        for (u_t i=l1;i<=u1;i++)
            allowed[i].emplace_back(size);
        for (u_t j=l2;j<=u2;j++)
            allowed[j].emplace_back(size);

        if (key.substr(0, strlen("departure")) == "departure")
            reqs.emplace_back(size);

        size++;

    } while (input.peek()!='\n');
    //end function read_reqs

    while(input.get()!=':' && !input.eof()); // gobble "your ticket:"

    if (input.get()!='\n') {
        std::cerr<<"newline expected"<<std::endl;
        exit(1);
    }

    u_t val;
    std::vector<u_t> ticket, my_ticket;
    do { // read my ticket
        input >> val;
        my_ticket.emplace_back(val);
    } while (input.get() == ',');

    while (input.get()!=':' && !input.eof()); // gobble "nearby tickets:"

    if (input.get() != '\n') {
        std::cerr << "newline expected" << std::endl;
        exit(1);
    }

    ull_t ans1 = 0;

    // The possibilities for my_map: First number is number of possibilities, second number is original index, then vector with "true" for possibilities.
    std::vector<std::tuple<u_t, u_t, std::vector<bool>>> possible_indices(size);

    for (u_t i=0;i<possible_indices.size();i++)
        // Initialise: all indices are still possible
        possible_indices[i] = std::make_tuple(size, i, std::vector<bool>(size, true));

    // begin function read_tickets
    while (input.peek() != -1 && input.peek() != '\n') { // Read the other tickets
        ticket.clear();
        u_t field;
        bool good = true;
        do {
            input >> field;
            ticket.emplace_back(field);
            if (field >= 1000 || !allowed[field].size()) {
                good = false;
                ans1 += field;
                if (verb_lvl > 9)
                    std::cout<<"Adding "<<field<<" to ans1"<<std::endl;
            }
        } while (input.get() == ',');

        if (good) {
            if (ticket.size() != size) {
                std::cerr<<"Expected "<<ticket.size()<<'='<<size<<std::endl;
                exit(1);
            }

            for (u_t i=0; i<ticket.size(); i++) {
                u_t ind = 0;
                for (u_t j=0; j<size; j++) {
                    if (j == allowed[ticket[i]][ind]) {
                        ind = (ind + 1 < allowed[ticket[i]].size() ? ind + 1 : 0);
                    } else {
                        if (std::get<2>(possible_indices[i])[j]) {
                            if (verb_lvl > 6)
                                std::cout<<"removing " << j << " from (" << i <<") because of "<<ticket[i]<<" not being in bounds"<<std::endl;

                            std::get<0>(possible_indices[i]) --;
                            std::get<2>(possible_indices[i])[j] = false;
                        }
                    }
                }
            }
        }
    }
    //end function read_tickets

    input.close();

    std::cout << ans1 << std::endl;

    std::vector<u_t> my_map(size, -1);

    auto not_done = possible_indices.begin();

    while (not_done != possible_indices.end()) {
        std::sort(not_done, possible_indices.end());

        while (not_done != possible_indices.end() && std::get<0>(*not_done) == 1) {
            u_t solution=-1;
            for (u_t i=0; i<std::get<2>(*not_done).size(); i++) {
                if(std::get<2>(*not_done)[i]) {
                    solution = i;
                    break;
                }
            }
            if (solution ==static_cast<u_t>(-1)) {
                std::cerr << "No solution found..." << std::endl;
                exit(1);
            }
            if (verb_lvl > 3) {
                std::cout << std::get<1>(*not_done) << " is put at " << solution << std::endl;
            }
            my_map[solution] = std::get<1>(*not_done);

            // Note that I increase not_done here.
            for (auto a = ++not_done; a != possible_indices.end(); a++) {
                if (std::get<2>(*a)[solution]) {
                    std::get<2>(*a)[solution] = false;
                    std::get<0>(*a)--;
                }
            }
        }

        if (verb_lvl > 7) {
            std::cout<<"Current map:"<<std::endl;
            for (u_t i=0;i<my_map.size();i++) {
                if (my_map[i]!=static_cast<u_t>(-1))
                    std::cout<<i<<"->"<<my_map[i]<<' ';
            }
            std::cout<<std::endl;
        }
        if (verb_lvl > 9) {
            std::cout<<"Remaining parts:"<<std::endl;
            for (auto a = not_done; a != possible_indices.end(); a++) {
                std::cout<<std::get<1>(*a)<<" has "<<std::get<0>(*a)<<": ";
                for (u_t i=0;i<std::get<2>(*a).size();i++)
                    if (std::get<2>(*a)[i])
                        std::cout<<i<<' ';
                std::cout<<std::endl;
            }
        }
    }

    ull_t ans2 = 1;
    for (auto a:reqs) {
        ans2 *= my_ticket[my_map[a]];
        if (verb_lvl > 3) {
            std::cout<<"Giving ("<<a<<") at "<<my_map[a]<<" with value "<<my_ticket[my_map[a]]<<", ans is now "<<ans2<<std::endl;
        }
    }

    std::cout<<ans2<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
