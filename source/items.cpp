#include "../header/items.h"
#include <iostream>

extern double phisycsUpdateInterval;

Weapon::Weapon(){
    dmg = 1;
	ammo = 10;
	ammo_cap = 10;
	spread = 0.5;
	fire_delay= 0.2;
	reload_delay = 2;
	fire_timer = 0;
    reload_timer= 0;
};

void Weapon::fire(){
	if(fire_timer <= 0 && reload_timer <= 0 && this->ammo != 0){
		std::cout << "Firin!" << std::endl;
		this->ammo--;
		fire_timer = fire_delay;
	}
}

void Weapon::reload(){
	//std::cout << ammo << std::endl;
	if(ammo < ammo_cap){
		std::cout << "Reloadin!" << std::endl;
		this->ammo = this->ammo_cap;
	}
	if (reload_timer <= 0){
        reload_timer = reload_delay;
    }
}

void Weapon::Update(bool shoot){
    UpdateTimers();
	if(shoot){
		this->fire();
	}
}

void Weapon::UpdateTimers(){
    fire_timer -= phisycsUpdateInterval;
    reload_timer -= phisycsUpdateInterval;
}
