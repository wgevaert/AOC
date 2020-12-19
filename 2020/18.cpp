#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

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

ull_t calc_block_rules_1(const std::string& line, const u_t depth, size_t& index) {
    ull_t ans = 0, number;
    char op = '?';
    bool calc;
    while (line.size() > index) {
        calc = false;
        switch (line[index]) {
            case '(':
                index++;
                number = calc_block_rules_1(line, depth + 1, index);
                calc = true;
                break;
            case ')':
                index++;
                return ans;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                number = index;
                while (line[index] <= '9' && line[index] >= '0')
                    index++;
                number = std::stoull(line.substr(number, index-number));
                calc = true;
                break;
            case '+':
            case '*':
                op = line[index];
                index++;
                break;
            case ' ':
                index++;
                break;
            default:
                std::cerr << "error: " << line[index] << " unexpected" << std::endl;
                exit(1);
        }
        if (calc) {
            if (op == '?') {
                ans = number;
            } else if (op == '+') {
                ans += number;
            } else if (op =='*') {
                ans *= number;
            }
        }
    }
    return ans;
}

ull_t calc_block_rules_2(const std::string& line, const u_t depth, size_t& index) {
    if (verb_lvl > 5) {
        std::cout<<"Calcing "<<line.substr(index)<<" at depth "<<depth<<std::endl;
    }
    ull_t ans = 1, number = 0;
    size_t dummy;
    bool calc;
    while (line.size() > index) {
        calc = false;
        switch (line[index]) {
            case '(':
                index++;
                number += calc_block_rules_2(line, depth + 1, index);
                break;
            case ')':
                index++;
                if (number != 0)
                    ans *= number;
                if (verb_lvl > 5)
                    std::cout<<"Got "<<ans<<" at depth "<<depth<<std::endl;
                return ans;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                dummy = index;
                while(line[index] <= '9' && line[index] >= '0')
                    index++;
                number += std::stoull(line.substr(dummy, index - dummy));
                break;
            case '*':
                calc = true;
                index++;
                break;
            case '+':
            case ' ':
                index++;
                break;
            default:
                std::cerr << "error: " << line[index] << " unexpected" << std::endl;
                exit(1);
        }
        if (calc) {
            ans *= number;
            number = 0;
        }
        if (verb_lvl > 6)
            std::cout<<"Ans is "<<ans<<", number is "<<number<<" at depth "<<depth<<std::endl;
    }
    ans *= (number ? number : 1);
    if (verb_lvl > 5)
        std::cout<<"Got "<<ans<<" at depth "<<depth<<std::endl;
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

    ull_t ans1 = 0, ans2 = 0, index;
    std::string line;

    while(!input.eof()) {
        std::getline(input, line);
        if (verb_lvl > 2) {
            std::cout<<" evaluating "<<line<<std::endl;
        }
        if (line.size()) {
            index = 0;
            ans1 += calc_block_rules_1(line, 0, index);

            index = 0;
            ans2 += calc_block_rules_2(line, 0, index);
        }
    }

    std::cout<<ans1<<' '<<ans2<<std::endl;
    // do things with input.

    input.close();

    // do things without input
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
