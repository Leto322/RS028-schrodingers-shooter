#include "../header/player.h"
#include "../header/items.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>

Player::Player(float x, float y, float r)
:   position({x, y}), r(r)
{};

Player::Player(){
    r = 0.15;
    speed = 0.03;
    health = 100;
    input.vertical = 0;
    input.horizontal = 0;
    input.shoot = false;
    rotation = 90;
    equiped_weapon = new Weapon();
};

void Player::Draw(){
    if (team){
        glColor3f(1, 0.3, 0.3);
    }
    else{
        glColor3f(0.3, 0.3, 1);
    }
    glPushMatrix();
    glTranslatef(position.x, position.y, 0);
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
    float rcos = cos(rotation*M_PI/180);
    float rsin = sin(rotation*M_PI/180);
    float vx = -rcos*input.vertical + rsin*input.horizontal;
    float vy = rsin*input.vertical - rcos*input.horizontal;
    
    position.x += vx*speed;
    position.y += vy*speed;

    //std::cout << "pos " << speed << " " << position.y<< std::endl;
};
