#include <iostream>
#include <fstream>
#include <chrono>
#include <string>

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

class blueprint {
public:
    u_t id, ore_ore, clay_ore, obby_ore, obby_clay, geo_ore, geo_obby;
    blueprint() {}
    void print() const {
        std::cout<<"Blueprint "<<id<<":\n Each ore robot costs "<<ore_ore
                 <<" ore.\n Each clay robot costs "<<clay_ore<<" ore.\n Each obsidian robot costs "
                 <<obby_ore<<" ore and "<<obby_clay<<" clay.\n Each geode robot costs "<<geo_ore
                 <<" ore and "<<geo_obby<<" obsidian.\n"<<std::endl;
    }
};

blueprint read_blueprint(std::istream& i) {
    blueprint rtr;
    read_or_die("Blueprint ",i);
    i>>rtr.id;
    read_or_die(": Each ore robot costs ",i);
    i>>rtr.ore_ore;
    read_or_die(" ore. Each clay robot costs ",i);
    i>>rtr.clay_ore;
    read_or_die(" ore. Each obsidian robot costs ",i);
    i>>rtr.obby_ore;
    read_or_die(" ore and ",i);
    i>>rtr.obby_clay;
    read_or_die(" clay. Each geode robot costs ",i);
    i>>rtr.geo_ore;
    read_or_die(" ore and ",i);
    i>>rtr.geo_obby;
    read_or_die(" obsidian.\n",i);
    return rtr;
}

u_t final_max_limit_no_further = 24;
class state {
public:
    u_t ore_robots=1, clay_robots=0, obby_robots=0, geo_robots=0;
    u_t ores=0, clays=0, obbys=0, geos=0;
    u_t next_minute=1;
    bool step() {
        if (next_minute > final_max_limit_no_further) {
            return false;
        }
        ores += ore_robots;
        clays += clay_robots;
        obbys += obby_robots;
        geos += geo_robots;
        if(verb_lvl>1)std::cout<<"== Minute "<<next_minute<<" ==\n"
                 <<ore_robots<<" ore-collecting robots collect as many ores; you now have "<<ores<<" ore.\n"
                 <<clay_robots<<" clay-collecting robots collect as many clay; you now have "<<clays<<" clay.\n"
                 <<obby_robots<<" obsidian-collecting robots collect as many obsidian; you now have "<<obbys<<" obsidian.\n"
                 <<geo_robots<<" geode-cracking robots crack as many geodes; you now have "<<geos<<" open geodes.\n"<<std::endl;
        next_minute++;
        return true;
    }
};

u_t build_robots(state s, blueprint b, u_t ore_max, u_t next_robot, ull_t ppid) {
    static ull_t pid = 0;
    ull_t mpid=++pid;
    u_t result, max_result = 0;
    bool did_step=true;
    if (next_robot == 1) {
        while(s.ores < b.ore_ore && (did_step=s.step()));
        if (did_step) {
            s.ores -= b.ore_ore;
            s.step();
            s.ore_robots++;
        } else {
            if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<s.geos<<std::endl;
            return s.geos;
        }
    } else if (next_robot == 2) {
        while (s.ores < b.clay_ore && (did_step=s.step()));
        if (did_step) {
            s.ores -= b.clay_ore;
            s.step();
            s.clay_robots++;
        } else {
            if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<s.geos<<std::endl;
            return s.geos;
        }
    } else if (next_robot == 3) {
        if (s.clay_robots == 0) {
            std::cerr<<"cannot build obby robots without clay!"<<std::endl;
            exit(1);
        }
        while ((s.ores < b.obby_ore || s.clays < b.obby_clay) && (did_step=s.step()));
        if (did_step) {
            s.ores -= b.obby_ore;
            s.clays -= b.obby_clay;
            s.step();
            s.obby_robots++;
        } else {
            if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<s.geos<<std::endl;
            return s.geos;
        }
    } else if (next_robot == 4) {
        if (s.obby_robots == 0) {
            std::cerr<<"cannot build geo robots without obby!"<<std::endl;
            exit(1);
        }
        while ((s.ores < b.geo_ore || s.obbys < b.geo_obby) && (did_step=s.step()));
        if (did_step) {
            s.ores -= b.geo_ore;
            s.obbys -= b.geo_obby;
            s.step();
            s.geo_robots++;
        } else {
            if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<s.geos<<std::endl;
            return s.geos;
        }
    }
    if (s.ores >= b.geo_ore && s.obbys >= b.geo_obby) {
        // Always build geo robots if you can.
        result = build_robots(s,b,ore_max,4,mpid);
        // We assume building geo robot is optimal.
        if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<result<<std::endl;
        return result;
    }
    if (ore_max > s.ore_robots && s.next_minute < 12) {
        // Try building an ore robot, it can't hurt right?
        // Building ore robot very early is always best (Maybe build 1 or 2 clay robots before last ore robot).
        // Manual work showed that all ore robots needed should be build before minute 12 
        max_result = build_robots(s,b,ore_max,1,mpid);
    }
    if (b.obby_clay > s.clay_robots && 2*b.obby_clay > s.clays) {// Usually, you don't want to build clay robots when you can build loads of obby robots
        result = build_robots(s,b,ore_max,2,mpid);
        if (result > max_result)max_result = result;
    }
    if (s.clay_robots > 0) {
        result = build_robots(s,b,ore_max,3,mpid);
        if (result > max_result) max_result = result;
    }
    if (s.obby_robots > 0) {// Always build geo robots, be hopefull!
        result = build_robots(s,b,ore_max,4,mpid);
        if (result > max_result) max_result = result;
    }
    if(verb_lvl>1)std::cout<<mpid<<" started by "<<ppid<<" returns "<<max_result<<std::endl;
    return max_result;
}

ull_t calc_ans_1(blueprint b) {
    state s;
    u_t max_ore_robots = b.clay_ore, result;
    if (b.obby_ore>max_ore_robots)max_ore_robots=b.obby_ore;
    if (b.geo_ore>max_ore_robots)max_ore_robots=b.geo_ore;
    return std::max(build_robots(s,b,max_ore_robots,0,0),build_robots(s,b,max_ore_robots,1,0));
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

    ull_t ans=0, ans2=1;
    u_t cnt=0;
    while(input.peek()=='B') {
        blueprint b=read_blueprint(input);
        ull_t ans1 = calc_ans_1(b);
        if(verb_lvl)b.print();
        if (cnt<3) {
            final_max_limit_no_further = 32;
            ull_t ansie = calc_ans_1(b);
            final_max_limit_no_further = 24;
            ans2 *= ansie;
            if (verb_lvl)std::cout<<"ans2: "<<ansie<<std::endl;
        }
        if(verb_lvl)std::cout<<"ans1: "<<ans1<<'\n'<<std::endl;
        
        ans += ans1*b.id;
        cnt++;
    }
    input.close();

    std::cout<<ans<<' '<<ans2<<std::endl;

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
