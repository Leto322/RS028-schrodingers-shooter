#ifndef ITEM_DEF
#define ITEM_DEF

#include <math.h>
#include <Box2D/Box2D.h>
#include <vector>

class Player;

class Item {
public:
	Item(float x, float y, float pickupDistance, std::string icon);
	virtual void Pickup(Player *picker) = 0;
	void Draw();
	std::string  Name();
	bool IsColliding(Player *picker);
	b2Vec2 itemPosition;
private:
	float pickupDistance;
	std::string icon;
};

class ItemPool {
public:
	ItemPool();
	~ItemPool();
	void CheckPickups(Player *picker);
	void Add(Item *item);
	void Remove(Item *item);
	void Remove(int index);
	void DrawItems();
	void SpawnRandom(b2Vec2 pos);
private:
	std::vector<Item*> m_items;

};

#endif
