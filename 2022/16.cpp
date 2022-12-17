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

// Kind-of constant, set during input handling/preprocessing.
const u_t s_max = 16;
u_t s;
u_t dist[s_max][s_max];
u_t flows[s_max];

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

class valve {
public:
    u_t flow;
    std::string name;
    std::vector<u_t> nbs = {};
    std::vector<u_t> dists = {};
};

// Gives index from string, and makes sure index exists on valves.
u_t name_to_id(std::string n, std::vector<valve>& valves) {
    static std::unordered_map<std::string, u_t> bloep = {};
    static u_t cur=0;

    auto pos = bloep.find(n);
    if (pos != bloep.end()) return pos->second;
    std::cout<<n<<" maps to "<<cur<<std::endl;
    bloep[n] = cur++;
    if (valves.size()<cur)
        valves.resize(cur);
    return bloep[n];
}

std::string read_name(std::istream& in) {
    char a=in.get(), b=in.get();
    return std::string(1,a) + std::string(1,b);
}

ull_t calc_ans_1(u_t time, u_t pos, ull_t released, std::vector<u_t>& nexts) {
    ull_t result=0;
    bool chose = false;
    for (auto a=0;a<nexts.size();a++) {
        if (time > dist[pos][nexts[a]]) {
            u_t w=nexts[a];
            nexts.erase(nexts.begin()+a);
            u_t new_time = time - dist[pos][w] - 1; // time to walk + open
            ull_t r = calc_ans_1(new_time, w, released + flows[w]*new_time, nexts);
            nexts.insert(nexts.begin()+a, w);
            if (r>result)result=r;
            chose = true;
        }
    }
    if (!chose) {
        return released;
    }
    return result;
}
ull_t calc_ans_2(u_t time, u_t t_ele, u_t pos, u_t p_ele, ull_t released, std::vector<u_t>& nexts) {
    static ull_t cnt=0, max=0;
    if (!(++cnt%1000000))std::cout<<cnt<<std::endl;
    ull_t result=0;
    bool chose = false;

    // bail early if hopeless.
    u_t t_max = time<t_ele?t_ele:time;
    u_t max_result = released;
    for (auto a:nexts) {
        u_t new_time = time - 1 - (dist[pos][a]<dist[p_ele][a]?dist[pos][a]:dist[p_ele][a]);
        max_result += flows[a]*new_time;
    }
    if (max_result < max){
        return 0;
    }

    for (auto a=0;a<nexts.size();a++) {
        u_t w=nexts[a];
        nexts.erase(nexts.begin()+a);
        if (time > dist[pos][w]) {
            u_t new_time = time - dist[pos][w] - 1; // time to walk + open
            ull_t r = calc_ans_2(new_time, t_ele, w, p_ele, released + flows[w]*new_time, nexts);
            if (r>result)result=r;
            chose = true;
        }
        if (t_ele > dist[p_ele][w]) {
            u_t new_time = t_ele - dist[p_ele][w] - 1; // time to walk + open
            ull_t r = calc_ans_2(time, new_time, pos, w, released + flows[w]*new_time, nexts);
            if (r>result)result=r;
            chose = true;
        }
        nexts.insert(nexts.begin()+a, w);
    }
    if (!chose) {
        return released;
    }
    if (result>max) {std::cout<<"Found "<<result<<std::endl;max=result;}
    return result;
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

    u_t aa=-1, max=-1;
    std::vector<valve> valves;
    while(input.peek() == 'V') {
        read_or_die("Valve ", input);
        std::string name = read_name(input);
        u_t i=name_to_id(name, valves);
        if (name == "AA")aa=i;
        std::getline(input, valves.back().name, ' ');
        read_or_die("has flow rate=",input);
        input>>valves[i].flow;
        read_or_die("; tunnel", input);
        if (input.get()=='s')input.get(); //gobble "s " or " "
        read_or_die("lead", input);
        if (input.get()=='s')input.get(); //gobble "s " or " "
        read_or_die("to valve", input);
        if (input.get()=='s')input.get(); //gobble "s " or " "
        do {
            std::string name = read_name(input);
            u_t j = name_to_id(name, valves);
            valves[i].nbs.emplace_back(j);

            if (input.get() == '\n')break;
            input.get();
        } while (true);
    }
    input.close();

    // calculate distances
    std::vector<u_t> prevs[2] = {{},{}};
    for (u_t i=0;i<valves.size();i++) {
        valves[i].dists.resize(valves.size(), max);
        valves[i].dists[i]=0;
        u_t dists_set=1, cur_dist=1, cur=0;
        for (auto a:valves[i].nbs){
            valves[i].dists[a]=cur_dist;++dists_set;
        }
        prevs[1].clear();
        prevs[0]=valves[i].nbs;
        while(dists_set<valves.size()&&cur_dist<valves.size()) {
            cur_dist++;
            for (auto a:prevs[cur]){
                for (auto b:valves[a].nbs)if (valves[i].dists[b]>cur_dist){
                    dists_set++;
                    prevs[(cur+1)%2].emplace_back(b);
                    valves[i].dists[b] = cur_dist;
                }
            }
            prevs[cur].clear();
            cur = (cur+1)%2;
        }
    }

    std::vector<u_t> new_ids = {};
    for (u_t i=0;i<valves.size();i++)if(i==aa){aa=new_ids.size();new_ids.emplace_back(i);} else if(valves[i].flow!=0)new_ids.emplace_back(i);
    s=new_ids.size();

    if (s>s_max){std::cerr<<s_max<<" is too small for "<<s<<std::endl;exit(1);}
    for (u_t a=0;a<new_ids.size();a++)std::cout<<new_ids[a]<<" has been changed to "<<a<<std::endl;

    for (u_t a=0;a<s;a++){
        flows[a]=valves[new_ids[a]].flow;
        for (u_t b=a;b<s;b++) {
            dist[a][b]=dist[b][a]=valves[new_ids[a]].dists[new_ids[b]];
        }
    }

    // Output all the info that we need to solve problem. All other info is useless.
    for (u_t a=0;a<new_ids.size();a++) {
        for (u_t b=0;b<s;b++) std::cout<< std::setfill('0') << std::setw(2)<<dist[a][b]<<' ';
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    for (u_t a=0;a<s;a++)std::cout<<flows[a]<<' ';
    std::cout<<std::endl;

    std::vector<u_t> nexts = {};
    for (u_t i=0;i<s;i++)if(i!=aa)nexts.emplace_back(i);

    u_t ans1=calc_ans_1(30,aa,0,nexts), ans2 = calc_ans_2(26,26,aa,aa,0,nexts);

    std::cout<<ans1<<' '<<ans2<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
