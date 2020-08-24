#include <iostream>
#include <string>

int main() {
    long unsigned nice_cnt=0;
    std::string line;
    bool double_pair, sandwitch;

    while(!std::cin.eof()) {
        std::getline(std::cin, line);
        double_pair = false;
        sandwitch=false;
        for (unsigned i=0; i < line.length()-3; i++){
            for(unsigned j=i+2; j < line.length()-1; j++)
                if(line[i]==line[j] && line[i+1]==line[j+1])
                    double_pair=true;
            if (line[i]==line[i+2] || line[i+1]==line[i+3])
                sandwitch = true;
            if(double_pair && sandwitch) {
                std::cout<<(++nice_cnt)<<' ';
                break;
            }
        }
        std::cout<<line << ' ' << double_pair<<' '<<sandwitch<<' '<<nice_cnt<<std::endl;
    }
    std::cout<<"answer: "<<nice_cnt<<std::endl;
    return 0;
}
