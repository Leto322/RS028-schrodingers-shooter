#ifndef ITEMS_DEF
#define ITEMS_DEF

#include <ctime>

class Weapon {
public:
	Weapon();
	void Update(bool shoot);
	void fire();
	void reload();
	bool allowed_fire;
	
private:
	int dmg;
	double rate_of_fire;
	int ammo;
	int ammo_cap;
	int reload_time;
	float spread;
	clock_t last_shot;
};

#endif