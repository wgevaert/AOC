#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>

unsigned get_X(std::ifstream & input_file) {
    if(input_file.get()!=' '){std::cout<<"OH NOoES!"<<std::endl;exit(1);}
    unsigned rtr=static_cast<unsigned>(input_file.get()-'a');
    if(rtr>=26){std::cout<<"HERE IS THE MISTAKE!"<<static_cast<char>(rtr+'a');exit(1);}
    return rtr;
}

long long get_Y(std::ifstream & input_file,bool&relative) {
    if(input_file.get()!=' '){std::cout<<"OH NOoS!"<<std::endl;exit(1);}
    char a=input_file.get(),A;
    long long b=0,sign=1;
    if (a>='a'&&a<='z'){
        if((A=input_file.get())!='\n'){if(A==' '){input_file.unget();}else {std::cout<<"OH NO!"<<std::endl;exit(1);}}
        relative=true;
        return static_cast<long long>(a-'a');
    }
    if (a=='-'){sign=-1;a=input_file.get();}
    while(a!='\n'&&a!=' '){
        if('0'>a||a>'9'){std::cout<<"OH NO!"<<a<<std::endl;exit(1);}
        b = b*10+a-'0';
        a=input_file.get();
    }
    if(a==' ')input_file.unget();
    relative=false;
    return sign*b;
}

template <class T>
struct pipe_block {
    pipe_block<T>* prev=NULL;
    T content;
};

template <class T>
struct pipe {
private:
    pipe_block<T>*last=NULL,*first=NULL;
    unsigned size=0;
public:
    void push_back(T content);
    T pop_front();
    bool is_empty();
    unsigned get_size();
};

template <class T>
void pipe<T>::push_back(T content){
    if(first==NULL){
        last = first = new pipe_block<T>;
//        std::cout<<"MADE BLOCK "<<last<<' '<<first<<std::endl;
        last->content=content;
    } else {
        if(last==NULL)std::cout<<"WHY MUST THIS HAPPEN TO ME?"<<std::endl;
        last->prev = new pipe_block<T>;
        last = last->prev;
        last->content = content;
    }
    size++;
}

template <class T>
T pipe<T>::pop_front(){
    pipe_block<T>* the_first = first;
    if(first==NULL) std::cout<<"WHYY??"<<std::endl;
    first = the_first->prev;
//    if(first==NULL) {std::cout<<"PIPE IS NOW EMPTY AGAIN"<<std::endl;}
    T cont = the_first->content;
    delete the_first;
//    size--;
    return cont;
}

template<class T>
bool pipe<T>::is_empty(){
    return first==NULL;
}

template<class T>
unsigned pipe<T>::get_size(){
    return size;
}

struct register_machine {
    long long registers[26]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    pipe<long long>* pipe_front;
    pipe<long long> pipe_back;
    unsigned pos;
    int id;
    unsigned packets_send;
};

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: {program} {name of input file}"<<std::endl;
        return 0;
    }
    pipe<long long> test_pipe;
    test_pipe.push_back(0);test_pipe.push_back(1);test_pipe.push_back(2);test_pipe.push_back(3);
    std::cout<<std::endl<<test_pipe.pop_front()<<' ';
    std::cout<<std::endl<<test_pipe.pop_front()<<' ';
    std::cout<<std::endl<<test_pipe.pop_front()<<' ';
    std::cout<<std::endl<<test_pipe.pop_front()<<' ';

    std::string filename = argv[1];
    std::cout<<"reading file "<<filename<<std::endl;

    std::ifstream input_file;
    input_file.open(filename);
    if (!input_file.good()) {
        std::cout<<"something wrong with "<<filename<<std::endl;
        return 0;
    }

    long long Y,X_l;
    unsigned X;
    bool relative, rel_X;

