#include <iostream>
#include <GL/glut.h>
#include "../header/image.h"
#include <string>
#include <vector>

std::vector<std::string> textureLocations = {
	"textures/wall.bmp"
	};

#define TIMER_ID 0
#define TIMER_INTERVAL 20

static GLuint textureNames[1];

static int animation_ongoing;
static void on_keyboard(unsigned char key, int x, int y);
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
		std::cout << "width " << image->width << std::endl; 
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
	glutReshapeFunc(on_reshape);

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
	}
}


static void on_mouse_pressed_released(int button, int state, int x, int y) {
	

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
	glVertex3f(-6.4, -3.6, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-6.4, 3.6, 0);
	glTexCoord2f(1, 0);
	glVertex3f(6.4, -3.6, 0);
	glTexCoord2f(1, 1);
	glVertex3f(6.4, 3.6, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnd();
}

static void on_display(void)
{
	/* Postavlja se boja svih piksela na zadatu boju pozadine. */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 6.5,
		0, 0, 0,
		0, 1, 0);
	
	DrawMap();
	//DrawPlayers();

	/* Nova slika se salje na ekran. */
	glutSwapBuffers();
	
}


