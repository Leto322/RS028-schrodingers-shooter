#ifndef ITEM_DEF
#define ITEM_DEF

#include <math.h>
#include <Box2D/Box2D.h>
#include <vector>

class Player;

class Item {
public:
	Item(float x, float y, float pickupDistance);
	Item();
	virtual void Pickup(Player *picker) = 0;
	virtual void Draw() = 0;
	bool IsColliding(Player *picker);
	b2Vec2 itemPosition;
	float pickupDistance;
};

class ItemPool {
public:
	ItemPool();
	void CheckPickups(Player *picker);
	void Add(Item *item);
	void Remove(Item *item);
	void Remove(int index);
	void DrawItems();
private:
	std::vector<Item*> m_items;

};

#endif