#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
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

#define east 0
#define southeast 1
#define southwest 2
#define west 3
#define northwest 4
#define northeast 5

struct pos_t {
    int a,b;
    pos_t(){};
    pos_t(int A, int B):a(A),b(B){};
};

const std::array<pos_t,6> dirs {{
    {0,1},{-1,1},{-1,0},{0,-1},{1,-1},{1,0}
}};

pos_t& operator+=(pos_t& a, pos_t b){a.a+=b.a;a.b+=b.b;return a;}

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

    const int32_t mid = 1024;
    static bool field[2][2*mid+1][2*mid+1];

    pos_t ul={mid,mid},br={mid,mid}; // upper left and bottom right

    while (input.peek() != -1) {
        pos_t pos = {0,0};
        while (input.peek()!='\n') {
            switch(input.get()) {
                case 's':
                    switch (input.get()) {
                        case 'e':
                            pos += dirs[southeast];
                            break;
                        case 'w':
                            pos += dirs[southwest];
                            break;
                        default:
                            std::cerr<<"ERROR"<<std::endl;
                            exit(1);
                    }
                    break;
                case 'n':
                    switch (input.get()) {
                        case 'e':
                            pos += dirs[northeast];
                            break;
                        case 'w':
                            pos += dirs[northwest];
                            break;
                        default:
                            std::cerr<<"ERROR"<<std::endl;
                            exit(1);
                    }
                    break;
                case 'w':
                    pos += dirs[west];
                    break;
                case 'e':
                    pos += dirs[east];
                    break;
                default:
                    std::cout<<"ERROR"<<std::endl;
                    exit(1);
            }
        }
        if (pos.a>mid||pos.a<-1*mid||pos.b>mid||pos.b<-1*mid) {
            std::cerr<<pos.a<<','<<pos.b<<" doesn't fit"<<std::endl;
            exit(1);
        }
        if (pos.a+mid < ul.a)ul.a=pos.a+mid;
        if (pos.b+mid < ul.b)ul.b=pos.b+mid;
        if (pos.a+mid > br.a)br.a=pos.a+mid;
        if (pos.b+mid > br.b)br.b=pos.b+mid;
        field[0][pos.a+mid][pos.b+mid] = !field[0][pos.a+mid][pos.b+mid];
        read_or_die("\n",input);
    }

    input.close();

    uint32_t ans1=0;
    for(int a=ul.a;a<=br.a;a++)
        for (int b=ul.b;b<=br.b;b++)
            if (field[0][a][b])
                ans1++;

    std::cout<<ans1<<std::endl;

    for (int i=0;i<100;i++) {
        for (int a=ul.a-1;a<=br.a+1;a++)
            for (int b=ul.b-1;b<=br.b+1;b++) {
                uint32_t n = 0;
                for (auto d:dirs) {
                    if (field[i%2][a+d.a][b+d.b])
                        n++;
                }
                if ((field[i%2][a][b] && (n==1||n==2)) || (!field[i%2][a][b] && n==2)) {
                    if (a<ul.a)ul.a=a;
                    if (b<ul.b)ul.b=b;
                    if (a>br.a)br.a=a;
                    if (b>br.b)br.b=b;
                    field[1-(i%2)][a][b] = true;
                } else {
                    field[1-(i%2)][a][b] = false;
                }
            }
        if (verb_lvl > 5) {
            std::cout<<i<<':'<<std::endl;
            for (int a=ul.a;a<=br.a;a++) {
                for (int b=ul.b;b<=br.b;b++)
                    std::cout<<(field[1-(i%2)][a][b] ? '#' : '.');
                std::cout<<std::endl;
            }
        }
    }

    uint32_t ans2=0;
    for(int a=0;a<=2*mid;a++)
        for (int b=0;b<=2*mid;b++)
            if (field[0][a][b])
                ans2++;

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
