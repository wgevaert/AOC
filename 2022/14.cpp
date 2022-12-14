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

const u_t x_min=300,x_max=700,y_min=0,y_max=200;

const bool part2=false;
const bool make_images=true;

u_t char_to_val(char a) {
    switch (a) {
        case '.':return 0;
        case '#': return 4;
        case '+': return 3;
        case '~': return 2;
        default: return 1;
    }
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

    char field[y_max-y_min+1][x_max-x_min+1];
    for (u_t i=y_min;i<=y_max;i++)for(u_t j=x_min;j<=x_max;j++)field[i-y_min][j-x_min]='.';

    u_t x,y,nx,ny,maxy;
    while(true) {
        input>>x;
        if (input.eof())break;
        read_or_die(",",input);
        input>>y;
        if (y>maxy)maxy=y;
        read_or_die(" -> ",input);
        while(true) {
            input>>nx;
            read_or_die(",",input);
            input>>ny;
            if (ny>maxy)maxy=ny;
            int dy = (ny<y?-1:1), dx=(nx<x?-1:1);
            if(verb_lvl>3)std::cout<<"drawing "<<x<<','<<y<<" -> "<<nx<<','<<ny<<std::endl;
            for (int Y=y;Y-dy!=ny;Y+=dy) {
                for(int X=x;X-dx!=nx;X+=dx)field[Y-y_min][X-x_min]='#';
            }
            x=nx;y=ny;
            if (input.get()!=' ')break;
            read_or_die("-> ",input);
        }
    }
    input.close();

    field[0-y_min][500-x_min]='+';
    std::vector<std::pair<u_t,u_t>> falling_sand = {{500,0}};
    u_t stationary_sand=0;
    u_t image_step = (part2 ? 100 : 1);

    if (part2)for (u_t i=x_min;i<=x_max;i++)field[maxy+2][i-x_min]='#';
    if (make_images) {
        std::string filename = std::to_string(stationary_sand);
        filename = "14-"+std::string(5-filename.size(),'0') + filename+".pgm";
        std::ofstream image(filename);
        if (!image.good()) {
            std::cerr<<"Cannot open image "<<filename<<std::endl;exit(1);
        }
        image<<"P2 "<<(x_max-x_min+1)<<' '<<(y_max-y_min+1)<<" 4 ";
        for (u_t j=y_min;j<=y_max;j++)for(u_t i=x_min;i<=x_max;i++)image<<char_to_val(field[j-y_min][i-x_min])<<' ';
    }

    while (falling_sand.size()&&falling_sand.back().second<y_max) {
        if (falling_sand.back().first <=x_min || falling_sand.back().first >= x_max) {
            std::cerr<<x_min<<" and "<<x_max<<" are not sufficient for "<<falling_sand.back().first<<std::endl;exit(1);
        }
        if (field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min] == '.') {
            field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min] = '~';
            falling_sand.emplace_back(falling_sand.back().first, falling_sand.back().second+1);
        } else if (field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min-1] == '.') {
            field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min-1] = '~';
            falling_sand.emplace_back(falling_sand.back().first-1, falling_sand.back().second+1);
        } else if (field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min+1] == '.') {
            field[falling_sand.back().second-y_min+1][falling_sand.back().first-x_min+1] = '~';
            falling_sand.emplace_back(falling_sand.back().first+1, falling_sand.back().second+1);
        } else {
            field[falling_sand.back().second-y_min][falling_sand.back().first-x_min]='o';
            stationary_sand++;
            falling_sand.pop_back();
            if (make_images && !(stationary_sand%image_step)) {
                std::string filename = std::to_string(stationary_sand);
                filename = "14-"+std::string(5-filename.size(),'0') + filename+".pgm";
                std::ofstream image(filename);
                if (!image.good()) {
                    std::cerr<<"Cannot open image "<<filename<<std::endl;exit(1);
                }
                image<<"P2 "<<(x_max-x_min+1)<<' '<<(y_max-y_min+1)<<" 4 ";
                for (u_t j=y_min;j<=y_max;j++)for(u_t i=x_min;i<=x_max;i++)image<<char_to_val(field[j-y_min][i-x_min])<<' ';
            }
        }
    }

    if (make_images) {
        std::ofstream image("14.pgm");
        if (!image.good()) {
            std::cerr<<"Cannot open image 14.pgm"<<std::endl;exit(1);
        }
        image<<"P2 "<<(x_max-x_min+1)<<' '<<(y_max-y_min+1)<<" 4 ";
        for (u_t j=y_min;j<=y_max;j++)for(u_t i=x_min;i<=x_max;i++)image<<char_to_val(field[j-y_min][i-x_min])<<' ';
    }


    std::cout<<stationary_sand<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
