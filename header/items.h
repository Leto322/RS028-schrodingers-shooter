#ifndef ITEMS_DEF
#define ITEMS_DEF

class Weapon {
public:
	Weapon();
	void Update(bool shoot);
	void UpdateTimers();
	void fire();
	void reload();
	bool allowed_fire;
	
private:
	int dmg;
	int ammo;
	int ammo_cap;
	float spread;
	double fire_delay;
	double reload_delay;
	double fire_timer;
	double reload_timer;
};

#endif
