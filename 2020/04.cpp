#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <vector>

// Because I'm too lazy to type
typedef unsigned long long ull_t;
typedef unsigned u_t;
typedef long long ll_t;

unsigned verb_lvl = 0;

constexpr bool is_hex(char a){
    return (a<='9'&&a>='0')||(a<='f'&&a>='a');
}

int check_passport(const std::string batch) {
    bool has_byr=false,
        has_iyr=false,
        has_eyr=false,
        has_hgt=false,
        has_hcl=false,
        has_ecl=false,
        has_pid=false;
    bool valid_byr=false,
        valid_iyr=false,
        valid_eyr=false,
        valid_hgt=false,
        valid_hcl=false,
        valid_ecl=false,
        valid_pid=false;
    u_t i=1; // passports start with space...

    while (i<batch.size()) {
        if (batch[i]=='b'&&batch[i+1]=='y'&&batch[i+2]=='r') {
            has_byr=true;
            if(verb_lvl>2){std::cout<<"BYR FOUND"<<std::endl;}

            if (batch[i+3] ==':' &&
                (
                    (
                        batch[i+4]=='2'&&
                        batch[i+5]=='0'&&
                        batch[i+6]=='0'&&
                        batch[i+7]<='2'&&
                        batch[i+7]>='0'
                    ) ||
                    (
                        batch[i+4]=='1'&&
                        batch[i+5]=='9'&&
                        batch[i+6]>='2'&&
                        batch[i+6]<='9'&&
                        batch[i+7]<='9'&&
                        batch[i+7]>='0'
                    )
                )
                && (batch[i+8]==' '||batch[i+8]=='\0')
            ) {
                    valid_byr=true;
                if(verb_lvl>2){std::cout<<"BYR VALID"<<std::endl;}
            }
        } else if (batch[i]=='i'&&batch[i+1]=='y'&&batch[i+2]=='r') {
            has_iyr=true;
            if(verb_lvl>2){std::cout<<"IYR FOUND"<<std::endl;}

            if (
                batch[i+3] ==':' &&
                batch[i+4]=='2'&&
                batch[i+5]=='0'&&
                (
                    (
                        batch[i+6]=='1'&&
                        batch[i+7]>='0'&&
                        batch[i+7]<='9'
                    )||(
                        batch[i+6]=='2'&&
                        batch[i+7]=='0'
                    )
                )&&
                (batch[i+8]==' '||batch[i+8]=='\0')
            ) {
                valid_iyr=true;
                if(verb_lvl>2){std::cout<<"IYR VALID"<<std::endl;}
            }
        } else if (batch[i]=='e'&&batch[i+1]=='y'&&batch[i+2]=='r') {
            has_eyr=true;
            if(verb_lvl>2){std::cout<<"EYR FOUND"<<std::endl;}

            if (
                batch[i+3] ==':' &&
                batch[i+4]=='2'&&
                batch[i+5]=='0'&&
                (
                    (
                        batch[i+6]=='2'&&
                        batch[i+7]>='0'&&
                        batch[i+7]<='9'
                    )||(
                        batch[i+6]=='3'&&
                        batch[i+7]=='0'
                    )
                ) &&
                (batch[i+8]==' '||batch[i+8]=='\0')
            ) {
                valid_eyr=true;
                if(verb_lvl>2){std::cout<<"IYR VALID"<<std::endl;}
            }
        } else if (batch[i]=='h'&&batch[i+1]=='g'&&batch[i+2]=='t') {
            has_hgt=true;
            if(verb_lvl>2){std::cout<<"HGT FOUND"<<std::endl;}
            if (batch[i+3] == ':' ) {
                u_t hgt=0,j=i+4;
                while (batch[j]<='9'&&batch[j]>='0'){
                    hgt=hgt*10+(batch[j]-'0');
                    j++;
                }
                if (batch[j]=='c'&&batch[j+1]=='m'&&hgt<=193&&hgt>=150&& (batch[j+2]==' '||batch[j+2]=='\0')) {
                    valid_hgt=true;
                    if(verb_lvl>2)std::cout<<"HGT in cm VALID"<<std::endl;
                } else if (batch[j]=='i'&&batch[j+1]=='n'&&hgt<=76&&hgt>=59&& (batch[j+2]==' '||batch[j+2]=='\0')) {
                    valid_hgt=true;
                    if(verb_lvl>2)std::cout<<"HGT in in VALID"<<std::endl;
                }
            }
        } else if (batch[i]=='h'&&batch[i+1]=='c'&&batch[i+2]=='l') {
            has_hcl=true;
            if(verb_lvl>2){std::cout<<"HCL FOUND"<<std::endl;}

            if (
                batch[i+3]==':' &&
                batch[i+4] == '#' &&
                is_hex(batch[i+5]) &&
                is_hex(batch[i+6]) &&
                is_hex(batch[i+7]) &&
                is_hex(batch[i+8]) &&
                is_hex(batch[i+9]) &&
                is_hex(batch[i+10]) &&
                (batch[i+11]==' '||batch[i+11]=='\0')
            ) {
                valid_hcl=true;
                if(verb_lvl>2){std::cout<<"HCL VALID"<<std::endl;}
            }
        } else if (batch[i]=='e'&&batch[i+1]=='c'&&batch[i+2]=='l') {
            has_ecl=true;
            if(verb_lvl>2){std::cout<<"ECL FOUND"<<std::endl;}

            if (
                batch[i+3]==':' &&
                (
                    (
                        batch[i+4] == 'a'&&
                         batch[i+5]=='m'&&
                         batch[i+6]=='b'
                     )||(
                         batch[i+4] == 'b' &&
                         batch[i+5]=='l'&&
                         batch[i+6]=='u'
                     )||(
                         batch[i+4] == 'b'&&
                         batch[i+5]=='r'&&
                         batch[i+6]=='n'
                     )||(
                         batch[i+4] == 'g'&&
                         batch[i+5]=='r'&&
                         batch[i+6]=='y'
                     )||(
                         batch[i+4] == 'g'&&
                         batch[i+5]=='r'&&
                         batch[i+6]=='n'
                     )||(
                         batch[i+4] == 'h'&&
                         batch[i+5]=='z'&&
                         batch[i+6]=='l'
                     )||(
                         batch[i+4] == 'o'&&
                          batch[i+5]=='t'&&
                          batch[i+6]=='h'
                     )
                ) && (batch[i+7]==' '||batch[i+7]=='\0')
            ) {
                valid_ecl=true;
                if(verb_lvl>2){std::cout<<"ECL VALID"<<std::endl;}
            }
        } else if (batch[i]=='p'&&batch[i+1]=='i'&&batch[i+2]=='d') {
            has_pid=true;
            if(verb_lvl>2){std::cout<<"PID FOUND"<<std::endl;}

            if (
                batch[i+3]==':' &&
                batch[i+4]<='9'&&
                batch[i+4]>='0' &&
                batch[i+5]<='9'&&
                batch[i+5]>='0' &&
                batch[i+6]<='9'&&
                batch[i+6]>='0'&&
                batch[i+7]<='9'&&
                batch[i+7]>='0'&&
                batch[i+8]<='9'&&
                batch[i+8]>='0'&&
                batch[i+9]<='9'&&
                batch[i+9]>='0'&&
                batch[i+10]<='9'&&
                batch[i+10]>='0'&&
                batch[i+11]<='9'&&
                batch[i+11]>='0'&&
                batch[i+12]<='9'&&
                batch[i+12]>='0'&&
                (batch[i+13]==' '||batch[i+13]=='\0')
            ) {
                valid_pid=true;
                if(verb_lvl>2){std::cout<<"PID VALID"<<std::endl;}
            }
        } else if (batch[i]!='c'||batch[i+1]!='i'||batch[i+2]!='d') {
            std::cerr<<"Did not expect "<<batch[i]<<batch[i+1]<<batch[i+2]<<" in "<<batch<<std::endl;
            exit(1);
        }
        while (batch[i++] != ' ');
        if (verb_lvl > 1) {
            std::cout<<"Now at "<<i<<": "<<batch[i]<<batch[i+1]<<batch[i+2]<<std::endl;
        }
    }

    if (verb_lvl > 1) {
        if (!has_byr) {
            std::cout<<"missing byr"<<std::endl;
        } else if (!has_iyr) {
            std::cout<<"missing iyr"<<std::endl;
        } else if (!has_eyr) {
            std::cout<<"missing eyr"<<std::endl;
        } else if (!has_hgt) {
            std::cout<<"missing hgt"<<std::endl;
        } else if (!has_hcl) {
            std::cout<<"missing hcl"<<std::endl;
        } else if (!has_ecl) {
            std::cout<<"missing ecl"<<std::endl;
        } else if (!has_pid) {
            std::cout<<"missing pid"<<std::endl;
        } else {
            std::cout<<"Found one! ";
            if (!valid_byr) {
                std::cout<<"invalid byr"<<std::endl;
            } else if (!valid_iyr) {
                std::cout<<"invalid iyr"<<std::endl;
            } else if (!valid_eyr) {
                std::cout<<"invalid eyr"<<std::endl;
            } else if (!valid_hgt) {
                std::cout<<"invalid hgt"<<std::endl;
            } else if (!valid_hcl) {
                std::cout<<"invalid hcl"<<std::endl;
            } else if (!valid_ecl) {
                std::cout<<"invalid ecl"<<std::endl;
            } else if (!valid_pid) {
                std::cout<<"invalid pid"<<std::endl;
            } else {
                std::cout<<"It is valid!"<<std::endl;
            }
        }
    }

    if (    has_byr &&
        has_iyr &&
        has_eyr &&
        has_hgt &&
        has_hcl &&
        has_ecl &&
        has_pid
    ) {
        if (valid_byr &&
        valid_iyr &&
        valid_eyr &&
        valid_hgt &&
        valid_hcl &&
        valid_ecl &&
        valid_pid) return 2;
        return 1;
    }
    return 0;
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

    u_t valids=0,valids_2;

    while (!input.eof()) {
        std::string batch="",line="";
        do {
            std::getline(input, line);
            batch += " " + line;
        } while(line.size()>0);
        if(verb_lvl>0) {
            std::cout<<"checking passport"<<batch<<std::endl;
        }
        switch(check_passport(batch)) {
            case 2:
                valids_2++;
                // no break
            case 1:
                valids++;
        }
    }

    std::cout<<valids<<' '<<valids_2<<std::endl;

    input.close();

    return 0;
}

int main (int argc, char** argv) {
    auto start = std::chrono::high_resolution_clock::now();

    real_main(argc,argv);

    auto stop = std::chrono::high_resolution_clock::now();
    std::cout<<"Duration: "<<std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count()<<std::endl;
}
