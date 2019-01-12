#include <iostream>
#include "../header/menuScene.h"
#include "../header/gameScene.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <Box2D/Box2D.h>
#include <AL/alut.h>
#include <chrono>
#include <string>
#include <map>

extern float windowWidth, windowHeight, aspectRatio;
extern std::map<std::string, int> textures;
extern std::map<std::string, int> sounds;
extern std::map<std::string, int> textures;
extern std::chrono::high_resolution_clock::time_point lastFrameTime;
std::map<std::string, bool> pressedButtons;
extern ALuint ambientSource[1];
extern bool GameOver;
bool menuActive;
bool creditsActive;
bool controlsActive;
bool resetGame;

enum scene {
	GAME,
	MENU,
	EASTER_EGG
};
extern enum scene currentScene;


void InitMenu() {
	alSourcePlay(ambientSource[0]);
		pressedButtons.insert( std::pair<std::string,bool>(std::string("play"),false) );
		pressedButtons.insert( std::pair<std::string,bool>(std::string("reset"),false) );
		pressedButtons.insert( std::pair<std::string,bool>(std::string("credits"),false) );
		pressedButtons.insert( std::pair<std::string,bool>(std::string("controls"),false) );
		pressedButtons.insert( std::pair<std::string,bool>(std::string("exit"),false) );
		pressedButtons.insert( std::pair<std::string,bool>(std::string("back"),false) );
    menuActive = true;
	resetGame = true;
    creditsActive = false;
    controlsActive = false;
}



void pressButton(int x, int y){
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
    float x1 = (x- windowWidth / 2)/windowWidth*2*w;
    float y1 = -(y- windowHeight / 2)/windowHeight*2*h;
    if(resetGame && x1>=-w/8 && x1 <= w/8 && y1 <= h/2+h/20 && y1 >= h/2-h/20 && menuActive){
        pressedButtons["play"] = true;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/2+h/20 && y1 >= h/2-h/20 && menuActive){
        pressedButtons["play"] = true;
    }
    else if(GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20+h/4 && y1 >= h/4-h/20+h/4 && menuActive){
        pressedButtons["reset"] = true;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20 && y1 >= h/4-h/20 && menuActive){
        pressedButtons["reset"] = true;
    }
    else if((GameOver||resetGame) && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20 && y1 >= h/4-h/20 && menuActive ){
        pressedButtons["controls"] = true;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20-h/4 && y1 >= h/4-h/20-h/4 && menuActive ){
        pressedButtons["controls"] = true;
    }
    else if((GameOver||resetGame) && x1>=-w/8 && x1 <= w/8 && y1 <= h/20  && y1 >= -h/20 && menuActive ){
        pressedButtons["credits"] = true;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/20-h/4  && y1 >= -h/20-h/4 && menuActive ){
        pressedButtons["credits"] = true;
    }
    else if((GameOver||resetGame) && x1>=-w/8 && x1 <= w/8 && y1 <= -h/4+h/20  && y1 >= -h/4-h/20 && menuActive){
        pressedButtons["exit"] = true;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= -h/4+h/20-h/4 && y1 >= -h/4-h/20-h/4 && menuActive){
        pressedButtons["exit"] = true;
    }
    else if(x1>=-w/8 && x1 <= w/8 && y1 <= -h/1.8+h/20  && y1 >= -h/1.8-h/20  && (creditsActive || controlsActive)){
        pressedButtons["back"] = true;
    }
    
}

