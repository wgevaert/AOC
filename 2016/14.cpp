#include "md5.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

const unsigned get_from_char(const char a){
    if(a>'9')return a-'a'+10;
    return a-'0';
}

struct valid_key{
    unsigned key,seq;
    char mechar;
    valid_key(unsigned _key,unsigned _seq,char _mechar){key=_key;seq=_seq;mechar=_mechar;}
};
bool operator<(valid_key a,valid_key b){return a.key<b.key;}

std::string superhash(const std::string input){
    std::string cur_hash = md5(input);
    for(unsigned iter=0;iter<2016;iter++){
        cur_hash = md5(cur_hash);
    }
    return cur_hash;
}

int main(const int argc, const char** const argv) {
    if(argc<2){std::cout<<"Usage: {program} {input}"<<std::endl;return 0;}
    unsigned long index = 0;
    std::vector<unsigned long> possible_keys[16];
    std::vector<valid_key> keys;
    unsigned unit=0;
    while(unit <= 1000){
        std::string md5_hash=std::string(argv[1]) + std::to_string(index);
        if(argc < 3)md5_hash = md5(md5_hash);
        else md5_hash = superhash(md5_hash);
        bool pushed = false;
        for(size_t a=0;a<md5_hash.size()-2;a++){
            if(md5_hash[a]==md5_hash[a+1]&&md5_hash[a+1]==md5_hash[a+2]) {
                unsigned cur_char = get_from_char(md5_hash[a]);
                if(a<md5_hash.size()-4 && md5_hash[a]==md5_hash[a+3]&&md5_hash[a]==md5_hash[a+4]){
                    for(int b=possible_keys[cur_char].size()-1;b>=0;b--){
                        if(possible_keys[cur_char][b]>index){std::cout<<"HELP!"<<possible_keys[cur_char][b]<<' '<<index<<' '<<md5_hash<<std::endl;return 1;}
                        else if(possible_keys[cur_char][b]==index) {std::cout<<md5_hash<<" is special "<<std::endl;}
                        else if(possible_keys[cur_char][b]+1000>index){
                            std::cout<<"found key "<<possible_keys[cur_char][b]<<' '<<std::endl;
                            keys.emplace_back(possible_keys[cur_char][b],index,md5_hash[a]);
                        } else {
                            std::cout<<' '<<possible_keys[cur_char][b]<<" is not relevant at "<<index<<std::endl;
                        }
                    }
                    possible_keys[cur_char].clear();
                }
                if(!pushed){
                    std::cout<<"PUSHING "<<index<<" for "<<md5_hash[a]<<std::endl;
                    possible_keys[cur_char].emplace_back(index);
                    pushed = true;
                }
            }
        }
        index++;
        if(keys.size()>=64)unit++;
    }

    std::sort(keys.begin(),keys.end());
    std::cout<<'{';for(auto a:keys)std::cout<<a.key<<',';std::cout<<'}'<<std::endl;
    std::cout<<keys[63].key<<std::endl;
    return 0;
}
