#include <iostream>

const size_t size=8;
char password[size+1]={'a','b','c','d','e','f','g','h','\0'};
unsigned position[size]={0,1,2,3,4,5,6,7};

void set_pwd_from_pos(){
    for(size_t j=0;j<size;j++)password[position[j]]=j+'a';
}

void set_pos_from_pwd(){
	for(size_t j=0;j<size;j++)position[password[j]-'a']=j;
}

void rotate_right(unsigned i){
	char new_pwd[size];
	for(size_t j=0;j<size;j++){
		new_pwd[(j+i)%size]=password[j];
	}
	for(size_t j=0;j<size;j++)position[(password[j]=new_pwd[j])-'a']=j;
}

void rotate_left(unsigned i){
	rotate_right(size-i);
}

void swap_letter(char a,char b){
	size_t x=position[a-'a'];
	position[a-'a']=position[b-'a'];
	position[b-'a']=x;
	set_pwd_from_pos();
}
void swap_position(size_t a,size_t b){
	char x=password[a];
	password[a]=password[b];
	password[b]=x;
	set_pos_from_pwd();
}

void move_position(size_t a,size_t b){
	if (a<b){
		for(size_t i=0;i<size;i++)if(a<position[i]&&position[i]<=b)position[i]--;
		position[password[a]-'a']=b;//letter at a is set to b;
	} else {
		for(size_t i=0;i<size;i++)if(a>position[i]&&position[i]>=b)position[i]++;
		position[password[a]-'a']=b;//letter at a is set to b;
    }
	set_pwd_from_pos();
}

void reverse_positions(size_t a,size_t b){
	for(size_t i=0;i<size;i++)if(position[i]>=a&&b>=position[i])position[i]=a+b-position[i];
	set_pwd_from_pos();
}

void rotate_based_on_position_of_letter(char a){
	if(position[a-'a']>=4)rotate_right(position[a-'a']+2);
	else rotate_right(position[a-'a']+1);
}

