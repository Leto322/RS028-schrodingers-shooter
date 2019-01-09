#include <iostream>
#include "../header/menuScene.h"
#include <GL/glut.h>
#include <Box2D/Box2D.h>
#include <AL/alut.h>
#include <map>

extern float windowWidth, windowHeight, aspectRatio;
extern GLuint textureIDs[];
extern std::map<std::string, int> sounds;
extern ALuint ambientSource[1];


void InitMenu() {
	alSourcePlay(ambientSource[0]);
}





// void on_mouse_pressed_released_game(int button, int state, int x, int y) {
// 	switch (button) {
// 	case GLUT_LEFT_BUTTON:
// 		if (state == GLUT_DOWN)
// 			myPlayer->input.shoot = true;
// 		else if (state == GLUT_UP)
// 			myPlayer->input.shoot = false;
// 		break;
// 	}
// 
// }

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

void DrawMenu() {
    float h = tan(30 * M_PI / 180) * 4;
	float w = h * aspectRatio;
	glPushMatrix();
	glColor3f(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
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

}



void on_display_menu(void)
{
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 4,
		0, 0, 0,
		0, 1, 0);


	DrawMenu();

}
