#include "../header/bullet.h"
#include "../header/util.h"
#include "../header/particleSystem.h"
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

	fixtureDef.filter.groupIndex = -1;

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

void Bullet::StartSparkEffect() {
	std::cout << "stigao" << std::endl;
	b2Vec2 vel = body->GetLinearVelocity();
	vel.Normalize();
	b2Vec2 pos = body->GetPosition();
	vel = 0.5 * vel;
	Emitter* spark = new Emitter(pos, vel, b2Vec2(0, 0), 3, 0.2, "spark");
	spark->SetScale(0.04, 0.07);
	spark->SetSpeed(0.1, 0.3);
	spark->SetStartRotation(atan2(vel.y, vel.x));
	spark->Start();

	Emitter* smoke = new Emitter(pos, b2Vec2(0, 0), b2Vec2(0, 0), 10, 1, "smoke");
	smoke->SetScale(0.03, 0.06);
	smoke->SetSpeed(0.02, 0.04);
	smoke->SetRotation(1, 2);
	smoke->SetAlphaTween(TW_CUBIC);
	smoke->Start();
};

Bullet::~Bullet() {
	world->DestroyBody(this->body);
}


ClassID Bullet::getClassID(){return BULLET;}
