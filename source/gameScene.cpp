#include <iostream>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "../header/image.h"
#include "../header/player.h"
#include "../header/geometry.h"
#include "../header/weapon.h"
#include "../header/bullet.h"
#include "../header/gameScene.h"
#include "../header/collision.h"
#include "../header/basicItems.h"
#include "../header/enemySpawner.h"
#include "../header/particleSystem.h"
#include "../header/util.h"
#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <Box2D/Box2D.h>
#include <AL/alut.h>

extern float windowWidth, windowHeight, aspectRatio;
extern GLuint textureIDs[];
extern std::map<std::string, int> sounds;
extern std::map<std::string, int> textures;

int updateCount;

Player* myPlayer;
b2World* world;
MyContactListener* contactListener;
std::vector<Block> walls;
std::vector<Block> ground;
std::chrono::high_resolution_clock::time_point lastFrameTime;
double accumulator = 0;
double phisycsUpdateInterval = 0.02;
std::vector<Bullet*> bullets;
std::vector<Grenade*> thrownGrenades;
std::vector<Player*> players;
std::vector<b2Vec2> spawnPositions;
std::vector<AudioWrapper*> audioWrappers;
ItemPool itemPool;
EnemySpawner* enemySpawner;
ParticleSystem* particleSystem;
ALuint ambientSource[1];
char text[100];


enum scene {
	GAME,
	MENU,
	EASTER_EGG
};
extern enum scene currentScene;

void InitGame() {
	updateCount = 0;

	alGenSources(1, ambientSource);
	//CREDITS:  www.bensound.com
	alSourcei(ambientSource[0], AL_BUFFER, sounds["music"]);
	alSourcef(ambientSource[0], AL_GAIN, 0.1);
	alSourcef(ambientSource[0], AL_PITCH, 1);
	alSourcei(ambientSource[0], AL_LOOPING, AL_TRUE);

	srand(clock());

	b2Vec2 gravity(0.0f, 0.0f);
	world = new b2World(gravity);

	contactListener = new MyContactListener();
	world->SetContactListener(contactListener);

	LoadWalls();


	myPlayer = new Player();
	myPlayer->SetBrain(new playerBrain(*myPlayer));
	myPlayer->SetMaxHealth(200);
	myPlayer->SetMaxArmor(200);
	myPlayer->body->SetTransform(b2Vec2(0, 0), 1);
	myPlayer->Revive();
	players.push_back(myPlayer);

	for (int i = 1; i < 30; i++) {
		players.push_back(new Player());
		players[i]->body->SetTransform(b2Vec2(-100, 0), 0);
		players[i]->SetBrain(new botBrain(*players[i]));
		players[i]->team = !myPlayer->team;
	}


	//spawnPositions.push_back(b2Vec2(0, 3));

	enemySpawner = new EnemySpawner(players, spawnPositions);
	particleSystem = new ParticleSystem();

	//b2Vec2 pos, b2Vec2 force, int particleCount, float lifespan, std::string texture)
	//players[1]->input.shoot = true;

	//Test for the items
	itemPool.Add(new Rifle(-2, 0, 0));
	itemPool.Add(new Shotgun(-3, 0, 0, 4));
	itemPool.Add(new HealthPotion(-4, 0, 20));
	itemPool.Add(new Armor(-4.5, 0));

	alSourcePlay(ambientSource[0]);

	lastFrameTime = std::chrono::high_resolution_clock::now();
}

void on_keyboard_game(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		/* Zavrsava se program. */
		currentScene = MENU;
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
	case 'g':{
		if(myPlayer->alive)
			myPlayer->throwGrenade();
		break;
	}
    case 'f':
        glutFullScreen();
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
	case 'g':{
		if(myPlayer->alive && myPlayer->grenades != 0){
			thrownGrenades[thrownGrenades.size() - 1]->throwMe(myPlayer->input.angle, myPlayer->input.cursorDist);
			alSourcePlay(myPlayer->soundSource[3]);
			myPlayer->grenades--;
		}
		break;
		}
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

	myPlayer->input.cursorDist = sqrt(dx*dx + dy*dy)/windowWidth;

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
		updateCount++;
		world->Step(phisycsUpdateInterval, 6, 2);
        BotMoves();
		for (int i = 0; i < players.size(); i++) {
			if (players[i]->deathFlag) {
				players[i]->deathFlag = false;
				itemPool.SpawnRandom(players[i]->body->GetPosition());
				players[i]->die();
			}
			if (!players[i]->alive) {
				continue;
			}
			itemPool.CheckPickups(players[i]);
			players[i]->m_brain->Update();
			if(i == 0){
				alListener3f(AL_POSITION, players[i]->body->GetPosition().x, players[i]->body->GetPosition().y, 0);
			}
			players[i]->equiped_weapon->Update(players[i]->input.shoot);

		}

		enemySpawner->Update();
		particleSystem->Update();

		for(int i = 0; i < thrownGrenades.size(); i++){
			if(thrownGrenades[i]->toDelete){
				Grenade* tmp = thrownGrenades[i];
				thrownGrenades.erase(thrownGrenades.begin() + i);
				i--;
				delete tmp;
				continue;
			}

			float Gn = 0.18;
			float Gvx = Gn*cos(myPlayer->input.angle);
	    float Gvy = Gn*sin(myPlayer->input.angle);
			thrownGrenades[i]->Update(myPlayer->body->GetPosition().x+Gvx, myPlayer->body->GetPosition().y+Gvy);
		}

		for(int i = 0; i < audioWrappers.size(); i++){
			if(!(audioWrappers[i]->isPlaying()) && audioWrappers[i]->toDelete){
				AudioWrapper* tmp = audioWrappers[i];
				audioWrappers.erase(audioWrappers.begin() + i);
				i--;
				delete tmp;
			}
		}

		alSource3f(ambientSource[0], AL_POSITION, myPlayer->body->GetPosition().x, myPlayer->body->GetPosition().y, 0);

		if(!myPlayer->alive)
			alSourceStop(ambientSource[0]);

		accumulator -= phisycsUpdateInterval;
	}

}

