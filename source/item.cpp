#include "../header/item.h"
#include "../header/player.h"
#include <vector>
#include <GL/glut.h>


Item::Item(float x, float y, float pickupDistance) {
	itemPosition.Set(x, y);
	this->pickupDistance = pickupDistance;
}

Item::Item() {
	itemPosition.Set(0, 0);
	pickupDistance = 0.1;
}

bool Item::IsColliding(Player *picker){
	b2Vec2 playerPos = picker->body->GetPosition();
	if (fabs(playerPos.x - itemPosition.x) < pickupDistance && fabs(playerPos.y - itemPosition.y) < pickupDistance) {
		return true;
	}
	return false;
};

ItemPool::ItemPool() {
	m_items = std::vector<Item*>();
}

void ItemPool::CheckPickups(Player *picker) {
	int n = m_items.size();
	for (int i = 0; i < n; i++) {
		if (m_items[i]->IsColliding(picker)) {
			Remove(i);
			m_items[i]->Pickup(picker);
		}
	}
}


void ItemPool::Add(Item *item) {
	m_items.push_back(item);
}


void ItemPool::Remove(Item *item) {
	int n = m_items.size();
	for (int i = 0; i < n; i++) {
		if (m_items[i] == item) {
			m_items.erase(m_items.begin() + i);
			return;
		}
	}
}

void ItemPool::Remove(int index) {
	m_items.erase(m_items.begin() + index);
}

void ItemPool::DrawItems() {
	for (Item* item : m_items) {
		item->Draw();
	}
}
