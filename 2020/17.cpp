#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <string>
#include <array>
#include <tuple>

// Because I'm too lazy to type
typedef uint64_t ull_t;
typedef uint32_t u_t;
typedef int64_t ll_t;

unsigned verb_lvl = 0;

template<typename T>
struct line {
private:
    std::vector<T> f,b;
public:
    T& operator[](int a) {
        if (a < 0) { //return b[-1-a]
            a = -1-a;
            if (static_cast<unsigned>(a) >= b.size()) {
                b.resize(a + 1, 0);
            }
            return b[a];
        } else { // return f[a]
            if (static_cast<unsigned>(a) >= f.size()) {
                f.resize(a+1, 0);
            }
            return f[a];
        }
    }
    line(){};
    line(int a) {a++;};
    std::pair<int, int> get_bounds() {
        return {-1*static_cast<int>(b.size()), static_cast<int>(f.size())};
    }
};

typedef struct cube_elt {
    bool alive = false;
    cube_elt(bool a):alive(a){};
} cube_elt_t;

typedef line<line<line<cube_elt_t>>> cube_t;
typedef line<line<line<line<cube_elt_t>>>> hcube_t;

const std::array<std::array<int,3>, 26> dirs = {{
    { 0,  0,  1}, //0
    { 0,  1,  1}, //1
    { 0, -1,  1}, //2
    { 1,  0,  1}, //3
    { 1,  1,  1}, //4
    { 1, -1,  1}, //5
    {-1,  0,  1}, //6
    {-1,  1,  1}, //7
    {-1, -1,  1}, //8
    { 0,  1,  0}, //9
    { 0, -1,  0}, //10
    { 1,  0,  0}, //11
    { 1,  1,  0}, //12
    { 1, -1,  0}, //13
    {-1,  0,  0}, //14
    {-1,  1,  0}, //15
    {-1, -1,  0}, //16
    { 0,  0, -1}, //17
    { 0,  1, -1}, //18
    { 0, -1, -1}, //19
    { 1,  0, -1}, //20
    { 1,  1, -1}, //21
    { 1, -1, -1}, //22
    {-1,  0, -1}, //23
    {-1,  1, -1}, //24
    {-1, -1, -1}  //25
}};

unsigned count_neighbours(cube_t& c, int i, int j, int k) {
    unsigned ans = 0;
    for (auto a:dirs) {
        if (c[i + a[0]][j + a[1]][k + a[2]].alive) {
            ans++;
        }
    }
    return ans;
}

unsigned count_neighbours(hcube_t& cube, int i, int j, int k, int l) {
    unsigned ans = 0;
    for (int a=-1; a<=1; a++)
        for (int b=-1; b<=1; b++)
            for (int c=-1; c<=1; c++)
                for (int d=-1; d<=1; d++)
                    if ((a!=0 || b!=0 || c!=0 || d!=0) && cube[i+a][j+b][k+c][l+d].alive)
                        ans++;
    return ans;
}

void print_cube(cube_t& my_cube, std::ostream& o) {
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;

    o <<"Size: "<<i_min<<'-'<<i_max<<','<<j_min<<'-'<<j_max<<','<<k_min<<'-'<<k_max<<std::endl;

    for (
        int i = i_min-1;
        i < i_max+1;
        i++
    ) {
        o<<"\nLayer "<<i<<":\n";
        for (
            int j = j_min-1;
            j < j_max+1;
            j++
        ) {
            for (
                int k = k_min-1;
                k < k_max+1;
                k++
            ) {
                o<<(my_cube[i][j][k].alive ? static_cast<char>('a' + count_neighbours(my_cube, i, j, k)) : static_cast<char>('0' + count_neighbours(my_cube, i, j, k)));// : '.');
            }
            o << std::endl;
        }
    }
}

void print_hcube(hcube_t& my_cube, std::ostream& o) {
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;
    int l_min = my_cube[0][0][0].get_bounds().first, l_max = my_cube[0][0][0].get_bounds().second;

    o <<"Size: "<<i_min<<'-'<<i_max<<','<<j_min<<'-'<<j_max<<','<<k_min<<'-'<<k_max<<std::endl;

    for (
        int i = i_min-1;
        i < i_max+1;
        i++
    ) {
        for (
            int j = j_min-1;
            j < j_max+1;
            j++
        ) {
            o<<"\nLayer "<<i<<','<<j<<":\n";
            for (
                int k = k_min-1;
                k < k_max+1;
                k++
            ) {
                for (
                    int l=l_min-1;
                    l<l_max+1;
                    l++
                ) {
                    o<<(my_cube[i][j][k][l].alive ? static_cast<char>('a' + count_neighbours(my_cube, i, j, k, l)) : static_cast<char>('A' + count_neighbours(my_cube, i, j, k, l)));// : '.');
                }
                o << std::endl;
            }
        }
    }
}