void releaseButton(int x, int y){
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
    float x1 = (x- windowWidth / 2)/windowWidth*2*w;
    float y1 = -(y- windowHeight / 2)/windowHeight*2*h;

    if(resetGame && x1>=-w/8 && x1 <= w/8 && y1 <= h/2+h/20  && y1 >= h/2-h/20  && pressedButtons["play"] ){
        lastFrameTime = std::chrono::high_resolution_clock::now();
		resetGame = false;
        currentScene = GAME;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/2+h/20  && y1 >= h/2-h/20  && pressedButtons["play"] ){
        lastFrameTime = std::chrono::high_resolution_clock::now();
		resetGame = false;
        currentScene = GAME;
    }
    else if(GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20+h/4 && y1 >= h/4-h/20+h/4  && pressedButtons["reset"]){
	   Clean(false);
       	   InitGame();
	   resetGame = true;
    }
    else if(!resetGame && !GameOver  && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20  && y1 >= h/4-h/20  && pressedButtons["reset"]){
	   Clean(false);
       InitGame();
	   resetGame = true;
    }
    else if((resetGame || GameOver) && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20 && y1 >= h/4-h/20  && pressedButtons["controls"]){
       controlsActive = true;
       menuActive = false;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/4+h/20-h/4 && y1 >= h/4-h/20-h/4 && pressedButtons["controls"]){
       controlsActive = true;
       menuActive = false;
    }
    else if((resetGame || GameOver) && x1>=-w/8 && x1 <= w/8 && y1 <= h/20  && y1 >= -h/20 && pressedButtons["credits"]){
       creditsActive = true;
       menuActive = false;
    }
    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= h/20-h/4  && y1 >= -h/20-h/4 && pressedButtons["credits"]){
       creditsActive = true;
       menuActive = false;
    }
    else if((resetGame ||(!resetGame && GameOver))&& x1>=-w/8 && x1 <= w/8 && y1 <= -h/4+h/20  && y1 >= -h/4-h/20 && pressedButtons["exit"]){
		Clean(true);
		glutLeaveMainLoop();
    }

    else if(!resetGame && !GameOver && x1>=-w/8 && x1 <= w/8 && y1 <= -h/4+h/20-h/4  && y1 >= -h/4-h/20-h/4 && pressedButtons["exit"]){
		Clean(true);
		glutLeaveMainLoop();
    }
    else if(x1>=-w/8 && x1 <= w/8 && y1 <= -h/1.8+h/20 && y1 >= -h/1.8-h/20 && pressedButtons["back"] && creditsActive ){
       creditsActive = false;
       menuActive = true;
    }
    else if(x1>=-w/8 && x1 <= w/8 && y1 <= -h/1.8+h/20 && y1 >= -h/1.8-h/20 && pressedButtons["back"] && controlsActive ){
       controlsActive = false;
       menuActive = true;
    }

    pressedButtons["play"] = false;
	pressedButtons["reset"] = false;
    pressedButtons["controls"] = false;
    pressedButtons["credits"] = false;
    pressedButtons["exit"] = false;
    pressedButtons["back"] = false;
}


void on_mouse_pressed_released_menu(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			pressButton(x,y);
		else if (state == GLUT_UP)
			releaseButton(x,y);

		break;
	}

}

// void on_mouse_move_active_game(int x, int y) {
// 	on_mouse_move_game(x, y);
// }


// void on_mouse_move_game(int x, int y) {
// 	float dx = x - windowWidth / 2;
// 	float dy = y - windowHeight / 2;
//
// 	float angle = atan2(dy, dx);
// 	myPlayer->input.angle = -angle;
// }



// void on_timer_menu()
// {
//
// }


void on_keyboard_menu(unsigned char key, int x, int y)
{
	switch (key) {
        case 27:
            glutPositionWindow(5,30);
            glutReshapeWindow(1280, 720);
            break;
        case 'f':
            glutFullScreen();
            break;

	}
}

