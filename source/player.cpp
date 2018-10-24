#include "../header/player.h"
#include "../header/items.h"
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
    rotation = 90;
    equiped_weapon = new Weapon();
    
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);
    body = world->CreateBody(&bodyDef);
    
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
    fixtureDef.friction = 0.3f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);
};

void Player::Draw(){
    if (team){
        glColor3f(1, 0.3, 0.3);
    }
    else{
        glColor3f(0.3, 0.3, 1);
    }
    glPushMatrix();
    glTranslatef(body->GetPosition().x, body->GetPosition().y, 0);
    glutSolidSphere(r, 20, 20);
    glRotatef(rotation, 0, 0, 1);
    glTranslatef(r, 0, 0);
    glColor3f(0, 0, 0); 
    glutSolidSphere(r/4, 20, 20);
    glPopMatrix();
};

void Player::Update(){
    Move();
    //std::cout << "vertical " << input.vertical  << "horizontal " << input.horizontal << std::endl;
};

void Player::Move(){
    /*float rcos = cos(rotation*M_PI/180);
    float rsin = sin(rotation*M_PI/180);
    float vx = -rcos*input.vertical + rsin*input.horizontal;
    float vy = rsin*input.vertical - rcos*input.horizontal;*/
    float vx = input.horizontal;
    float vy = input.vertical;
    
    b2Vec2 vel(vx, vy);
    
    body->SetLinearVelocity(vel);
    //position.x += vx*speed;
    //position.y += vy*speed;

    //std::cout << "pos " << speed << " " << position.y<< std::endl;
};
