#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>/*
#include <unordered_map>*/

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

class file {
public:
    std::string name;
    ull_t size;
    file(std::string n, ull_t s) {size=s;name=n;}
};

class dir {
    // Note: Files are not actually used. Might as well not build this array...
    std::vector<file> files = {};
public:
    std::vector<dir> subdirs = {};
    std::string name;
    ull_t size = 0;
    dir(std::string n) {name=n;};
    void add_subdir(dir d) {size+=d.size; subdirs.push_back(d);}
    void add_file(std::string n, ull_t s) {size+=s;files.emplace_back(n,s);}
};

dir read_directory(std::istream& in) {
    std::string dirname;
    read_or_die("$ cd ",in);
    std::getline(in, dirname);
    if(verb_lvl>0)std::cout<<"READING "<<dirname<<std::endl;

    read_or_die("$ ls\n", in);
    dir rtr(dirname);
    char a;
    u_t subdir_cnt=0;
    while ((a=in.peek())!='$'&&a!='\n'&&!in.eof()) {
        if ('0'<=a&&a<='9') {//file
            ull_t s;
            std::string n;
            in>>s;
            read_or_die(" ",in);
            std::getline(in, n);
            rtr.add_file(n, s);
        } else {
            subdir_cnt ++;
            // gobble line
            while(in.get()!='\n');
        }
    }
    for (u_t i=0;i<subdir_cnt;i++) {
        if (verb_lvl>2)std::cout<<"READING "<<i<<" OUT OF "<<subdir_cnt<<std::endl;
        rtr.add_subdir(read_directory(in));
        if (in.peek()=='$')read_or_die("$ cd ..\n", in);
    }
    if(verb_lvl>1)std::cout<<"SIZE OF "<<dirname<<": "<<rtr.size<<std::endl;
    return rtr;
}

ull_t cnt_ans_1(dir d) {
    ull_t rtr=0;
    if (d.size <= 100000)
        rtr += d.size;
    for (auto a:d.subdirs) {
        rtr += cnt_ans_1(a);
    }
    return rtr;
}

ull_t find_min_greater_than(dir d, ull_t gt, ull_t min) {
    if (d.size < gt) return min;
    if (d.size < min) min=d.size;
    for (auto a:d.subdirs) {
        min = find_min_greater_than(a, gt, min);
    }
    return min;
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

    dir root = read_directory(input);
    std::cout<<cnt_ans_1(root)<<std::endl;
    // do things with input.

    ull_t min=-1,gt=root.size-40000000;
    if(verb_lvl>0)std::cout<<"FINDING dir OF SIZE AT LEAST "<<gt<<std::endl;
    std::cout<<find_min_greater_than(root, gt, min)<<std::endl;
    

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
