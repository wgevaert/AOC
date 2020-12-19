#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

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

struct rule {
    size_t u1=-1,v1=-1,u2=-1,v2=-1;
    char test = '?';
    bool matches(const std::string in, size_t& index, const std::vector<rule>& rules) const {
        if (test != '?') {
            // note the increase of index
            return index < in.size() && in[index++] == test;
        } else if (u1 < rules.size()) {
            size_t prev_index = index;
            if (v1 < rules.size()) {
                if (rules[u1].matches(in, index, rules) && rules[v1].matches(in, index, rules)) {
                    return true;
                } else {
                    index = prev_index;
                    if (u2 < rules.size()) {
                        if (v2 < rules.size() && rules[u2].matches(in, index, rules) && rules[v2].matches(in, index, rules)) {
                            return true;
                        } else if (v2 >= rules.size() && rules[u2].matches(in, index, rules)) {
                            return true;
                        }
                    }
                }
            } else if (u2 >= rules.size() && rules[u1].matches(in, index, rules)) {
                return true;
            } else if (u2 < rules.size() && v2 >= rules.size()) {
                if (rules[u1].matches(in, index, rules)) {
                    return true;
                }
                index = prev_index;
                if (rules[u2].matches(in, index, rules)) {
                    return true;
                }
            }
        }
        return false;
    }
};

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

    std::vector<rule> rules;

    while(true) {
        if (input.peek() == '\n' || input.peek() == -1) {
            if (verb_lvl > 5) {
                std::cout << "done, we have " <<rules.size()<<" rules"<< std::endl;
            }
            break;
        }
        size_t index;
        input >> index;

        if (verb_lvl > 6)
            std::cout<<"read index "<<index<<std::endl;

        if (index >= rules.size()) {
            rules.resize(index + 1);
        }

        read_or_die(": ", input);
        if (input.peek() == '"') {
            input.get();
            input >> rules[index].test;
            read_or_die("\"", input);
        } else {
            input >> rules[index].u1;

            if (input.get() != '\n') {
                if (input.peek() == '|') {
                    read_or_die("| ", input);
                    input>>rules[index].u2;
                    if (input.get()!='\n'){
                        std::cerr<<"Expected 1 alt when 1 other given"<<std::endl;
                        exit(1);
                    }
                    continue;
                }
                input >> rules[index].v1;
                if (input.get() == ' ' && input.get() == '|' && input.get() == ' ') {
                    input >> rules[index].u2;
                    if (input.get() == ' ') {
                        input >> rules[index].v2;
                    } else {
                        std::cerr<<" Expected 2 alts when 2 others given"<<std::endl;
                        exit(1);
                    }
                    if (input.get() == '\n') {
                        continue;
                    } else {
                        std::cerr<<"Newline expected"<<std::endl;
                        exit(1);
                    }
                } else {
                    continue;
                }
            } else {
                continue;
            }
        }
        if (input.get() != '\n') {
            input.unget();
            std::cerr<<"Newline expected, got "<<static_cast<int>(input.get())<<std::endl;
            exit(1);
        }
    }

    if (verb_lvl > 3) {
        std::cout<<"Found these rules:"<<std::endl;

        size_t i=0;
        for (auto a : rules) {
            std::cout<<i++<<": ";
            if (a.test != '?') {
                std::cout<<'"'<<a.test<<'"'<<std::endl;
            } else {
                if (a.u1 < rules.size()) {
                    std::cout << a.u1 << ' ';
                    if (a.v1 < rules.size()) {
                        std::cout << a.v1 <<' ';
                        if (a.u2 < rules.size()) {
                            std::cout<<"| "<<a.u2<<' ';
                            if (a.v2 < rules.size()) {
                                std::cout<<a.v2<<' ';
                            } else {
                                std::cout<<"broken";
                            }
                        }
                    } else if (a.u2 < rules.size()) {
                        std::cout<<"| "<<a.u2;
                        if (a.v2 < rules.size()) {
                            std::cout<<"broken";
                        }
                    }
                } else {
                    std::cout<<"broken:"<<a.test<<' '<<a.u1<<' '<<a.v1<<' '<<a.u2<<' '<<a.v2;
                }
                std::cout<<std::endl;
            }
        }
    }

    std::vector<rule> rules2 = rules;

    size_t ans1 = 0, ans2 = 0, index;

    if (rules[0].u1 != 8 || rules[0].v1 != 11 || rules[0].u2 < rules.size() || rules[0].v2 < rules.size()) {
        std::cerr<<" This input cannot be handled for part 2: 0 is wrong"<<std::endl;
        exit(1);
    }
    if (rules[8].u1 != 42 || rules[8].v1 < rules.size() || rules[8].u2 < rules.size() || rules[8].v2 < rules.size()) {
        std::cerr<<" This input cannot be handled for part 2: 8 is wrong"<<std::endl;
    }
    if (rules[11].u1 != 42 || rules[11].v1 != 31 || rules[11].u2 < rules.size() || rules[11].v2 < rules.size()) {
        std::cerr<<" This input cannot be handled for part 2: 11 is wrong"<<std::endl;
    }

    // Yes I _should_ also check that 11 and 8 are not in other rules than 0.

    while (!input.eof()) {
        std::string in;
        std::getline(input, in);

        index = 0;
        if (rules[0].matches(in, index, rules) && index == in.size()) {
            ans1++;
            ans2++;
        } else {
            std::vector<size_t> rule2;
            bool done = false;
            // let's hope the threshold of 10 is big enough
            for (size_t c42s=2; c42s<=7 && !done; c42s++)
                for (size_t c31s=1; c31s<c42s && !done; c31s++) {
                    rule2.clear();
                    rule2.resize(c42s, 42);
                    rule2.resize(c42s+c31s, 31);
                    if (verb_lvl > 9) {
                        std::cout<<"at "<<c42s<<','<<c31s<<':';
                        for(auto a: rule2) {
                            std::cout<<' '<<a;
                        }
                        std::cout<<std::endl;
                    }
                    index = 0;
                    done = true;
                    for (auto a: rule2) {
                       if (!rules[a].matches(in, index, rules)) {
                           done = false;
                           break;
                       }
                    }
                    if (done && in.size() != index) {
                        done = false;
                    }
                    if (verb_lvl > 4 && done) {
                        std::cout<<in<<" matches for 2 at "<<c42s<<" 42s and "<<c31s<<" 31s"<<std::endl;
                    }
                }
            if (done) {
                ans2++;
            }
        }
    }

    input.close();

    std::cout<<ans1<<' '<<ans2<<std::endl;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
