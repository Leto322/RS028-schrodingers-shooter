#include <iostream>
#include <GL/glut.h>
#include "../header/image.h"
#include "../header/player.h"
#include "../header/geometry.h"
#include "../header/items.h"
#include <string>
#include <vector>
#include <Box2D/Box2D.h>

std::vector<std::string> textureLocations = {
	"textures/Map.bmp"
	};

#define TIMER_ID 0
#define TIMER_INTERVAL 15

static GLuint textureNames[1];

static int animation_ongoing;
static void on_keyboard(unsigned char key, int x, int y);
static void keyboard_up(unsigned char key, int x, int y);
static void keyboard_down(unsigned char key, int x, int y);
static void on_mouse_move(int x, int y);
static void on_mouse_move_active(int x, int y);
static void on_mouse_pressed_released(int button, int state, int x, int y);
static void on_timer(int value);
static void on_display(void);
static void on_reshape(int width, int height);

void LoadTextures(){
	glGenTextures(textureLocations.size(), textureNames);
	Image* image;
	image = image_init(0, 0);
	
	for (int i=0; i<textureLocations.size(); i++){
		char *cstr = &textureLocations[i][0u];
		image_read(image, cstr);
		glBindTexture(GL_TEXTURE_2D, textureNames[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
		delete [] cstr;
	}

	image_done(image);
}

Player* players[1];
Player* myPlayer;


int main(int argc, char **argv)
{
	/* Inicijalizuje se GLUT. */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	/* Kreira se prozor. */
	glutInitWindowSize(1280, 720);
	//glutInitWindowPosition(0, 0);
	glutCreateWindow(argv[0]);

	/* Registruju se funkcije za obradu dogadjaja. */
	glutKeyboardFunc(on_keyboard);
	glutDisplayFunc(on_display);
	glutMotionFunc(on_mouse_move_active);
	glutPassiveMotionFunc(on_mouse_move);
	glutMouseFunc(on_mouse_pressed_released);
    glutKeyboardUpFunc(keyboard_up);
    glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
    
    glutReshapeFunc(on_reshape);
    myPlayer = new Player();
    players[0] = myPlayer;
	//glutSetCursor(GLUT_CURSOR_NONE);

	/* Obavlja se OpenGL inicijalizacija. */
	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable(GLUT_MULTISAMPLE);
	glEnable(GL_TEXTURE_2D);
	
	//glShadeModel (GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	//glEnable(GL_LIGHT2);
	//glEnable(GL_COLOR_MATERIAL);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,
              GL_MODULATE);


	LoadTextures();
    
	//GLfloat lightPos0[] = { 0, 0, -2, 1};
	//glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

    /* Ulazi se u glavnu petlju. */
	glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
  	animation_ongoing = 1;

	glutMainLoop();

    return 0;
}


float windowWidth, windowHeight, aspectRatio;

static void on_reshape(int width, int height){
	windowWidth = width;
	windowHeight = height;
	aspectRatio = (float)width/height;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,  (float)width/height, 0.01 ,  1000);
}

static void on_keyboard(unsigned char key, int x, int y)
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

static void keyboard_up(unsigned char key, int x, int y){
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


static void on_mouse_pressed_released(int button, int state, int x, int y) {
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN)
				myPlayer->input.shoot = true;
			else if(state == GLUT_UP)
				myPlayer->input.shoot = false;
			break;
	}

}

static void on_mouse_move_active(int x, int y) {
	
}


static void on_mouse_move(int x, int y){
	
}



static void on_timer(int value)
{
	/*
	* Proverava se da li callback dolazi od odgovarajuceg tajmera.
	*/
	if (value != TIMER_ID){
		return;
	}

    for (int i=0; i<1; i++){
        players[i]->Update();
        players[i]->equiped_weapon->Update(players[i]->input.shoot);
    }
	/* Po potrebi se ponovo postavlja tajmer. */
	if (animation_ongoing) {
		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
	}

	glutPostRedisplay();
}

void DrawMap(){
	glColor3f(1, 1, 1);
	glBegin(GL_TRIANGLE_STRIP);

	glBindTexture(GL_TEXTURE_2D, textureNames[0]);
	glNormal3f(0, 0, 1);
	glTexCoord2f(0, 0);
	glVertex3f(-9, -9.2, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-9, 9.2, 0);
	glTexCoord2f(1, 0);
	glVertex3f(9, -9.2, 0);
	glTexCoord2f(1, 1);
	glVertex3f(9, 9.2, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();
}

static void DrawPlayers(){
    for (int i=0; i<1; i++){
        players[i]->Draw();
    }
}

static void on_display(void)
{
	/* Postavlja se boja svih piksela na zadatu boju pozadine. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt  (myPlayer->position.x, myPlayer->position.y, 4,
                myPlayer->position.x, myPlayer->position.y, 0,
                0, 1, 0);
	
	DrawMap();
	DrawPlayers();

	/* Nova slika se salje na ekran. */
	glutSwapBuffers();
	
}


