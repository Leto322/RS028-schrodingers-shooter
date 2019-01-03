#ifndef PLAYER_DEF
#define PLAYER_DEF

#include "../header/items.h"
#include <Box2D/Box2D.h>

typedef struct _Input{
    float horizontal, vertical, angle;
    bool shoot;
} Input;

class Brain;

class Player{
public:
    Player(float x, float y, float r);
    Player();
	void Draw();
	void DrawShadow();
//     void Update();
//     void Move();   
    
    void SetBrain(Brain* brain);

    bool team;
    Input input;
    int health;
    
    Brain* m_brain;
    Weapon* equiped_weapon;
    b2Body* body;
    float r;
private:
    float speed;
protected:

};

class Brain{
protected:
    Brain(Player& player);
    Player* m_player;
    ~Brain();
    
public:
    virtual void Update() = 0;
};


class playerBrain: public Brain{
public:
    playerBrain(Player& player);
    
public:
    void Update();
};

class botBrain: public Brain{
public:
    botBrain(Player& player);
    
public:
    void Update();
};

#endif
