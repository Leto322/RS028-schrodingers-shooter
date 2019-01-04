#ifndef PLAYER_DEF
#define PLAYER_DEF

#include "../header/util.h"
#include "../header/items.h"
#include <Box2D/Box2D.h>

typedef struct _Input{
    float horizontal, vertical, angle;
    bool shoot;
} Input;


class Player : public Colider{
public:
    Player(float x, float y, float r);
    Player();
	  void Draw();
	  void DrawShadow();
    void Update();
    void Move();
    void takeDmg(int dmg);
    virtual ClassID getClassID();
    bool team;
    Input input;
    Weapon* equiped_weapon;
    b2Body* body;
    float r;

private:
    float speed;
    int health;
    void die();
protected:

};

#endif
