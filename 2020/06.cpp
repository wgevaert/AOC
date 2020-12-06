#include <iostream>
#include <fstream>
#include <chrono>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

int real_main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [-v {verbosity_level}] {input_file}" << std::endl;
        exit(1);
    }
    verb_lvl = argc > 3 && argv[1][0] == '-' && argv[1][1] == 'v' ? std::stoul(argv[2]) : 0;

    std::ifstream input(argv[argc - 1]);

    if (!input.good()) {
        std::cerr << "Input file " << argv[argc - 1] << " did not open correctly" << std::endl;
        exit(1);
    }

    if (verb_lvl > 0) {
        std::cout << "Running in verbosity mode " << verb_lvl << std::endl;
    }

    ull_t ans1 = 0, ans2 = 0;
    bool seen[26];
    u_t asked[26];
    char question;
    for (int i=0; i<26; i++)
        seen[i] = false;
    for (int i=0; i<26; i++)
        asked[i] = 0;
    ull_t count = 0, index = 0;
    while(!input.eof()) {

        question = input.get();

        if (question >= 'a' && 'z' >= question) {
            if (!seen[question - 'a']){
                count++;
                seen[question - 'a'] = true;
            }
            if (asked[question - 'a'] == index) {
                asked[question - 'a']++;
            }
        } else if (question == '\n' && (input.peek() == '\n' || input.peek() == -1)) {
            ans1 += count;
            count = 0;
            for (int i=0; i<26; i++) {
                if (asked[i] == index+1) {
                    ans2++;
                }
                asked[i] = 0;
            }
            index=-1;
            for (int i=0; i<26; i++)
                seen[i] = false;
        } else if (question == '\n') {
            index++;
        } else {
            std::cerr << question << " with value " << static_cast<int>(question) << " was unexpected" << std::endl;
            exit(1);
        }
    }

    std::cout << ans1 << ' ' << ans2 << std::endl;;

    input.close();
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc, argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << std::endl;
}
