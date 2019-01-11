#include "../header/bullet.h"
#include "../header/util.h"
#include <math.h>
#include <GL/glut.h>
#include <Box2D/Box2D.h>

extern b2World* world;

/*Bullet constructor: takes a point from which a bullet is being fired and a trajectory angle*/
Bullet::Bullet(float x, float y, float angle, int dmg, float bulletSize){
	r = bulletSize;
	speed = 0.01;
	m_dmg = dmg;
	toDelete = 0;

	/*Creating Bullet Body*/
	b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
	bodyDef.linearDamping = 0;
    bodyDef.position.Set(x, y);
    body = world->CreateBody(&bodyDef);
		body->SetUserData(this);

    b2CircleShape cShape;
    cShape.m_p.Set(0, 0); //position, relative to body position
    cShape.m_radius = r;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &cShape;

    fixtureDef.density = 1.0f;

    fixtureDef.friction = 0;

    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    //When bullet is created its being fired so we set it a linear impulse
    float vx = speed*cos(angle);
    float vy =  speed*sin(angle);

    body->ApplyLinearImpulse(b2Vec2(vx,vy), body->GetWorldCenter(), true);

};

void Bullet::Draw(){

    glColor3f(1, 1, 1);

    glPushMatrix();
    glTranslatef(body->GetPosition().x, body->GetPosition().y, r);
    glutSolidSphere(r, 20, 20);
    glPopMatrix();
};

Bullet::~Bullet() {
	world->DestroyBody(this->body);
}


ClassID Bullet::getClassID(){return BULLET;}
