#include "../header/item.h"
#include "../header/player.h"
#include "../header/basicItems.h"
#include <vector>
#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <map>

extern std::map<std::string, int> textures;
extern int updateCount;


Item::Item(float x, float y, float pickupDistance, std::string icon): icon(icon), pickupDistance(pickupDistance) {
	itemPosition.Set(x, y);
}

bool Item::IsColliding(Player *picker){
	b2Vec2 playerPos = picker->body->GetPosition();
	if (fabs(playerPos.x - itemPosition.x) < pickupDistance && fabs(playerPos.y - itemPosition.y) < pickupDistance) {
		return true;
	}
	return false;
};

void Item::Draw() {
	glColor4f(1, 1, 1, 1);
	
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glPushMatrix();
	glNormal3f(0, 0, 1);
	glTranslatef(itemPosition.x, itemPosition.y, 0.3);

	glBindTexture(GL_TEXTURE_2D, textures[icon]);
	float animationScale = 0.05*sin(updateCount / 20.0);
	glScalef(pickupDistance+animationScale, pickupDistance+animationScale, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(-1, -1, 0);
	glTexCoord2f(1, 0);
	glVertex3f(1, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(1, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-1, 1, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

};

std::string Item::Name() {
	return icon;
};

ItemPool::ItemPool() {
	m_items = std::vector<Item*>();
}

void ItemPool::CheckPickups(Player *picker) {
	int n = m_items.size();
	for (int i = 0; i < n; i++) {
		if (m_items[i]->IsColliding(picker)) {
			m_items[i]->Pickup(picker);
			Remove(i);
		}
	}
}


void ItemPool::Add(Item *item) {
	m_items.push_back(item);
}

void ItemPool::SpawnRandom(b2Vec2 pos) {
	Item* item = new HealthPotion(pos.x, pos.y, 20);
	Add(item);
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
