#include "../header/player.h"
#include "../header/weapon.h"
#include "../header/util.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <Box2D/Box2D.h>

extern b2World* world;

/*Player::Player(float x, float y, float r)
:   position({x, y}), r(r)
{
    Player();

};*/

Player::Player(){
    r = 0.15;
    speed = 0.03;
    health = 100;
    input.vertical = 0;
    input.horizontal = 0;
    input.shoot = false;
	input.angle= M_PI/2;
    equiped_weapon = new Weapon(0.0f, 0.0f, input.angle, 0.1);
    team = false;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.0f;
    bodyDef.position.Set(0.0f, 0.0f);
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);

    // Define another box shape for our dynamic body.
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = r;
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.01f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
};

void Player::SetBrain(Brain* brain){
    m_brain = brain;
}

void Player::Draw(){
    glPushMatrix();
		if (team){
			glColor3f(1, 0.3, 0.3);
		}
		else{
			glColor3f(0.3, 0.3, 1);
		}
		glTranslatef(body->GetPosition().x, body->GetPosition().y, r);
		glutSolidSphere(r, 15, 5);
		glRotatef(input.angle*180/M_PI, 0, 0, 1);
		glTranslatef(r, 0, 0);
		glColor3f(0, 0, 0);
		glutSolidSphere(r/4, 10, 2);
    glPopMatrix();
};

void Player::DrawShadow() {
	//DRAWING PLAYER SHADOW
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(body->GetPosition().x + r / 2, body->GetPosition().y - r / 2, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(1, 1.5, 0.1);
	glColor4f(0, 0, 0, 0.2);
	glutSolidSphere(r, 20, 2);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glPopMatrix();
}

Brain::Brain(Player& player)
    : m_player(&player) {}
    
    
playerBrain::playerBrain(Player& player)
    : Brain(player) {}
    
void playerBrain::Update(){
    float vx = Brain::m_player->input.horizontal;
    float vy = Brain::m_player->input.vertical;
    
    b2Vec2 vel(vx, vy);
    
    Brain::m_player->body->SetLinearVelocity(vel);
    
    vx = cos(Brain::m_player->input.angle);
    vy =  sin(Brain::m_player->input.angle);
    float n = 0.3;
    Brain::m_player->equiped_weapon->SetPositionAndAngle(Brain::m_player->body->GetPosition().x + vx*n, Brain::m_player->body->GetPosition().y + vy*n, Brain::m_player->input.angle);
}

botBrain::botBrain(Player& player)
    : Brain(player) {}
    
void botBrain::Update(){
    float vx = Brain::m_player->input.horizontal;
    float vy = Brain::m_player->input.vertical;
    
    b2Vec2 vel(vx, vy);
    
    Brain::m_player->body->SetLinearVelocity(vel);
    
    vx = cos(Brain::m_player->input.angle);
    vy =  sin(Brain::m_player->input.angle);
    float n = 0.15;
    Brain::m_player->equiped_weapon->SetPositionAndAngle(Brain::m_player->body->GetPosition().x + vx*n, Brain::m_player->body->GetPosition().y + vy*n, Brain::m_player->input.angle);
}
void Player::die(){
  std::cout << "Player is dead!" << std::endl;
}

void Player::takeDmg(int dmg){
  health -= dmg;
  if(health <= 0)
    die();
}

ClassID Player::getClassID() {return PLAYER;}
