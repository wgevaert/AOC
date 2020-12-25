#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

const uint64_t mod = 20201227;

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

uint64_t safe_mult(uint64_t a, uint64_t b, uint64_t m) { //Multiply without an overflow
    uint64_t res = 0;
    while (b>0) {
        if (b%2)res = (res+a) % m;
        a = (a+a) % m;
        b /= 2;
    }
    return res;
}

uint64_t euler_phi(uint64_t a) {
    uint64_t b = 2, phi = 1;
    while (b*b < a) {
        if (!(a%b)) {
            a /= b;
            phi *= (b-1);
            while (!(a%b)) {
                a /= b;
                phi *= b;
            }
        }
        if (b==2)b++; else b+=2;
    }
    if (a>1) phi*=(a-1);
    return phi;
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

    uint64_t n1,n2,loop1=0,loop2=0, unit=mod-1;

    input>>n1;
    input>>n2;

    input.close();

    uint64_t *pow7s = new uint64_t[mod];
    pow7s[0]=1;
    for (uint64_t i=1;i<=unit;i++) {
        pow7s[i] = (7 * pow7s[i-1]) % mod;
        if (pow7s[i] == n1) {
            loop1 = i;
            if (verb_lvl > 3)
                std::cout << "Loop1 is " << loop1 << std::endl;
            if (loop2)
                unit = safe_mult(loop1, loop2, euler_phi(mod));
        }
        if (pow7s[i] == n2) {
            loop2 = i;
            if (verb_lvl > 3)
                std::cout << "Loop2 is " << loop2 << std::endl;
            if (loop1)
                unit = safe_mult(loop1, loop2, euler_phi(mod));
        }
    }

    std::cout << pow7s[unit] << std::endl;

    delete[] pow7s;
    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
