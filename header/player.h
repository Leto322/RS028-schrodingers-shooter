#ifndef PLAYER_DEF
#define PLAYER_DEF

#include "../header/items.h"

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
    Point position;
    Weapon* equiped_weapon;
private:
    float rotation, r;
    float speed;
protected:

};

#endif
