#ifndef BULLET_DEF
#define BULLET_DEF

#include "../header/util.h"
#include <Box2D/Box2D.h>

class Bullet : public Colider {
public:
	Bullet(float x, float y, float angle, int dmg, float bulletSize);
	~Bullet();
	void Draw();
	void StartSparkEffect();
	virtual ClassID getClassID();

	b2Body* body;
	int m_dmg;
	int toDelete;

private:
	float r;
	float speed;
};

#endif
