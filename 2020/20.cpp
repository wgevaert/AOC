#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <array>
#include <bitset>
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

std::vector<std::string> do_turn(const std::vector<std::string>& Tile) {
    std::vector<std::string> new_tile(Tile.size(), std::string(Tile.size(),'?'));
    for (u_t i=0;i<Tile.size();i++) {
        for (u_t j=0;j<Tile[i].size();j++) {
            new_tile[j][Tile.size()-i-1] = Tile[i][j];
        }
    }
    return new_tile;
}

std::vector<std::string> do_flip(const std::vector<std::string>& Tile) {
    std::vector<std::string> new_tile(Tile.size(), std::string(Tile.size(), '?'));
    for (u_t i=0;i<Tile.size();i++) {
        for (u_t j=0;j<Tile[i].size();j++) {
            new_tile[Tile.size()-i-1][j] = Tile[i][j];
        }
    }
    return new_tile;
}

struct tile {
    uint64_t id;
    std::vector<std::string> Tile;
    bool used = false;
    std::array<uint32_t,4> hash={9696, 9696, 9696, 9696};

    tile(uint64_t ID): id(ID){};
    tile(){};

    void turn() {
        Tile = do_turn(Tile);
    }
    void flip() {
        Tile = do_flip(Tile);
    }

    void calc_hashes() {
        hash[0] = 0;
        for (auto c:Tile[0]) {
            hash[0]<<=1;
            if (c == '#') {
                hash[0]|=1;
            }
        }
        hash[1] = 0;
        for (auto t:Tile) {
            hash[1]<<=1;
            if (t.back()=='#') {
                hash[1]|=1;
            }
        }
        hash[2] = 0;
        for (auto c:Tile.back()) {
            hash[2]<<=1;
            if (c == '#') {
                hash[2]|=1;
            }
        }
        hash[3] = 0;
        for (auto t:Tile) {
            hash[3]<<=1;
            if (t[0]=='#') {
                hash[3]|=1;
            }
        }
    }
};

const std::array<std::pair<uint32_t,uint32_t>,15> monster = {{
                                                                                                                {0,18},
    {1,0},                      {1,5},{1,6},                       {1,11},{1,12},                        {1,17},{1,18},{1,19},
          {2,1},          {2,4},            {2,7},          {2,10},              {2,13},          {2,16}
}};

uint32_t reverse_hash(uint32_t hash) {
    uint32_t ans=0;
    for (int i=0;i<10;i++) {
        ans <<= 1;
        if (hash % 2)
            ans |= 1;
        hash >>= 1;
    }
    return ans;
}

struct solution {
    uint32_t size;
    std::vector<tile> tiles;
    solution(uint32_t s, tile corner):size(s),tiles(1,corner){};
    bool attempt(tile t) {
        t.calc_hashes();
        if (tiles.size() >= size) {
            // test if t fits at top
            if (t.hash[0] != tiles[tiles.size()-size].hash[2]) {
                return false;
            }
        }
        if (tiles.size()%size) {
            if (t.hash[3] != tiles.back().hash[1])
                return false;
        }
        tiles.push_back(t);
        return true;
    }
    std::vector<std::string> get_solution() {
        std::vector<std::string> ans(8*size,std::string(8*size,'?'));
        for (u_t i=0;i<8*size;i++) {
            for (u_t j=0;j<8*size;j++) {
                auto row = i/8;
                auto col = j/8;
                if (row*size + col < tiles.size()) {
                    ans[i][j] = tiles[row*size+col].Tile[i%8+1][j%8+1];
                }
            }
        }
        return ans;
    }
    void print() {
        auto a = get_solution();
        for (auto b:a)std::cout<<b<<std::endl;
    }
    void do_print() {
        for (u_t i=0;i<10*size;i++) {
            if (!(i%10))
                std::cout<<std::endl;
            for (u_t j=0;j<10*size;j++) {
                auto row = i/10;
                auto col = j/10;
                if (!(j%10))std::cout<<' ';
                if (row*size + col < tiles.size()) {
                    std::cout<<tiles[row*size+col].Tile[i%10][j%10];
                }
            }
            std::cout<<std::endl;
        }
        auto a = get_solution();
        for (auto b:a)std::cout<<b<<std::endl;
    }
};