u_t count_alive(cube_t& my_cube) {
    u_t ans=0;
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;
    for (
        int i = i_min;
        i < i_max;
        i++
    ) {
        for (
            int j = j_min;
            j < j_max;
            j++
        ) {
            for (
                int k = k_min;
                k < k_max;
                k++
            ) {
                if (my_cube[i][j][k].alive)
                    ans++;
            }
        }
    }
    return ans;
}

u_t count_alive(hcube_t& my_cube) {
    u_t ans=0;
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;
    int l_min = my_cube[0][0][0].get_bounds().first, l_max = my_cube[0][0][0].get_bounds().second;
    for (
        int i = i_min;
        i < i_max;
        i++
    ) {
        for (
            int j = j_min;
            j < j_max;
            j++
        ) {
            for (
                int k = k_min;
                k < k_max;
                k++
            ) {
                for (
                    int l = l_min;
                    l < l_max;
                    l++
                ) {
                    if (my_cube[i][j][k][l].alive)
                        ans++;
                }
            }
        }
    }
    return ans;
}

void do_step(cube_t& my_cube, cube_t& new_cube) {
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;
    for (
        int i = i_min - 1;
        i <= i_max;
        i++
    ) {
        for (
            int j = j_min-1;
            j <= j_max;
            j++
        ) {
            for (
                int k = k_min -1;
                k <= k_max;
                k++
            ) {
                if (my_cube[i][j][k].alive) {
                    u_t n = count_neighbours(my_cube, i,j,k);
                    new_cube[i][j][k] = (n == 2 || n == 3);
                } else {
                    new_cube[i][j][k] = count_neighbours(my_cube,i,j,k) == 3;
                }
            }
        }
    }
}
void do_step(hcube_t& my_cube, hcube_t& new_cube) {
    int i_min = my_cube.get_bounds().first, i_max = my_cube.get_bounds().second;
    int j_min = my_cube[0].get_bounds().first, j_max = my_cube[0].get_bounds().second;
    int k_min = my_cube[0][0].get_bounds().first, k_max = my_cube[0][0].get_bounds().second;
    int l_min = my_cube[0][0][0].get_bounds().first, l_max = my_cube[0][0][0].get_bounds().second;
    for (
        int i = i_min - 1;
        i <= i_max;
        i++
    ) {
        for (
            int j = j_min-1;
            j <= j_max;
            j++
        ) {
            for (
                int k = k_min -1;
                k <= k_max;
                k++
            ) {
                for (
                    int l = l_min -1;
                    l <= l_max;
                    l++
                ) {
                    if (my_cube[i][j][k][l].alive) {
                        u_t n = count_neighbours(my_cube, i,j,k,l);
                        new_cube[i][j][k][l] = (n == 2 || n == 3);
                    } else {
                        new_cube[i][j][k][l] = count_neighbours(my_cube,i,j,k,l) == 3;
                    }
                }
            }
        }
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

    cube_t my_cube[2];
    hcube_t my_hcube[2];

    int y=0;

    std::string my_line;

    while (!input.eof()) {
        std::getline(input, my_line);

        for (u_t i=0;i<my_line.size();i++) {
            my_cube[0][0][y][i] = my_line[i] == '#';
            my_hcube[0][0][0][y][i] = my_line[i] == '#';
        }

        y++;
    }

    input.close();

    u_t ind=0;
    for (int i=0;i<6;i++) {
        do_step(my_cube[ind], my_cube[1-ind]);
        do_step(my_hcube[ind], my_hcube[1-ind]);
        ind = 1-ind;
    }

    if (verb_lvl > 10) {
        print_cube(my_cube[ind],std::cout);
        print_hcube(my_hcube[ind],std::cout);
    }

    std::cout<<count_alive(my_cube[ind])<<' '<<count_alive(my_hcube[ind])<<std::endl;

    

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    int result = real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
    return result;
}
