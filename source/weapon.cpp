#include "../header/weapon.h"
#include "../header/bullet.h"
#include "../header/player.h"
#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>


const float RE_AMM_MAX = 1, RE_AMM_MIN = 0, RE_AMM_UP = 0.15, RE_AMM_DOWN = 0.005;

extern double phisycsUpdateInterval;
extern std::vector<Bullet*> bullets;
extern std::map<std::string, int> sounds;

float randomNumber(float start, float end){
	return ( float(rand())/float(RAND_MAX) )*(end - start) + start;
}

Weapon::Weapon(float x, float y, float angle, float pickupDistance, std::string icon) : Item(x, y, pickupDistance, icon){
	std::cout << "Weapon created " << icon << std::endl;
	dmg = 10;
	ammo = 30;
	ammo_cap = 30;
	spread = 0.15;
	fire_delay= 0.15;
	reload_delay = 2;
	fire_timer = 0;
	reload_timer= 0;
	pos_x = x;
	pos_y = y;
	angle = angle;
	recoilAmount = 0;

	alGenSources(2, soundSource);
	alSourcei(soundSource[0], AL_BUFFER, sounds[icon]);
	alSourcei(soundSource[1], AL_BUFFER, sounds["reload"]);
	alSourcef(soundSource[0], AL_GAIN, 0.1);
	alSourcef(soundSource[0], AL_PITCH, 1);
};

//Function for firing a bullet, it sends the bullet its position and angle
void Weapon::fire(){
	if(fire_timer <= 0 && reload_timer <= 0 && this->ammo != 0){
		//Calculating new angle from random spread
		float firing_angle = angle + spread*randomNumber(-1,1)*recoilAmount;
		Bullet* firedBullet = new Bullet(pos_x, pos_y, firing_angle, dmg);
		//Adding bullet to the list of fired bullets
		bullets.push_back(firedBullet);

		alSourcePlay(soundSource[0]);

		this->ammo--;
		fire_timer = fire_delay;
		if(recoilAmount < RE_AMM_MAX)
			recoilAmount+=RE_AMM_UP;
		else if(recoilAmount > RE_AMM_MAX)
			recoilAmount = RE_AMM_MAX;
	}
}

void Weapon::reload(){
	//std::cout << ammo << std::endl;
	if(ammo < ammo_cap){
		std::cout << "Reloadin!" << std::endl;
		alSourcePlay(soundSource[1]);
		this->ammo = this->ammo_cap;
	}
	if (reload_timer <= 0){
        reload_timer = reload_delay;
    }
}

void Weapon::Update(bool shoot){
    UpdateTimers();

		alSource3f(soundSource[0], AL_POSITION, pos_x, pos_y, 0);
		alSource3f(soundSource[1], AL_POSITION, pos_x, pos_y, 0);

	if(shoot){
		this->fire();
	}
	if(recoilAmount > RE_AMM_MIN)
		recoilAmount -= RE_AMM_DOWN;
	else if(recoilAmount < RE_AMM_MIN)
		recoilAmount = RE_AMM_MIN;
}

int Weapon::GetAmmo() const{
    return ammo;
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

void Weapon::Pickup(Player* picker) {
	std::cout << "Weapon picked up " << this->Name() << std::endl;
	picker->SwapWeapon(this);
}