void DrawMenu() {
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
	glPushMatrix();

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textures["menu"]);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-w,-h, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w, -h, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w, h, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w, h, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glPushMatrix();

    //Play button
	
	if(resetGame || !GameOver){
		if(pressedButtons["play"])
			glColor4f(0.3,0.3,0.3,0.8);
		else
			glColor3f(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures["button"]);
		glNormal3f(0, 0, 1);
		glTranslatef(0, h/2, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-w/8,-h/20, 0);
			glTexCoord2f(1, 0);
			glVertex3f(w/8, -h/20, 0);
			glTexCoord2f(1, 1);
			glVertex3f(w/8, h/20, 0);
			glTexCoord2f(0, 1);
			glVertex3f(-w/8, h/20, 0);
		glEnd();
	}
	
	//Reset button
	if(GameOver || !resetGame){
		if(GameOver)
			glTranslatef(0, 3*h/4, 0);
		if(pressedButtons["reset"])
			glColor4f(0.3,0.3,0.3,0.8);
		else
			glColor3f(1,1,1);
		glBindTexture(GL_TEXTURE_2D, textures["button"]);
		glNormal3f(0, 0, 1);
		glTranslatef(0, -h/4, 0);
		glBegin(GL_QUADS);
			glTexCoord2f(0, 0);
			glVertex3f(-w/8,-h/20, 0);
			glTexCoord2f(1, 0);
			glVertex3f(w/8, -h/20, 0);
			glTexCoord2f(1, 1);
			glVertex3f(w/8, h/20, 0);
			glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
		glEnd();
	}


    //Controls button
    if(pressedButtons["controls"])
        glColor4f(0.3,0.3,0.3,0.8);
    else
        glColor3f(1,1,1);
    glTranslatef(0, -h/4, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
		glVertex3f(-w/8,-h/20, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w/8, -h/20, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w/8, h/20, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
	glEnd();

    //Credits button
    if(pressedButtons["credits"])
        glColor4f(0.3,0.3,0.3,0.8);
    else
        glColor3f(1,1,1);
    glTranslatef(0, -h/4, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
		glVertex3f(-w/8,-h/20, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w/8, -h/20, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w/8, h/20, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
	glEnd();

    //Exit button
    if(pressedButtons["exit"])
        glColor4f(0.3,0.3,0.3,0.8);
    else
        glColor3f(1,1,1);
    glTranslatef(0, -h/4, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
		glVertex3f(-w/8,-h/20, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w/8, -h/20, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w/8, h/20, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
	glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();
	


    unsigned char play[] = "Play";
	unsigned char reset[] = "Reset";
    unsigned char credits[] = "Credits";
    unsigned char controls[] = "Controls";
    unsigned char exit[] = "Exit";
    glPushMatrix();
	
	
	if(resetGame || !GameOver){
		glTranslatef(-w/30, h/2-h/80, 0);
		glColor3f(1,1,1);
		glRasterPos3f(0, 0, 0);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,play);
	}
	
	
	if(GameOver || !resetGame){
		if(GameOver)
			glTranslatef(-w/30, 3*h/4-h/80, 0);
		glTranslatef(-w/80, -h/4, 0);
		glRasterPos3f(0, 0, 0);
		glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,reset);
		glTranslatef(w/80, 0, 0);
	}

    glTranslatef(-w/30, -h/4, 0);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,controls);

    glTranslatef(w/60, -h/4, 0);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,credits);

    glTranslatef(w/60, -h/4, 0);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,exit);
    glPopMatrix();
}

void DrawControls(){
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
	glPushMatrix();

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textures["menu"]);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-w,-h, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w, -h, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w, h, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w, h, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);


    unsigned char controls[] = "w - Move up\n\na - Move left\n\ns - Move down\n\nd - Move right\n\nMouse left - Shoot\n\nf - Fullscreen\n\nESC in menu - Windowed mode\n\nESC in game- Main menu";


    glTranslatef(-w/7, h/1.3, 0);
    glColor3f(1,1,1);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,controls);
    glPopMatrix();


    glBindTexture(GL_TEXTURE_2D, textures["button"]);
    if(pressedButtons["back"])
        glColor4f(0.3,0.3,0.3,0.8);
    else
        glColor3f(1,1,1);
    glTranslatef(0, -h/1.8, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
		glVertex3f(-w/8,-h/20, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w/8, -h/20, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w/8, h/20, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
	glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glPushMatrix();
    unsigned char back[] = "Back";
    glTranslatef(-w/30, -h/60, 0);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,back);
    glPopMatrix();
}

void DrawCredits(){
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
	glPushMatrix();

	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textures["menu"]);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex3f(-w,-h, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w, -h, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w, h, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w, h, 0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, 0);


    unsigned char credits[] = "Branko Djakovic 41/2015\n\nMladen Krcmarevic 119/2015\n\nFilip Kristic 335/2015\n\nSounds from:\n-http://soundbible.com\n-https://www.zapsplat.com\n-www.bensound.com";


    glTranslatef(-w/5, h/2, 0);
    glColor3f(1,1,1);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,credits);
    glPopMatrix();

    glBindTexture(GL_TEXTURE_2D, textures["button"]);
    if(pressedButtons["back"])
        glColor4f(0.3,0.3,0.3,0.8);
    else
        glColor3f(1,1,1);
    glTranslatef(0, -h/1.8, 0);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
		glVertex3f(-w/8,-h/20, 0);
		glTexCoord2f(1, 0);
		glVertex3f(w/8, -h/20, 0);
		glTexCoord2f(1, 1);
		glVertex3f(w/8, h/20, 0);
		glTexCoord2f(0, 1);
		glVertex3f(-w/8, h/20, 0);
	glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
    glPopMatrix();

    glPushMatrix();
    unsigned char back[] = "Back";
    glTranslatef(-w/30, -h/60, 0);
    glRasterPos3f(0, 0, 0);
	glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24,back);
    glPopMatrix();
}

void on_display_menu(void)
{
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4,
		0, 0, 0,
		0, 1, 0);


    glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
    if(menuActive)
        DrawMenu();
    else if(controlsActive)
        DrawControls();
    else
        DrawCredits();
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
}
