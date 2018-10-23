#include "../header/items.h"
#include <iostream>
#include <ctime>


Weapon::Weapon(){
    dmg = 1;
	rate_of_fire = 0.01;
	ammo = 10;
	ammo_cap = 10;
	reload_time = 2;
	spread = 0.5;
	last_shot = std::clock();
};

void Weapon::fire(){
	double delta_time;
	delta_time = (std::clock() - this->last_shot)*1.0 / CLOCKS_PER_SEC;
	//std::cout << delta_time << std::endl;

	if(delta_time > rate_of_fire && this->ammo != 0){
		std::cout << "Firin!" << std::endl;
		this->ammo--;
		last_shot = std::clock();
	}
}

void Weapon::reload(){
	//std::cout << ammo << std::endl;
	if(ammo < ammo_cap){
		std::cout << "Reloadin!" << std::endl;
		this->ammo = this->ammo_cap;
	}
}

void Weapon::Update(bool shoot){
	if(shoot){
		this->fire();
	}
}