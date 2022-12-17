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

/*********************
0 = ####

    .#.
1 = ###
    .#.

    ..#
2 = ..#
    ###

    #
3 = #
    #
    #

4 = ##
    ##
*********************/

const u_t h[5] = {1,3,3,4,2};

class row {
public:
    char r[7];
    row() {for(u_t i=0;i<7;i++)r[i]='.';}
    char operator[] (const int i) const {
        if (i<0||i>=7)return '|';return r[i];
    }
};

const bool can_move_down(const u_t piece, const ull_t y, const short x, const std::vector<row>& field) {
    if (y==0) {
        return false;
    }
    switch (piece) {
        case 0:return field[y-1][x]=='.'&&field[y-1][x+1]=='.'&&field[y-1][x+2]=='.'&&field[y-1][x+3]=='.';
        case 1:return field[y][x]=='.'&&field[y-1][x+1]=='.'&&field[y][x+2]=='.';
        case 2:return field[y-1][x]=='.'&&field[y-1][x+1]=='.'&&field[y-1][x+2]=='.';
        case 3:return field[y-1][x]=='.';
        case 4:return field[y-1][x]=='.'&&field[y-1][x+1]=='.';
    }
    return false;
}

void put_piece(std::vector<row>& field, u_t piece, u_t x, ull_t y, char c) {
    switch (piece) {
        case 0:
            field[y].r[x]=c;
            field[y].r[x+1]=c;
            field[y].r[x+2]=c;
            field[y].r[x+3]=c;
            break;

        case 1:
            field[y].r[x+1]=c;
            field[y+1].r[x]=c;
            field[y+1].r[x+1]=c;
            field[y+1].r[x+2]=c;
            field[y+2].r[x+1]=c;
            break;

        case 2:
            field[y].r[x]=c;
            field[y].r[x+1]=c;
            field[y].r[x+2]=c;
            field[y+1].r[x+2]=c;
            field[y+2].r[x+2]=c;
            break;

        case 3:
            field[y].r[x]=c;
            field[y+1].r[x]=c;
            field[y+2].r[x]=c;
            field[y+3].r[x]=c;
            break;

        case 4:
            field[y].r[x]=c;
            field[y].r[x+1]=c;
            field[y+1].r[x]=c;
            field[y+1].r[x+1]=c;
            break;
            
    }
}

const bool can_move_left(const u_t piece, const ull_t y, const short x, const std::vector<row>& field) {
    if (field.size()<y+h[piece]) {
        std::cerr<<"WHOOPS!l"<<std::endl;exit(1);
    }
    switch (piece) {
        case 0:return field[y][x-1]=='.';
        case 1:return field[y][x]=='.'&&field[y+1][x-1]=='.'&&field[y+2][x]=='.';
        case 2:return field[y][x-1]=='.'&&field[y+1][x+1]=='.'&&field[y+2][x+1]=='.';
        case 3:return field[y][x-1]=='.'&&field[y+1][x-1]=='.'&&field[y+2][x-1]=='.'&&field[y+3][x-1]=='.';
        case 4:return field[y][x-1]=='.'&&field[y+1][x-1]=='.';
    }
    return false;
}

const bool can_move_right(const u_t piece, const ull_t y, const short x, const std::vector<row>& field) {
    if (field.size()<y+h[piece]) {
        std::cerr<<"WHOOPS!r"<<std::endl;exit(1);
    }
    switch (piece) {
        case 0:return field[y][x+4]=='.';
        case 1:return field[y][x+2]=='.'&&field[y+1][x+3]=='.'&&field[y+2][x+2]=='.';
        case 2:return field[y][x+3]=='.'&&field[y+1][x+3]=='.'&&field[y+2][x+3]=='.';
        case 3:return field[y][x+1]=='.'&&field[y+1][x+1]=='.'&&field[y+2][x+1]=='.'&&field[y+3][x+1]=='.';
        case 4:return field[y][x+2]=='.'&&field[y+1][x+2]=='.';
    }
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

    std::vector<bool> to_left;
    char a=input.get();
    while(a!='\n') {
        to_left.emplace_back(a=='<');
        a=input.get();
    }
    input.close();

    ull_t period = 1;// 756-413;// To solve puzzle: Set period = 1, look at output, manually find period, set period to that value, then run program again.

    ull_t cur_jet = 0, y_max=0, prev=0, mod = to_left.size()*period, prev_diff=-1, tries=0;
    if (to_left.size()%5) mod*=5;

    if (to_left.size()<100){mod=to_left.size()*7;} // The test case

    std::vector<row>field = {};

    // 1 000 000 000 000 that's a lot
    bool legend=true;
    for (ull_t i=0;i<1000000000000;i++) { //Basically an infinite loop

        if (period!=1&&i==2022)std::cout<<"Answer 1: "<<y_max<<std::endl;

        if ((i%mod)==(1000000000000%mod)){
            if (period!=1&&prev_diff==y_max-prev) {
                std::cout<<"Answer 2: "<<(y_max+prev_diff*((1000000000000-i)/mod))<<std::endl;
                legend = false;
                break;
            }
            if (period==1) {
                std::cout<<y_max-prev<<std::endl;
            } else if (tries > 3) {
                std::cerr<<"It seems the period you provided is not an actual period. Please reset period to 1 and retry finding period"<<std::endl;exit(1);
            }
            prev_diff=y_max-prev;
            prev=y_max;
            tries++;
        }

        if (verb_lvl&&i<40) {// output
            std::cout<<"\n\n";
            for (auto a=field.rbegin();a!=field.rend();a++){for(auto b:a->r)std::cout<<b;std::cout<<std::endl;}
            std::cout<<std::endl;
        }

        ull_t y=y_max+3;
        u_t piece = i%5, x=2;
        if (field.size()<=y+h[piece])field.resize(y+h[piece]+1);

        while(true) {

            if (verb_lvl&&i<40) { // output
                std::cout<<"\n\nNow:\n";
                put_piece(field, piece, x,y,'@');
                for (auto a=field.rbegin();a!=field.rend();a++){for(auto b:a->r)std::cout<<b;std::cout<<std::endl;}
                put_piece(field, piece, x,y,'.');
                std::cout<<std::endl;
            }

            if (to_left[cur_jet%to_left.size()]) {
                if (can_move_left(piece, y, x, field)) {
                    x--;
                }
            } else {
                if (can_move_right(piece, y, x, field)) {
                    x++;
                }
            }

            if (verb_lvl&&i<40) {
                std::cout<<"\n\nAfter move:\n";
                put_piece(field, piece, x,y,'@');
                for (auto a=field.rbegin();a!=field.rend();a++){for(auto b:a->r)std::cout<<b;std::cout<<std::endl;}
                put_piece(field, piece, x,y,'.');
                std::cout<<std::endl;
            }

            cur_jet++;
            if (can_move_down(piece, y, x, field)) {
                y--;
            } else {
                if (y_max < y+h[piece])y_max=y+h[piece];
                put_piece(field, piece, x, y, '#');
                break;
            }
        }
    }

    if (legend)std::cout<<"You are a legend, and you deserve the answer to part 2, which is "<<y_max<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