char p,q;
    std::vector<std::function<bool(register_machine&)>> operations;
    while(!input_file.eof()) {
//std::cout<<operations.size()<<':';
        switch(p=input_file.get()) {
            case 's':
                switch(q=input_file.get()) {
                    case 'n':
                        if(input_file.get()!='d'){
                            std::cout<<"OOPS"<<std::endl;
                            return 1;
                        }
                        Y = get_Y(input_file,relative);
//std::cout<<X<<'x'<<std::endl;
                        operations.push_back(
                            [Y,relative](register_machine& m) {
                                long long y = (relative?m.registers[Y]:Y);
                                std::cout<<m.id<<" PUSHED VALUE "<<y<<" ON OPERATION "<<m.pos<<std::endl;
                                m.pipe_back.push_back(y);
                                m.pos++;
                                return false;
                            }
                        );
                        break;
                    case 'e':
                        if(input_file.get()!='t'){
                            std::cout<<"OEPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y =get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](register_machine& m){m.registers[X]=(relative?m.registers[Y]:Y);m.pos++;return false;});
                        break;
                }
                break;
            case 'a':
                if(input_file.get()!='d'||input_file.get()!='d'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                Y = get_Y(input_file,relative);
                operations.push_back([X,Y,relative](register_machine& m){m.registers[X]+=(relative?m.registers[Y]:Y);m.pos++;return false;});
                break;
            case 'm':
                switch(input_file.get()) {
                    case 'u':
                        if(input_file.get()!='l'){
                            std::cout<<"OOPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y =get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](register_machine& m){m.registers[X]*=(relative?m.registers[Y]:Y);m.pos++;return false;});
                        break;
                    case 'o':
                        if(input_file.get()!='d'){
                            std::cout<<"OEPS"<<std::endl;
                            return 1;
                        }
                        X = get_X(input_file);
                        Y = get_Y(input_file,relative);
                        operations.push_back([X,Y,relative](register_machine& m){m.registers[X]%=(relative?m.registers[Y]:Y);m.pos++;return false;});
                        break;
                }
                break;
            case 'r':
                if(input_file.get()!='c'||input_file.get()!='v'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X = get_X(input_file);
                if(input_file.get()!='\n'){std::cout<<"Missing newline..."<<std::endl;return 1;}
                operations.push_back([X](register_machine& m){if(m.pipe_front->is_empty()){std::cout<<"WAITING"<<std::endl;return true;}std::cout<<m.id<<" POPPING FRONT ON "<<static_cast<char>(X+'a')<<" WITH VALUE "<<(m.registers[X]=m.pipe_front->pop_front())<<std::endl;;m.pos++;return false;});
                break;
            case 'j':
                if(input_file.get()!='g'||input_file.get()!='z'){
                    std::cout<<"OEPS"<<std::endl;
                    return 1;
                }
                X_l = get_Y(input_file,rel_X);
                Y = get_Y(input_file,relative);
                operations.push_back([X_l,Y,rel_X,relative](register_machine& m){
                    if( (rel_X?m.registers[X_l]:X_l)>0)
                        m.pos+=(relative?m.registers[Y]:Y);
                    else
                        m.pos++;
                    return false;
                });
                break;
            default:
                if(!input_file.eof()){std::cout<<"HELP!"<<std::endl;return 1;}
        }
//std::cout<<p<<q<<std::endl;
    }

    unsigned long long cnt=-1;
    register_machine r0,r1;
    r0.pipe_front=&r1.pipe_back;
    r1.pipe_front=&r0.pipe_back;
std::cout<<operations.size()<<' '<<cnt/2<<' '<<r0.pipe_front->get_size()<<r1.pipe_front->get_size()<<std::endl;;
    r0.id=0;
    r1.id=1;
    r1.registers['p'-'a']=1;
std::cout<<"HERE!"<<operations[0](r0)<<std::endl;

    for(int i=0;i<26;i++) {
        std::cout<<r0.registers[i]<<','<<r1.registers[i]<<' ';
    } std::cout<<"    "<<r0.pos<<','<<r0.pipe_back.get_size()<<' '<<r1.pos<<','<<r1.pipe_back.get_size()<<std::endl;
    while(r0.pos < operations.size() && r1.pos<operations.size() && (!operations[r0.pos](r0) || !operations[r1.pos](r1)))
    {//std::cout<<cnt++;//if(r1.pos==31)std::cout<<"HERE WE GO!";
//std::cout<<operations.size()<<' '<<cnt++<<'@';
        for(int i=0;i<26;i++){std::cout<<r0.registers[i]<<','<<r1.registers[i]<<' ';}std::cout<<"    "<<r0.pos<<','<<r0.pipe_back.get_size()<<' '<<r1.pos<<','<<r1.pipe_back.get_size()<<std::endl;
    };
std::cout<<"HOOI"<<r0.pipe_front->get_size()<<' '<<r1.pipe_front->get_size()<<std::endl;
        for(int i=0;i<26;i++){std::cout<<r0.registers[i]<<','<<r1.registers[i]<<' ';}std::cout<<"    "<<r0.pos<<','<<r0.pipe_back.get_size()<<' '<<r1.pos<<','<<r1.pipe_back.get_size()<<std::endl;


    return 0;
}
