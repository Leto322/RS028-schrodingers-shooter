#include <iostream>
#include <GL/glut.h>
#include "../header/image.h"
#include "../header/player.h"
#include "../header/geometry.h"
#include "../header/weapon.h"
#include "../header/bullet.h"
#include "../header/gameScene.h"
#include "../header/collision.h"
#include <string>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <Box2D/Box2D.h>

extern float windowWidth, windowHeight;
extern GLuint textureNames[];

Player* players[2];
Player* myPlayer;
b2World* world;
MyContactListener* contactListener;
std::vector<Block> walls;
std::vector<Block> ground;
std::chrono::high_resolution_clock::time_point lastFrameTime;
double accumulator = 0;
double phisycsUpdateInterval = 0.02;
std::vector<Bullet*> bullets;
ItemPool itemPool;

void InitGame() {

	srand(clock());

	b2Vec2 gravity(0.0f, 0.0f);
	world = new b2World(gravity);

	contactListener = new MyContactListener();
	world->SetContactListener(contactListener);

	LoadWalls();

	myPlayer = new Player();
    myPlayer->SetBrain(new playerBrain(*myPlayer));
	players[0] = myPlayer;
	players[1] = new Player();
    players[1]->SetBrain(new botBrain(*players[1]));
	players[1]->team = !myPlayer->team;
	players[1]->body->SetTransform(b2Vec2(-1, 0), 1);

	itemPool = ItemPool();

	//Test for the items
	itemPool.Add(new Weapon(-2, 0, 0, 0.1));
	itemPool.Add(new Weapon(-4, 0, 0, 0.3));

	lastFrameTime = std::chrono::high_resolution_clock::now();
}

void on_keyboard_game(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		/* Zavrsava se program. */
		exit(0);
		break;
	case 'a':
		myPlayer->input.horizontal -= 1;
		break;
	case 'd':
		myPlayer->input.horizontal += 1;
		break;
	case 'w':
		myPlayer->input.vertical += 1;
		break;
	case 's':
		myPlayer->input.vertical -= 1;
		break;
	case 'r':
		myPlayer->equiped_weapon->reload();
		break;

	}
	//std::cout << "vertical " << myPlayer.input.vertical  << "horizontal " << myPlayer.input.horizontal << std::endl;
}

void keyboard_up_game(unsigned char key, int x, int y) {
	switch (key) {
	case 'a':
		myPlayer->input.horizontal += 1;
		break;
	case 'd':
		myPlayer->input.horizontal -= 1;
		break;
	case 'w':
		myPlayer->input.vertical -= 1;
		break;
	case 's':
		myPlayer->input.vertical += 1;
		break;

	}
}


void on_mouse_pressed_released_game(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			myPlayer->input.shoot = true;
		else if (state == GLUT_UP)
			myPlayer->input.shoot = false;
		break;
	}

}

void on_mouse_move_active_game(int x, int y) {
	on_mouse_move_game(x, y);
}


void on_mouse_move_game(int x, int y) {
	float dx = x - windowWidth / 2;
	float dy = y - windowHeight / 2;

	float angle = atan2(dy, dx);
	myPlayer->input.angle = -angle;
}



void on_timer_game()
{
	auto now = std::chrono::high_resolution_clock::now();;
	std::chrono::duration<double>  deltaTime = now - lastFrameTime;
	accumulator += deltaTime.count();
	lastFrameTime = now;
	//std::cout << "dt " << deltaTime.count() << std::endl;
	while (accumulator > phisycsUpdateInterval) {
		world->Step(phisycsUpdateInterval, 6, 2);

		for (int i = 0; i < 1; i++) {
			itemPool.CheckPickups(players[i]);
			players[i]->m_brain->Update();
			players[i]->equiped_weapon->Update(players[i]->input.shoot);
		}

		accumulator -= phisycsUpdateInterval;
	}

}

void DrawMap() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textureNames[0]);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-9, -9, 0);
		glTexCoord2f(20, 0);
		glVertex3f(9, -9, 0);
		glTexCoord2f(20, 20);
		glVertex3f(9, 9, 0);
		glTexCoord2f(0, 20);
		glVertex3f(-9, 9, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void DrawPlayers() {
	for (int i = 0; i < 2; i++) {
		players[i]->DrawShadow();
	}
	for (int i = 0; i < 2; i++) {
		players[i]->Draw();
	}
}

void DrawBullets() {
	for (int i = 0; i < bullets.size(); i++) {
        if( (abs(bullets[i]->body->GetLinearVelocity().x) <= 0.1 && abs(bullets[i]->body->GetLinearVelocity().y) <= 0.1)
					|| bullets[i]->toDelete == 1
				){
						world->DestroyBody(bullets[i]->body);
						bullets.erase(bullets.begin() + i);
            i--;
            continue;
        }
		bullets[i]->Draw();
	}
}

void on_display_game(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(myPlayer->body->GetPosition().x, myPlayer->body->GetPosition().y, 4,
		myPlayer->body->GetPosition().x, myPlayer->body->GetPosition().y, 0,
		0, 1, 0);

	itemPool.DrawItems();
	DrawWalls();
	DrawMap();
	DrawBullets();
	DrawPlayers();
}
