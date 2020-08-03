#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {name of input file}"<<std::endl;
        return 0;
    }
    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<':'<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }
    std::vector<std::string> field;
    std::string a_line;

    std::getline(input_file,a_line);
    while(a_line.length() && !input_file.eof()){
        field.push_back(a_line);
        std::getline(input_file,a_line);
    }

    for(auto a:field)std::cout<<a<<std::endl;
    const std::pair<int,int> up=std::make_pair(-1,0),down=std::make_pair(1,0),left=std::make_pair(0,-1),right=std::make_pair(0,1);
    std::pair<int,int> direction=down;
    std::pair<unsigned,unsigned> pos=std::make_pair(0,0);

    while(field[pos.first][pos.second] != '|')pos.second++;

    a_line = "";
    unsigned steps=0;
    while(field[pos.first][pos.second]!=' '){
        ++steps;
        if (field[pos.first][pos.second] >='A'&&field[pos.first][pos.second]<='Z')
            a_line.push_back(field[pos.first][pos.second]);
        else if(field[pos.first][pos.second]=='+'){
            if (direction!=down&&pos.first!=0&&field[pos.first-1][pos.second]=='|')
                direction=up;
            else if(direction!=up&&pos.first!=field.size()-1&&field[pos.first+1][pos.second]=='|')
                direction=down;
            else if (direction!=right&&pos.second!=0&&field[pos.first][pos.second-1]=='-')
                direction=left;
            else if(direction!=left&&pos.second!=field[pos.first].size()-1&&field[pos.first][pos.second+1]=='-')
                direction=right;
            else {
                std::cout<<"HELP!!"<<std::endl;return 1;
            }
        }
        pos.first+=direction.first;
        pos.second+=direction.second;
    }
    std::cout<<"THE ANSWERS ARE "<<a_line<<" AND "<<steps<<std::endl;

    return 0;
}
