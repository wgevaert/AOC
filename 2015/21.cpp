#include <iostream>

struct item{
    unsigned cost,damage,armor;
    item() {};
    item(unsigned _cost,unsigned _damage,unsigned _armor) {cost=_cost;damage=_damage;armor=_armor;};
};

const item Weapons[5]={
item( 8,4,0),
item(10,5,0),
item(25,6,0),
item(40,7,0),
item(74,8,0)
}, Armor[6]={
item(13,0,1),
item(31,0,2),
item(53,0,3),
item(75,0,4),
item(102,0,5),
item(0,0,0)
}, Rings[8]={
item(25,1,0),
item(50,2,0),
item(100,3,0),
item(20,0,1),
item(40,0,2),
item(80,0,3),
item(0,0,0),
item(0,0,0)
};

const unsigned my_base_hp=100,my_base_atk=0,my_base_def=0,monster_atk=8,monster_hp=100,monster_def=2;

int main(){
    unsigned min_cost=-1,max_cost=0;
    for(int weapon=0;weapon<5;weapon++)for(int armor=0;armor<6;armor++)for(int ring_0=0;ring_0<7;ring_0++)for(int ring_1=ring_0+1;ring_1<8;ring_1++){
        unsigned cost = Weapons[weapon].cost+Armor[armor].cost+Rings[ring_0].cost+Rings[ring_1].cost;

        unsigned my_damage = my_base_atk+Weapons[weapon].damage+Rings[ring_0].damage+Rings[ring_1].damage;
        if(my_damage<=monster_def)my_damage=1;else my_damage-=monster_def;

        unsigned my_def = my_base_def+Armor[armor].armor+Rings[ring_0].armor+Rings[ring_1].armor;
        unsigned monster_damage = monster_atk;
        if(monster_damage<=my_def)monster_damage=1;else monster_damage-=my_def;

        unsigned steps = (monster_hp-1)/my_damage;//He should not die yet;
        //std::cout<<"after spending "<<cost<<" and fighting "<<steps<<" steps the monster has "<<monster_hp-steps*my_damage<<" hp and I have "<<static_cast<int>(my_base_hp)-static_cast<int>(steps*monster_damage)<<" hp"<<std::endl;
        if(my_base_hp<=steps*monster_damage && cost>max_cost)max_cost=cost;
        if(my_base_hp>steps*monster_damage && cost<min_cost)min_cost=cost;
    }
    std::cout<<min_cost<<' '<<max_cost<<std::endl;
    return 0;
}
