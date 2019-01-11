#ifndef WEAPON_DEF
#define WEAPON_DEF

#define NUM_OF_SOURCES_WEAP 3

#include "item.h"
#include <AL/alut.h>

class Weapon : public Item{
public:
	Weapon(float x, float y, float angle, float pickupDistance, std::string icon);
	void Pickup(Player* picker) override;
	void Update(bool shoot);
	void UpdateTimers();
	virtual void fire();
    int GetAmmo() const;
	int GetAmmoCap() const;
	void reload();
	void SetPositionAndAngle(float x, float y, float angle);
	bool allowed_fire;

protected:
	int dmg;
	int ammo;
	int ammo_cap;
	float spread;
	double fire_delay;
	double reload_delay;
	double fire_timer;
	double reload_timer;
	float pos_x;
	float pos_y;
	float angle;
	float recoilAmount;
	float bulletSIze;
	ALuint soundSource[NUM_OF_SOURCES_WEAP];
};

class Pistol : public Weapon {
public:
	Pistol(float x, float y, float angle) : Weapon(x, y, angle, 0.2, std::string("pistol")) {
		dmg = 5;
		ammo = 15;
		ammo_cap = 15;
		spread = 0.1;
		fire_delay = 0.25;
		reload_delay = 1.5;
		bulletSIze = 0.019;
	};
};

class Rifle : public Weapon {
public:
	Rifle(float x, float y, float angle) : Weapon(x, y, angle, 0.4, std::string("rifle")) {
		dmg = 10;
		ammo = 30;
		ammo_cap = 30;
		spread = 0.2;
		fire_delay = 0.15;
		reload_delay = 2;
		bulletSIze = 0.022;
	};
};

class Shotgun : public Weapon {
public:
	Shotgun(float x, float y, float angle, int palletNumber) : Weapon(x, y, angle, 0.4, std::string("shotgun")), palletNumber(palletNumber) {
		dmg = 8;
		ammo = 10;
		ammo_cap = 10;
		spread = 0.3;
		fire_delay = 0.6;
		reload_delay = 3;
		bulletSIze = 0.016;
	};
	int palletNumber;
	void fire() override;
};
#endif
