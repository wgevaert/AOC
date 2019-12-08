#include <iostream>

int main() {
    const int width = 25, height = 6, size=width*height;
    short image[width][height];
    for(int i=0;i<size;i++)image[i%width][i/width]=2;
    int index=0;
    char a;
    while ((a=std::cin.get())<='9' && a>='0') {
        if (!(index%size)) {// In a new layer.
            index = 0; // to prevent a possible overflow I guess...
        }
        if (image[index%width][index/width] == 2)
            image[index%width][index/width] = a - '0';
    ++index;
    }
    for (int i=0;i<size;i++){if(!(i%width))std::cout<<std::endl;std::cout<<(image[i%width][i/width] ? 'x':'.');}
    std::cout<<std::endl;
    return 0;
}
