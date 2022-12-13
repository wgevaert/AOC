#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>
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

const ll_t perms[24][3][2] = {
    {{0,1},{1,1},{2,1}}, {{0,1},{1,-1},{2,-1}}, {{0,-1},{1,1},{2,-1}}, {{0,-1},{1,-1},{2,1}},
    {{2,1},{0,1},{1,1}}, {{2,1},{0,-1},{1,-1}}, {{2,-1},{0,1},{1,-1}}, {{2,-1},{0,-1},{1,1}},
    {{1,1},{2,1},{0,1}}, {{1,1},{2,-1},{0,-1}}, {{1,-1},{2,1},{0,-1}}, {{1,-1},{2,-1},{0,1}},
    {{1,1},{0,1},{2,-1}}, {{1,1},{0,-1},{2,1}}, {{1,-1},{0,1},{2,1}}, {{1,-1},{0,-1},{2,-1}},
    {{2,1},{1,1},{0,-1}}, {{2,1},{1,-1},{0,1}}, {{2,-1},{1,1},{0,1}}, {{2,-1},{1,-1},{0,-1}},
    {{0,1},{2,1},{1,-1}}, {{0,1},{2,-1},{1,1}}, {{0,-1},{2,1},{1,1}}, {{0,-1},{2,-1},{1,-1}},
};

class beacon {
public:
    ll_t c[3];
    beacon(){}
    beacon(ll_t X,ll_t Y,ll_t Z) {c[0]=X;c[1]=Y;c[2]=Z;}
    bool operator==(const beacon& r) const {return r.c[0]==c[0]&&r.c[1]==c[1]&&r.c[2]==c[2];}
    ll_t dist(const beacon& b) {
        ll_t r=0;
        for (u_t i=0;i<3;i++)if(b.c[i]<c[i])r+=c[i]-b.c[i];else r+=b.c[i]-c[i];
        return r;
    }
};

struct beaconHash {
 std::size_t operator()(const beacon& k) const
 {
     return std::hash<ll_t>()(k.c[0]) ^
            (
                (
                    std::hash<ll_t>()(k.c[1]) ^ (std::hash<ll_t>()(k.c[2]) << 1)
                ) << 1
            );
 }
};

const u_t max_scanners = 33;

class scanner {
public:
    bool may_overlap_with[max_scanners], matched0=false;
    const u_t n;
    beacon pos;
    std::vector<beacon> beacons;
    scanner(u_t N): n(N) {for (u_t i=0;i<max_scanners;i++)may_overlap_with[i]=true;}
    std::vector<beacon> get_rotated(u_t perm_nr) {
        std::vector<beacon> rtr = {};
        for (auto &a:beacons) {
            rtr.emplace_back(
                a.c[perms[perm_nr][0][0]]*perms[perm_nr][0][1], 
                a.c[perms[perm_nr][1][0]]*perms[perm_nr][1][1], 
                a.c[perms[perm_nr][2][0]]*perms[perm_nr][2][1]
            );
        }
        return rtr;
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

    std::vector<scanner> scanners = {};

    while (!input.eof()) {
        u_t n;
        read_or_die("--- scanner ",input);
        input>>n;
        if (n!=scanners.size()) {
            std::cerr<<"Expected scanner "<<scanners.size()<<" but got "<<n<<std::endl;
            exit(1);
        }
        scanners.emplace_back(n);
        read_or_die(" ---\n",input);
        while(input.peek()!='\n'&&!input.eof()) {
            ll_t x,y,z;
            input>>x;
            read_or_die(",",input);
            input>>y;
            read_or_die(",",input);
            input>>z;
            read_or_die("\n",input);
            if(verb_lvl>6)std::cout<<" found beacon at "<<x<<','<<y<<','<<z<<" for scanner "<<n<<std::endl;
            scanners.back().beacons.emplace_back(x,y,z);
        }
        input.get();
    }
    input.close();
    if (scanners.size()>max_scanners) {
        std::cerr<<"Did not expect "<<scanners.size()<<" scanners, only "<<max_scanners<<" expected"<<std::endl;
        return 1;
    }

    scanners[0].matched0=true;
    scanners[0].pos=beacon(0,0,0);

    std::unordered_map<beacon, u_t, beaconHash> translations;
    u_t total_matches = 1;
    bool tried = true;
    while (total_matches != scanners.size() && tried) {
        tried=false;
        if(verb_lvl>5)std::cout<<total_matches<<std::endl;
        for (auto& a:scanners) {
            if (!a.matched0) {
                if (verb_lvl>9)std::cout<<"Looking at "<<a.n<<std::endl;
                for (u_t b=0;b<scanners.size();b++) {
                    if (scanners[b].matched0 && a.may_overlap_with[b]) {
                        tried=true;
                        bool match = false;
                        for (u_t p=0;p<24;p++) {
                            translations.clear();
                            auto v=a.get_rotated(p);
                            for (auto x:v)for (auto y:scanners[b].beacons) {
                                const beacon key(y.c[0]-x.c[0],y.c[1]-x.c[1],y.c[2]-x.c[2]);
                                const auto loc=translations.find(key);
                                if (loc==translations.end()) {
                                    translations[key]=1;
                                } else {
                                    loc->second++;
                                }
                            }
                            beacon tr;
                            for (auto t:translations) {
                                if (verb_lvl>10&&t.second!=1)std::cout<<"Comparing "<<a.n<<" with "<<scanners[b].n<<" for perm "<<p
                                         <<" gives translation "<<t.first.c[0]<<','<<t.first.c[1]<<','<<t.first.c[2]
                                         <<" with count "<<t.second<<std::endl;
                                if (t.second >= 12) {
                                    tr=t.first;
                                    match=true;
                                    break;
                                }
                            }
                            if (match) {
                                a.matched0=true;
                                if(verb_lvl>4)std::cout<<a.n<<" found a match with "<<b<<" with permutation "<<p<<" and translation "<<tr.c[0]<<','<<tr.c[1]<<','<<tr.c[2]<<std::endl;
                                for (auto& vv:v)for (u_t i=0;i<3;i++)vv.c[i]+=tr.c[i];
                                a.beacons=v;
                                total_matches++;
                                a.pos = tr;
                                break;
                            }
                        }
                        if (!match) {
                            a.may_overlap_with[b]=false;
                        } else {
                            break;
                        }
                    } else if (verb_lvl>8) {
                        std::cout<<a.n<<" did not try to match "<<scanners[b].n<<" with "<<scanners[b].matched0<<','<<a.may_overlap_with[b]<<std::endl;
                    }
                }
            }
        }
    }
    if (total_matches != scanners.size()) {
        std::cerr<<(scanners.size()-total_matches)<<" scanners could not be matched!"<<std::endl;
        return 1;
    }

    translations.clear();
    for (auto a:scanners)for (auto b:a.beacons)if (translations.find(b)==translations.end())translations[b]=1;

    std::cout<<translations.size()<<std::endl;

    ll_t max_dist=0;
    for (auto& a:scanners)for(auto& b:scanners){auto d=a.pos.dist(b.pos);if (d>max_dist)max_dist=d;}

    std::cout<<max_dist<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
