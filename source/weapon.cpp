#include "../header/weapon.h"
#include "../header/bullet.h"
#include "../header/player.h"
#include "../header/util.h"
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

	alGenSources(NUM_OF_SOURCES_WEAP, soundSource);
	alSourcei(soundSource[0], AL_BUFFER, sounds[icon]);

	if(icon == "shotgun"){
		alSourcei(soundSource[1], AL_BUFFER, sounds[std::string("reloadShotgun")]);
		alSourcef(soundSource[0], AL_GAIN, 0.6);
	}
	else{
		alSourcei(soundSource[1], AL_BUFFER, sounds[std::string("reload")]);
		alSourcef(soundSource[0], AL_GAIN, 0.15);
	}

	alSourcei(soundSource[2], AL_BUFFER, sounds[std::string("pickup")]);
	alSourcef(soundSource[0], AL_PITCH, 1);
};

//Function for firing a bullet, it sends the bullet its position and angle
void Weapon::fire(){
	if(fire_timer <= 0 && reload_timer <= 0 && this->ammo != 0){
		//Calculating new angle from random spread
		float firing_angle = angle + spread*randomNumber(-1,1)*recoilAmount;
		Bullet* firedBullet = new Bullet(pos_x, pos_y, firing_angle, dmg, bulletSIze);
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

		alSource3f(soundSource[0], AL_POSITION, pos_x, pos_y, 0.2);
		alSource3f(soundSource[1], AL_POSITION, pos_x, pos_y, 0.2);
		alSource3f(soundSource[2], AL_POSITION, pos_x, pos_y, 0.2);

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

int Weapon::GetAmmoCap() const{
	return ammo_cap;
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
	alSourcePlay(soundSource[2]);
}

void Shotgun::fire() {
	if (fire_timer <= 0 && reload_timer <= 0 && this->ammo != 0) {
		//Calculating new angle from random spread
			std::cout << "PalletNum " << palletNumber << std::endl;
		for (int i = 0; i < palletNumber; i++)
		{
			float rand = randomNumber(-1, 1);
			std::cout << "rand " << rand << std::endl;
			float firing_angle = angle + spread * rand * (i+1)/ (float)palletNumber;
			Bullet* firedBullet = new Bullet(pos_x, pos_y, firing_angle, dmg, bulletSIze);
			//Adding bullet to the list of fired bullets
			bullets.push_back(firedBullet);
		}

		alSourcePlay(soundSource[0]);

		this->ammo--;
		fire_timer = fire_delay;
	}
}
