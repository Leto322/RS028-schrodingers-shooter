#ifndef PLAYER_DEF
#define PLAYER_DEF

#include "weapon.h"
#include "../header/util.h"
#include <Box2D/Box2D.h>
#include <AL/alut.h>

typedef struct _Input{
    float horizontal, vertical, angle;
    bool shoot;
} Input;

class Brain;

class Player : public Colider{
public:
    Player(float x, float y, float r);
    Player();
    void Draw();
    void DrawShadow();
	  void SwapWeapon(Weapon* newWeapon);
	  void takeDmg(int dmg);
	  void IncreaseHealth(int amount);

    virtual ClassID getClassID();
    void SetBrain(Brain* brain);
    Brain* m_brain;
    bool team;
    Input input;
    Weapon* equiped_weapon;
    b2Body* body;
    float r;
    bool see_player;
    bool isDead;



private:
    float speed;
    int health;
    void die();
protected:
  ALuint soundSource[1];

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

void BotMoves();
void BotAim();
void Move(int ip, int jp,std::vector<std::vector<int>>& pathMap);

#endif
