#ifndef PLAYER_DEF
#define PLAYER_DEF
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
private:
    float rotation, r;
    float speed;
protected:

};

#endif
