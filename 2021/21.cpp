#include <iostream>
#include <fstream>
#include <chrono>
#include <string>/*
#include <vector>
#include <unordered_map>*/

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

const u_t unis[10] = {0,0,0,1,3,6,7,6,3,1}; // unis[x] gives number of ways to obtain x with three dice rolls.

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

constexpr u_t hash(const u_t p1, const u_t p2, const u_t s1, const u_t s2) {
    return p1+10*p2+10*10*s1+10*10*24*s2;
}

const ull_t answers2[10][10] = 
{{32491093007709,27674034218179,48868319769358,97774467368562,138508043837521,157253621231420,141740702114011,115864149937553,85048040806299,57328067654557},
{27464148626406,24411161361207,45771240990345,93049942628388,131888061854776,149195946847792,133029050096658,106768284484217,76262326668116,49975322685009},
{51863007694527,45198749672670,93013662727308,193753136998081,275067741811212,309991007938181,273042027784929,214368059463212,147573255754448,92399285032143},
{110271560863819,91559198282731,193170338541590,404904579900696,575111835924670,647608359455719,568867175661958,444356092776315,303121579983974,187451244607486},
{156667189442502,129742452789556,274195599086465,575025114466224,816800855030343,919758187195363,807873766901514,630947104784464,430229563871565,265845890886828},
{175731756652760,146854918035875,309196008717909,647920021341197,920342039518611,1036584236547450,911090395997650,712381680443927,486638407378784,301304993766094},
{152587196649184,131180774190079,272847859601291,570239341223618,809953813657517,912857726749764,803934725594806,630797200227453,433315766324816,270005289024391},
{116741133558209,105619718613031,214924284932572,446968027750017,634769613696613,716241959649754,632979211251440,499714329362294,346642902541848,218433063958910},
{83778196139157,75823864479001,148747830493442,306621346123766,435288918824107,492043106122795,437256456198320,348577682881276,245605000281051,157595953724471},
{56852759190649,49982165861983,93726416205179,190897246590017,270803396243039,306719685234774,274291038026362,221109915584112,158631174219251,104001566545663}
};

std::pair<ull_t,ull_t> get_wins(u_t p1, u_t p2, u_t s1, u_t s2) {
    static std::pair<ull_t,ull_t> sols[hash(9,9,23,23)+1];
    u_t h = hash(p1,p2,s1,s2);
    if (sols[h] != std::pair<ull_t,ull_t>(0,0)) {
        if (verb_lvl>2)std::cout<<"Returning cached result "<<sols[h].first<<','<<sols[h].second<<" for "<<p1<<','<<p2<<':'<<s1<<','<<s2<<std::endl;
        return sols[h];
    }
    ull_t w1=0,w2=0;
    for (u_t i=3;i<=9;i++){
        u_t p1n = (p1+i)%10, s1n = s1+p1n+1;
        if (s1n>=21) {
            if (verb_lvl)std::cout<<"1 wins for "<<p1<<','<<p2<<':'<<s1<<','<<s2<<" with score "<<s1n<<" when throwing "<<i<<" in "<<unis[i]<<" different ways."<<std::endl;
            w1 += unis[i];
        } else for (u_t j=3;j<=9;j++) {
            u_t p2n = (p2+j)%10, s2n = s2+p2n+1;
            if (s2n>= 21) {
                if(verb_lvl) std::cout<<"2 wins for "<<p1<<','<<p2<<':'<<s1<<','<<s2<<" with score "<<s2n<<" rivaling "<<s1n<<" when throwing "<<j<<" in "<<unis[j]<<" different ways, when 1 threw "<<i<<" in "<<unis[i]<<" ways"<<std::endl;
                w2 +=unis[i]*unis[j];
            } else {
                if (verb_lvl>1)std::cout<<"Recursing in "<<p1<<','<<p2<<':'<<s1<<','<<s2<<" into "<<p1n<<','<<p2n<<':'<<s1n<<','<<s2n<<" when throwing "<<i<<','<<j<<" (product "<<(unis[i]*unis[j])<<")\n";
                auto a=get_wins(p1n,p2n,s1n,s2n);
                w1 += a.first*unis[i]*unis[j];
                w2 += a.second*unis[i]*unis[j];
            }
        }
    }
    if (verb_lvl)std::cout<<"Could win for "<<p1<<','<<p2<<':'<<s1<<','<<s2<<" in "<<w1<<','<<w2<<" ways"<<std::endl;
    sols[h]={w1,w2};
    return {w1,w2};
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

    u_t p1,p2,P1,P2;
    read_or_die("Player 1 starting position: ",input);
    input>>p1;
    read_or_die("\nPlayer 2 starting position: ",input);
    input>>p2;
    input.close();

    // Part 1:
    p1--;p2--;// shift all by 1: 0<-1,...,9<-10
    P1=p1;P2=p2;
    u_t tot_rolls=0,cur_roll=1, s1=0,s2=0;
    while (s1<1000&s2<1000) {
        u_t roll=(cur_roll*3+1+2)%10;
        cur_roll=(3+cur_roll)%10;
        tot_rolls+=3;
        p1 = (p1+roll)%10;
        s1 += p1+1;
        if (s1>=1000) {
            std::cout<<(s2*tot_rolls)<<std::endl;
            break;
        }

        roll = (cur_roll*3+1+2)%10;
        cur_roll = (cur_roll+3)%10;
        tot_rolls+=3;
        p2 = (p2+roll)%10;
        s2 += p2+1;
        if (s2>=1000) {
            std::cout<<(s1*tot_rolls)<<std::endl;
        }
    }
    // End of part 1

    /*
    std::cout<<"const ull_t answers2[10][10] = {";
    for (u_t i=0;i<10;i++){
        std::cout<<'{';
        for(u_t j=0;j<10;j++){
            auto ans2 = get_wins(i,j,0,0);std::cout<<(ans2.first>ans2.second?ans2.first:ans2.second)<<(j==9?"":",");
        }std::cout<<'}'<<(i==9?"":",")<<std::endl;
    }std::cout<<"};\n";
    */

    // Cheesing part 2;
    std::cout<<answers2[P1][P2]<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
