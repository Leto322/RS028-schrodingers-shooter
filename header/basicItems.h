#ifndef BASIC_ITEMS_DEF
#define BASIC_ITEMS_DEF

#include <math.h>
#include <Box2D/Box2D.h>
#include <vector>
#include "item.h"


class HealthPotion : public Item {
public:
	HealthPotion(float x, float y, int amount) :
		Item(x, y, 0.2, "healthPotion"), m_amount(amount) {}
	void Pickup(Player *picker) override {
		picker->IncreaseHealth(m_amount);
	}
private:
	int m_amount;
};

#endif
