#ifndef PLAYER_DEF
#define PLAYER_DEF

#include "../header/items.h"
#include <Box2D/Box2D.h>

typedef struct _Point{
    float x, y;
} Point;

typedef struct _Input{
    float horizontal, vertical;
    bool shoot;
} Input;


class Player{
public:
    Player(float x, float y, float r);
    Player();
    void Draw();
    void Update();
    void Move();
    bool team;
    Input input;
    int health;
    Weapon* equiped_weapon;
    b2Body* body;
private:
    float rotation, r;
    float speed;
protected:

};

#endif