solution solve(
    tile corner,
    std::unordered_map <uint32_t, tile> tiles,
    std::array<std::vector<uint32_t>, (1<<10)> hashes
) {
    uint32_t size;
    if (tiles.size() == 144) {
        size = 12;
    } else if (tiles.size() == 9) {
        size = 3;
    }
    bool used0, used3;
    do {
        used0=false;
        used3=false;
        for (auto a : hashes[corner.hash[0]]) {
            if (a != corner.id) {
                used0 = true;
            }
        }
        for (auto a:hashes[corner.hash[3]]) {
            if (a != corner.id) {
                used3 = true;
            }
        }
        if (used0 || used3) {
            corner.turn();
            corner.calc_hashes();
        }
    } while(used0 || used3);

    if(verb_lvl > 0) {
        std::cout<<"Done turning the corner :D"<<std::endl;
    }

    solution sol(size, corner);

    while (sol.tiles.size() != tiles.size()) {
        tile my_tile;
        for (auto& a: tiles) {
            bool fits = true;
            // This code is just beautiful, I am so proud
            if (!a.second.used) {
                my_tile = a.second;
                if (!sol.attempt(my_tile)) {
                    my_tile.turn();
                    if (!sol.attempt(my_tile)) {
                        my_tile.turn();
                        if (!sol.attempt(my_tile)) {
                            my_tile.turn();
                            if (!sol.attempt(my_tile)) {
                                my_tile.flip();
                                if (!sol.attempt(my_tile)) {
                                    my_tile.turn();
                                    if (!sol.attempt(my_tile)) {
                                        my_tile.turn();
                                        if (!sol.attempt(my_tile)) {
                                            my_tile.turn();
                                            if (!sol.attempt(my_tile)) {
                                                fits = false;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (fits) {
                    a.second.used = true;
                    if (verb_lvl > 4) {
                        std::cout<<"Hurray it fits!"<<std::endl;
                        sol.do_print();
                    }
                    break;
                } else if (verb_lvl > 4) {
                    std::cout<<"No fit found"<<std::endl;
                }
            }
        }
    }

    if (sol.tiles.size() != tiles.size()) {
        std::cerr<<"I did not implement backtracking :("<<std::endl;
        exit(1);
    }

    return sol;
}

auto find_monsters(std::vector<std::string>& field) {
    uint32_t monster_cnt = 0;
    for (u_t i=0;i<field.size()-3;i++) {
        for (u_t j=0;j<field.size()-20;j++) {
            bool is_monster = true;
            for(auto m:monster) {
                if (field[i+m.first][j+m.second] != '#')
                    is_monster = false;
            }
            if (is_monster) {
                for (auto m:monster) {
                    field[i+m.first][j+m.second] = 'O';
                }
                monster_cnt++;
                if (verb_lvl > 8) {
                    std::cout<<"\n\nFound a monster!"<<std::endl;
                    for (auto a: field)std::cout<<a<<std::endl;
                }
            }
        }
    }
    return monster_cnt;
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

    std::unordered_map<uint32_t, tile> tiles;
    uint64_t id;
    std::string line;

    while (!input.eof()) {
        if (input.peek()==-1)
            break;
        read_or_die("Tile ",input);
        input>>id;
        tiles.emplace(id,id);
        read_or_die(":\n",input);
        std::getline(input, line);

        while (line.size()) {
            tiles[id].Tile.emplace_back(line);
            std::getline(input, line);
        }
    }

    input.close();

    if (tiles.size()>144) {
        std::cout<<"Expected size of 144"<<std::endl;
        exit(1);
    }

    if (verb_lvl > 10) {
        for (auto& a:tiles) {
            std::cout << a.second.id <<':' << std::endl;
            for (uint64_t i=0;i<a.second.Tile.size();i++) {
                std::cout<<a.second.Tile[i]<<'('<<i<<')'<<std::endl;
            }
        }
    }

    std::array<std::vector<uint32_t>, (1<<10)> hashes;

    for (auto& a : tiles) {
        a.second.calc_hashes();
        for (auto h: a.second.hash) {
            hashes[h].emplace_back(a.second.id);
            hashes[reverse_hash(h)].emplace_back(a.second.id);
        }
    }

    tile corner;

    for (auto& a:tiles) {// find a corner
        bool used[4]={false, false, false, false};
        for (int k=0;k<4;k++) {
            for (auto i : hashes[a.second.hash[k]]) {
                if (i != a.second.id) {
                    used[k] = true;
                    break;
                }
            }
        }
        if (
            (!used[0] && !used[1]) ||
            (!used[1] && !used[2]) ||
            (!used[2] && !used[3]) ||
            (!used[3] && !used[0])
        ) {
            if (verb_lvl > 3) {
                std::cout<<"found corner: "<<a.second.id<<std::endl;
            }
            if (!corner.used) {
                a.second.used = true;
                corner = a.second;
                break;
            }
        }
    }

    solution s = solve(corner, tiles, hashes);

    std::cout<<s.tiles[0].id * s.tiles[s.size-1].id * s.tiles.back().id * s.tiles[s.size*(s.size-1)].id<<std::endl;

    std::vector<std::string> field = s.get_solution();

    // same same
    if (!find_monsters(field)) {
        field = do_turn(field);
        if (!find_monsters(field)) {
            field = do_turn(field);
            if (!find_monsters(field)) {
                field = do_turn(field);
                if (!find_monsters(field)) {
                    field = do_flip(field);
                    if (!find_monsters(field)) {
                        field = do_turn(field);
                        if (!find_monsters(field)) {
                            field = do_turn(field);
                            if (!find_monsters(field)) {
                                field = do_turn(field);
                                if (!find_monsters(field)) {
                                    std::cerr<<"No monsters could be found :("<<std::endl;
                                    exit(1);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    if (verb_lvl > 0) {
        std::ofstream image1("20_monsters.pgm");

        if(!image1.good()){std::cout<<"20."<<id<<".pbm is bad"<<std::endl;exit(1);}
        image1 <<"P2 "<<96<<' '<<96<<'\n'<<2<<std::endl;

        for (auto a:field)
            for (auto b:a)
                switch(b) {
                    case '#':
                        image1<<0<<' ';
                        break;
                    case '.':
                        image1<<1<<' ';
                        break;
                    case 'O':
                        image1<<2<<' ';
                        break;
                }

        image1.close();
    }

    uint32_t ans2=0;

    for (auto a: field)
        for (auto b:a)
            if (b=='#')
                ans2++;
    std::cout << ans2 << std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
