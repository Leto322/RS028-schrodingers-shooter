#ifndef WEAPON_DEF
#define WEAPON_DEF

#include "item.h"

class Weapon : public Item{
public:
	Weapon(float x, float y, float angle, float pickupDistance, std::string icon);
	void Pickup(Player* picker) override;
	void Update(bool shoot);
	void UpdateTimers();
	virtual void fire();
    int GetAmmo() const;
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
	};
};
#endif
