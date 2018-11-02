#include "../header/items.h"
#include "../header/bullet.h"
#include <iostream>
#include <vector>

extern double phisycsUpdateInterval;
extern std::vector<Bullet*> bullets;

Weapon::Weapon(float x, float y, float angle){
    dmg = 1;
	ammo = 10;
	ammo_cap = 10;
	spread = 0.5;
	fire_delay= 0.2;
	reload_delay = 2;
	fire_timer = 0;
    reload_timer= 0;
    pos_x = x;
    pos_y = y;
    angle = angle;
};

//Function for firing a bullet, it sends the bullet its position and angle
void Weapon::fire(){
	if(fire_timer <= 0 && reload_timer <= 0 && this->ammo != 0){
		//std::cout << "Firin!" << std::endl;
		Bullet* firedBullet = new Bullet(pos_x, pos_y, angle);
		bullets.push_back(firedBullet);

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

//Function for updating position and angle at which weapon is being pointed, required for firing a bulet
void Weapon::SetPositionAndAngle(float x, float y, float angle){
	this->pos_x = x;
	this->pos_y = y;
	this->angle = angle;
}
