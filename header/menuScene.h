#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <iostream>
#include <string>
#include <vector>

void InitMenu();

void on_mouse_pressed_released_menu(int button, int state, int x, int y);
// 
// void on_mouse_move_active_game(int x, int y);
// 
// void on_mouse_move_game(int x, int y);

void on_keyboard_menu(unsigned char key, int x, int y);

void pressButton(int x, int y);
void releaseButton(int x, int y);
void on_timer_menu();
void DrawMenu();
void DrawControls();
void DrawCredits();

void on_display_menu(void);
void drawBitmapText(char *string,float x,float y,float z);

// class Button{
// public:
// 	Button(std::string& name, float x, float y)
// 		:m_name(std::move(name)), m_x(x), m_y(y);
// 	
// 	float m_x;
// 	float m_y;
// 	bool pressed = false;
// 	std::string m_name;
// };
#endif
