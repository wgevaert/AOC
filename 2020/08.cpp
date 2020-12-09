#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

// Operation function type.
typedef ll_t (*op_t)(ll_t val, ll_t& acum);

// Possible operations.
ll_t acc(ll_t val, ll_t& acum) {
    acum += val;
    return 1;
}

ll_t jmp(ll_t val, ll_t& acum) {
    return val;
}

ll_t nop(ll_t val, ll_t& acum) {
    return 1;
}

// Operation wrapper struct
typedef struct operation_s {
    op_t op;
    ll_t val;
    bool seen = false;
    operation_s(op_t op_i, ll_t val_i):op(op_i),val(val_i){};
    ll_t operator()(ll_t& acum) {
        return op(val,acum);
    }
} operation_t;

// See if program terminates, return true if it does.
// (note that the program does not have internal state, so when it sees
// an instruction for the second time, it is bound to repeat.)
bool attempt(std::vector<operation_t>& program, ll_t& acc_val) {
    acc_val = 0;
    ll_t pos = 0;

    for (auto& a:program) {
        // I also tried passing program by value, but then I got a corrupted double-linked list
        // (probrably a heap overflow), so just this loop is fine I guess...
        a.seen=false;
    }

    while (!program[pos].seen) {
        program[pos].seen = true;
        pos += program[pos](acc_val);
        if (pos == program.size()) {
            // A yippy termination!
            return true;
        } else if (pos < 0 || pos > program.size()) {
            std::cerr<<"Unexpected val of pos "<<pos<<" found"<<std::endl;
            exit(1);
        }
    }

    // We got stuck in a loop...
    return false;
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

    std::vector<operation_t> program = {};

    while (!input.eof()) {
        op_t oper;
        ll_t val, sign;
        switch (input.get()) {
            case 'a':
                if (input.get()!='c'||input.get()!='c') {
                    std::cerr<<"Did not read acc"<<std::endl;
                    exit(1);
                }
                oper = acc;
                break;
            case 'j':
                if (input.get()!='m'||input.get()!='p') {
                    std::cerr<<"Did not read jmp"<<std::endl;
                    exit(1);
                }
                oper = jmp;
                break;
            case 'n':
                if (input.get()!='o'||input.get()!='p') {
                    std::cerr<<"Did not read nop"<<std::endl;
                    exit(1);
                }
                oper = nop;
                break;
            default:
                input.unget();
                std::cerr<<"Found unexpected char with value "<<static_cast<int>(input.get())<<std::endl;
                exit(1);
        }

        if (input.get() != ' '){
           std::cerr<<"Did not read space"<<std::endl;
           exit(1);
        }

        switch (input.get()) {
            case '+':
                sign = 1;
                break;
            case '-':
                sign = -1;
                break;
            default:
                std::cerr<<"Expected a sign."<<std::endl;
                exit(1);
        }
        input>>val;
        val*=sign;
        program.emplace_back(oper, val);

        if (input.get()!='\n') {
            std::cerr<<"Newline expected"<<std::endl;
            exit(1);
        }
        if (input.peek()==-1)
             break;
    }

    input.close();

    ll_t acc_val=0;

    // Part 1
    attempt(program, acc_val);

    std::cout<<acc_val<<std::endl;

    // Part 2 (Yes I _could_ try a non-brute-force way, but I'm lazy)
    for (int i=0; i<program.size(); i++) {
        if (program[i].op == jmp) {
            program[i].op = nop;
            if (attempt(program, acc_val)) {
                std::cout<<"Changing "<<i<<" to nop resulted in terminating program with acc_val "<<acc_val<<std::endl;
            }
            program[i].op = jmp;
        } else if (program[i].op == nop) {
            program[i].op = jmp;
            if (attempt(program, acc_val)) {
                std::cout<<"Changing "<<i<<" to jmp resulted in terminating program with acc_val "<<acc_val<<std::endl;
            }
            program[i].op = nop;
        }
    }

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int res = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return res;
}