int main() {
std::cout<<password<<std::endl;/*
swap_position(4, 0);std::cout<<password<<0<<std::endl;
swap_letter('d', 'b');std::cout<<password<<1<<std::endl;
reverse_positions(0,4);std::cout<<password<<2<<std::endl;
rotate_left(1);std::cout<<password<<3<<std::endl;
move_position(1,4);std::cout<<password<<4<<std::endl;
move_position(3, 0);std::cout<<password<<5<<std::endl;
rotate_based_on_position_of_letter('b');std::cout<<password<<6<<std::endl;
rotate_based_on_position_of_letter('d');std::cout<<password<<7<<std::endl;*//*
rotate_right(3);std::cout<<password<<std::endl;
swap_letter('b','a');std::cout<<password<<std::endl;
move_position(3,4);std::cout<<password<<std::endl;
swap_position(0,7);std::cout<<password<<std::endl;
swap_letter('f','h');std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('f');std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('b');std::cout<<password<<std::endl;
swap_position(3,0);std::cout<<password<<std::endl;
swap_position(6,1);std::cout<<password<<std::endl;
move_position(4,0);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('d');std::cout<<password<<std::endl;
swap_letter('d','h');std::cout<<password<<std::endl;
reverse_positions(5,6);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('h');std::cout<<password<<std::endl;
reverse_positions(4,5);std::cout<<password<<std::endl;
move_position(3,6);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('e');std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('c');std::cout<<password<<std::endl;
rotate_right(2);std::cout<<password<<std::endl;
reverse_positions(5,6);std::cout<<password<<std::endl;
rotate_right(3);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('b');std::cout<<password<<std::endl;
rotate_right(5);std::cout<<password<<std::endl;
swap_position(5,6);std::cout<<password<<std::endl;
move_position(6,4);std::cout<<password<<std::endl;
rotate_left(0);std::cout<<password<<std::endl;
swap_position(3,5);std::cout<<password<<std::endl;
move_position(4,7);std::cout<<password<<std::endl;
reverse_positions(0,7);std::cout<<password<<std::endl;
rotate_left(4);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('d');std::cout<<password<<std::endl;
rotate_left(3);std::cout<<password<<std::endl;
swap_position(0,7);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('e');std::cout<<password<<std::endl;
swap_letter('e','a');std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('c');std::cout<<password<<std::endl;
swap_position(3,2);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('d');std::cout<<password<<std::endl;
reverse_positions(2,4);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('g');std::cout<<password<<std::endl;
move_position(3,0);std::cout<<password<<std::endl;
move_position(3,5);std::cout<<password<<std::endl;
swap_letter('b','d');std::cout<<password<<std::endl;
reverse_positions(1,5);std::cout<<password<<std::endl;
reverse_positions(0,1);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('a');std::cout<<password<<std::endl;
reverse_positions(2,5);std::cout<<password<<std::endl;
swap_position(1,6);std::cout<<password<<std::endl;
swap_letter('f','e');std::cout<<password<<std::endl;
swap_position(5,1);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('a');std::cout<<password<<std::endl;
move_position(1,6);std::cout<<password<<std::endl;
swap_letter('e','d');std::cout<<password<<std::endl;
reverse_positions(4,7);std::cout<<password<<std::endl;
swap_position(7,5);std::cout<<password<<std::endl;
swap_letter('c','g');std::cout<<password<<std::endl;
swap_letter('e','g');std::cout<<password<<std::endl;
rotate_left(4);std::cout<<password<<std::endl;
swap_letter('c','a');std::cout<<password<<std::endl;
rotate_left(0);std::cout<<password<<std::endl;
swap_position(0,1);std::cout<<password<<std::endl;
reverse_positions(1,4);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('d');std::cout<<password<<std::endl;
swap_position(4,2);std::cout<<password<<std::endl;
rotate_right(0);std::cout<<password<<std::endl;
swap_position(1,0);std::cout<<password<<std::endl;
swap_letter('c','a');std::cout<<password<<std::endl;
swap_position(7,3);std::cout<<password<<std::endl;
swap_letter('a','f');std::cout<<password<<std::endl;
reverse_positions(3,7);std::cout<<password<<std::endl;
rotate_right(1);std::cout<<password<<std::endl;
swap_letter('h','c');std::cout<<password<<std::endl;
move_position(1,3);std::cout<<password<<std::endl;
swap_position(4,2);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('b');std::cout<<password<<std::endl;
reverse_positions(5,6);std::cout<<password<<std::endl;
move_position(5,3);std::cout<<password<<std::endl;
swap_letter('b','g');std::cout<<password<<std::endl;
rotate_right(6);std::cout<<password<<std::endl;
reverse_positions(6,7);std::cout<<password<<std::endl;
swap_position(2,5);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('e');std::cout<<password<<std::endl;
swap_position(1,7);std::cout<<password<<std::endl;
swap_position(1,5);std::cout<<password<<std::endl;
reverse_positions(2,7);std::cout<<password<<std::endl;
reverse_positions(5,7);std::cout<<password<<std::endl;
rotate_left(3);std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('b');std::cout<<password<<std::endl;*/
rotate_left(3);std::cout<<password<<std::endl;
swap_letter('e','c');std::cout<<password<<std::endl;
rotate_based_on_position_of_letter('a');std::cout<<password<<std::endl;
swap_letter('f','a');std::cout<<password<<std::endl;
swap_position(0,6);std::cout<<password<<std::endl;
swap_position(4,7);std::cout<<password<<std::endl;
reverse_positions(0,5);std::cout<<password<<std::endl;
reverse_positions(3,5);std::cout<<password<<std::endl;
swap_letter('d','e');std::cout<<password<<std::endl;
move_position(0,7);std::cout<<password<<std::endl;
move_position(1,3);std::cout<<password<<std::endl;
reverse_positions(4,7);std::cout<<password<<std::endl;

std::cout<<password<<std::endl;
return 0;
}
