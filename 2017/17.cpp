#include <iostream>
#include <vector>

struct buffer_loc {
    unsigned prev,next;
    buffer_loc(unsigned a,unsigned b);
};

buffer_loc::buffer_loc(unsigned a,unsigned b){prev=a;next=b;}

int main() {
    std::vector<buffer_loc> buffers;

    buffers.push_back(buffer_loc(0,0));
    unsigned cur_pos = 0, input=328;
    while(buffers.size()<50000000){
        for (unsigned i=0;i<input;i++)
            cur_pos = buffers[cur_pos].next;


        buffers.push_back( buffer_loc( cur_pos, buffers[cur_pos].next ));
        buffers[buffers[cur_pos].next].prev=buffers.size()-1;
        buffers[cur_pos].next=buffers.size()-1;
        cur_pos=buffers.size()-1;

        if(!(buffers.size()%1000000))std::cout<<":"<<buffers.size()<<':'<<std::endl;
    }
    std::cout<<buffers[0].next<<std::endl;
    return 0;
}