void DrawMap() {
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
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
    glPushMatrix();
    glColor3f(0,0,0);

    for(int i = 0; i < 40; ++i){
        glBegin(GL_LINES);
//         std::cout << i*18.0/40-9 << std::endl;
        glVertex3f(-9,i*18.0/40-9,0.05);
        glVertex3f(9,i*18.0/40-9,0.05);
        glEnd();
    }

    for(int i = 0; i < 40; ++i){
        glBegin(GL_LINES);
//         std::cout << i*18.0/40-9 << std::endl;
        glVertex3f(i*18.0/40-9,9,0.05);
        glVertex3f(i*18.0/40-9,-9,0.05);
        glEnd();
    }
	glPopMatrix();
}

void DrawPlayers() {
	for (int i = 0; i < players.size(); i++) {
		if (!players[i]->alive)
			continue;

		players[i]->DrawShadow();
	}
	for (int i = 0; i < players.size(); i++) {
		if (!players[i]->alive)
			continue;

		players[i]->Draw();
	}
}

void DrawHUDPlayers() {
	for (int i = 0; i < players.size(); i++) {
		if (!players[i]->alive)
			continue;

		if (players[i]->team) {
			glColor3f(1, 0, 0);
		}
		else {
			glColor3f(0, 0, 1);
		}

		glPushMatrix();
		b2Vec2 pos = players[i]->body->GetPosition();

		glTranslatef(pos.x, pos.y, 0);
		glScalef(1, 1, 0.1);
		glutSolidCube(0.5);

		glPopMatrix();

	}
}

void DrawBullets() {
	for (int i = 0; i < bullets.size(); i++) {
        if( (abs(bullets[i]->body->GetLinearVelocity().x) <= 0.1 && abs(bullets[i]->body->GetLinearVelocity().y) <= 0.1) || bullets[i]->toDelete == 1){
			Bullet* tmp = bullets[i];
			bullets.erase(bullets.begin() + i);
			delete tmp;

            i--;
            continue;
        }
		bullets[i]->Draw();
	}
}

void DrawGrenades() {
	for (int i = 0; i < thrownGrenades.size(); i++) {
		if(thrownGrenades[i]->thrown)
			thrownGrenades[i]->Draw();
	}
}

void DrawHUDMap() {
	glPushMatrix();
	glColor3f(0.8, 0.65, 0);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(-9, -9, 0);
	glVertex3f(9, -9, 0);
	glVertex3f(9, 9, 0);
	glVertex3f(-9, 9, 0);
	glEnd();
	glPopMatrix();
}

void DrawHUDBar() {
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;

	glPushMatrix();
	glTranslatef(myPlayer->body->GetPosition().x,myPlayer->body->GetPosition().y-h+h/10,0);
	glBindTexture(GL_TEXTURE_2D, textures["hudbar"]);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-w,-h/10, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w, -h/10, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w, h/10, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w, h/10, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);


	glTranslatef(-w+w/10,0,0);
	DrawWepon();


	int numberOfAliveBots = 0;
	for (int i = 1; i < players.size(); i++) {
		if (players[i]->alive)
			numberOfAliveBots++;
	}
	int botsLeft = enemySpawner->GetEnemiesInWave() - (enemySpawner->GetEnemiesSpawned() - numberOfAliveBots);
	sprintf(text, "Ammo: %d/%d\tWave: %d\tLeft: %d", myPlayer->equiped_weapon->GetAmmo(), myPlayer->equiped_weapon->GetAmmoCap(), enemySpawner->GetCurrentWave(), botsLeft);
	glTranslatef(w/10, -h/40, 0);
	glColor3f(1,1,1);
    glRasterPos3f(0, 0, 0);
	for(int i = 0; i < strlen(text); i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,text[i]);
	}
    memset(text, 0, sizeof text);





	glPopMatrix();
}

void DrawWepon(){
	glColor4f(1, 1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glNormal3f(0, 0, 1);
	glBindTexture(GL_TEXTURE_2D, textures[myPlayer->equiped_weapon->Name()]);
	float animationScale = 0.25;
	glScalef(animationScale*1.5,animationScale, 1);
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
	glDisable(GL_BLEND);
}

void on_display_game(void)
{
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(myPlayer->body->GetPosition().x, myPlayer->body->GetPosition().y, 4,
		myPlayer->body->GetPosition().x, myPlayer->body->GetPosition().y, 0,
		0, 1, 0);


	DrawMap();
	DrawBullets();
	DrawPlayers();
	DrawGrenades();
	DrawWalls();
	itemPool.DrawItems();
	particleSystem->Draw();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	DrawHUDBar();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	//MiniMap
	glViewport(-120 + windowWidth / 6 + windowWidth - windowWidth/3, 0, windowWidth / 3, windowHeight / 3);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 18,
		0, 0, 0,
		0, 1, 0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	DrawHUDMap();
	DrawWalls();
	DrawHUDPlayers();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
