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

struct card {
    uint32_t number;
    card* prev = NULL;
    card(uint32_t n): number(n) {};
};

struct deck {
    card *first = NULL, *last = NULL;
    uint32_t size = 0;

    void push_back(card* c) {
        if (last == NULL) {
            last = first = c;
        } else {
            last->prev = c;
            last = c;
        }
        size++;
    }

    card* pop_front() {
        card* ans = first;
        first = first->prev;
        ans->prev = NULL;
        if (first == NULL)
            last = NULL;
        size--;
        return ans;
    }

    void clear() {
        while (first != NULL) delete pop_front();
    }

    void copy_from(deck& d, uint32_t s) {
        if (d.size < s) {
            std::cerr<<"Cannot copy "<<s<<" elements from list of size "<<d.size<<std::endl;
            exit(1);
        }
        clear();
        card*c=d.first;
        for (uint32_t i=0;i<s;i++) {
            this->push_back(new card(c->number));
            c = c->prev;
        }
    }

    deck(deck& d) {
        copy_from(d, d.size);
    }

    deck& operator=(deck& d) {
        if (this != &d) {
            copy_from(d, d.size);
        }
        return *this;
    }

    deck(){};

    ~deck(){clear();};

    bool operator==(deck& d) {
        card *c=d.first, *o=first;
        while (c!= NULL && o!=NULL) {
            if (c->number != o->number) {
                return false;
            }
            c = c->prev;
            o = o->prev;
        }
        return c==NULL && o==NULL;
    }
};

uint64_t calc_score(deck& d) {
    uint64_t ans=0;
    card* c=d.first;
    for (uint64_t i=d.size;i>0;i--) {
        ans += c->number * i;
        c = c->prev;
    }
    if (c != NULL) {
        std::cerr<<"WHOOPS"<<std::endl;
        exit(1);
    }
    return ans;
}

uint32_t play_game(deck& d1, deck&d2, bool recursive) {
    static uint32_t depth = 0;
    depth++;
    deck control1, control2;
    uint32_t window=1, step=1, winner=1;

    while (d1.size && d2.size) {
        if (window == step) {
            control1 = d1;
            control2 = d2;
            window <<= 1;
        }
        if (verb_lvl > 0 && depth < verb_lvl && recursive) {
            std::cout<<"At depth "<<depth<<std::endl;
            std::cout<<"Deck 1 with "<<d1.size<<" cards: ";
            card* c = d1.first;
            while (c != NULL) {
                std::cout<<c->number<<' ';
                c = c->prev;
            }

            std::cout<<"\nControl 1 with "<<control1.size<<" cards: ";
            c = control1.first;
            while (c != NULL) {
                std::cout<<c->number<<' ';
                c = c->prev;
            }

            std::cout<<"\nDeck 2 with "<<d2.size<<" cards: ";
            c = d2.first;
            while (c != NULL) {
                std::cout<<c->number<<' ';
                c = c->prev;
            }

            std::cout<<"\nControl 2 with "<<control2.size<<" cards: ";
            c = control2.first;
            while (c != NULL) {
                std::cout<<c->number<<' ';
                c = c->prev;
            }
            std::cout<<std::endl;
        }
        card* c1 = d1.pop_front(), *c2 = d2.pop_front();

        if (recursive && c1->number <= d1.size && c2->number <= d2.size) {
            deck o1,o2;

            o1.copy_from(d1, c1->number);
            o2.copy_from(d2, c2->number);

            winner = play_game(o1, o2, true);
        } else if (c1->number > c2->number) {
            winner = 1;
        } else {
            winner = 2;
        }
        switch (winner) {
            case 1:
                d1.push_back(c1);
                d1.push_back(c2);
                break;
            case 2:
                d2.push_back(c2);
                d2.push_back(c1);
                break;
        }

        if (d1 == control1 && d2 == control2) {
            if (depth < verb_lvl)
                std::cout<<"LOOP DETECTED"<<std::endl;
            depth--;
            return 1;
        }
        step++;
    }

    depth--;
    return winner;
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

    deck d1,d2,D1,D2;

    read_or_die("Player 1:\n", input);
    uint32_t n;

    while (input.peek()!='\n') {
        input>>n;
        d1.push_back(new card(n));
        read_or_die("\n", input);
    }

    if (verb_lvl > 0) {
        std::cout<<"Deck 1 with "<<d1.size<<" cards: ";
        card* c = d1.first;
        while (c != NULL) {
            std::cout<<c->number<<' ';
            c = c->prev;
        }std::cout<<std::endl;
    }
    read_or_die("\nPlayer 2:\n", input);

    while (input.peek() != -1) {
        input >> n;
        d2.push_back(new card(n));
        read_or_die("\n", input);
    }

    input.close();

    D1=d1;
    D2=d2;

    switch(play_game(d1, d2, false)) {
        case 1:
            std::cout<<calc_score(d1)<<std::endl;;
            break;
        case 2:
            std::cout<<calc_score(d2)<<std::endl;
            break;
    }
    switch(play_game(D1, D2, true)) {
        case 1:
            std::cout<<calc_score(D1)<<std::endl;
            break;
        case 2:
            std::cout<<calc_score(D2)<<std::endl;
    }

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
