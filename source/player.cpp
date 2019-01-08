#include "../header/player.h"
#include "../header/weapon.h"
#include "../header/util.h"
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <Box2D/Box2D.h>
#include <queue>

extern b2World* world;
extern std::vector< std::vector<char> > map;
extern std::vector<Player*> players;
extern float windowWidth, windowHeight;
extern std::map<std::string, int> sounds;


//Number of players that we havent reached in the BFS search
int num;

/*Player::Player(float x, float y, float r)
:   position({x, y}), r(r)
{
    Player();

};*/

Player::Player() {
    r = 0.15;
    speed = 0.03;
	maxHealth = 100;
    health = maxHealth;
    input.vertical = 0;
    input.horizontal = 0;
	input.shoot = false;
	input.angle= M_PI/2;
	equiped_weapon = new Pistol(0.0f, 0.0f, input.angle);
	deathFlag = false;
	alive = false;
    team = false;
    see_player = false;
    isDead = false;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.0f;
    bodyDef.position.Set(-100.0f, 0.0f);
    body = world->CreateBody(&bodyDef);
    body->SetUserData(this);

    // Define another box shape for our dynamic body.
    b2CircleShape circleShape;
    circleShape.m_p.Set(0, 0); //position, relative to body position
    circleShape.m_radius = r;
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    // Set the box density to be non-zero, so it will be dynamic.
    fixtureDef.density = 1.0f;
    // Override the default friction.
    fixtureDef.friction = 0.01f;
    // Add the shape to the body.
    body->CreateFixture(&fixtureDef);

    alGenSources(1, soundSource);
  	alSourcei(soundSource[0], AL_BUFFER, sounds["death"]);
    alSourcef(soundSource[0], AL_GAIN, 0.1);
  	alSourcef(soundSource[0], AL_PITCH, 1);

};

void Player::SetMaxHealth(int mh) {
	maxHealth = mh;
	health = maxHealth;
}

void Player::SetBrain(Brain* brain){
    m_brain = brain;
}

void Player::Draw(){
    glPushMatrix();
		if (team){
			glColor3f(1, 0.3, 0.3);
		}
		else{
			glColor3f(0.3, 0.3, 1);
		}
		b2Vec2 pos = body->GetPosition();
		glTranslatef(pos.x, pos.y, r);
		glutSolidSphere(r, 15, 5);
		glPushMatrix();
			glRotatef(input.angle*180/M_PI, 0, 0, 1);
			glTranslatef(r, 0, 0);
			glColor3f(0, 0, 0);
			glutSolidSphere(r/4, 10, 2);
		glPopMatrix();
		//Draw health bar
		glColor3f(0, 0, 0);
		glTranslatef(0, 0.3, 0.4);
		glScalef(0.3, 0.1, 0.1);
		glutWireCube(1);
		float percentage = (float)health / maxHealth;
		glTranslatef(-0.5*(1-percentage), 0, 0);
		glScalef(percentage, 1, 1);
		glColor3f(1 - percentage, percentage, 0);
		glutSolidCube(1);
    glPopMatrix();
};

void Player::DrawShadow() {
	//DRAWING PLAYER SHADOW
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTranslatef(body->GetPosition().x + r / 2, body->GetPosition().y - r / 2, 0);
	glRotatef(45, 0, 0, 1);
	glScalef(1, 1.5, 0.1);
	glColor4f(0, 0, 0, 0.2);
	glutSolidSphere(r, 20, 2);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glPopMatrix();
}

Brain::Brain(Player& player)
    : m_player(&player) {}


playerBrain::playerBrain(Player& player)
    : Brain(player) {}

void playerBrain::Update(){
    float vx = Brain::m_player->input.horizontal;
    float vy = Brain::m_player->input.vertical;

    b2Vec2 vel(vx, vy);

    Brain::m_player->body->SetLinearVelocity(vel);

    vx = cos(Brain::m_player->input.angle);
    vy =  sin(Brain::m_player->input.angle);
    float n = 0.18;
    Brain::m_player->equiped_weapon->SetPositionAndAngle(Brain::m_player->body->GetPosition().x + vx*n, Brain::m_player->body->GetPosition().y + vy*n, Brain::m_player->input.angle);

    //Sound source is protected, need to fix
    //alSource3f(Brain::m_player->soundSource[0], AL_POSITION, Brain::m_player->body->GetPosition().x, Brain::m_player->body->GetPosition().y, 0);
}

botBrain::botBrain(Player& player)
    : Brain(player) {}

void botBrain::Update(){
    float vx = Brain::m_player->input.horizontal;
    float vy = Brain::m_player->input.vertical;

    b2Vec2 vel(vx, vy);

    Brain::m_player->body->SetLinearVelocity(vel);

    vx = cos(Brain::m_player->input.angle);
    vy =  sin(Brain::m_player->input.angle);
    float n = 0.18;
    Brain::m_player->equiped_weapon->SetPositionAndAngle(Brain::m_player->body->GetPosition().x + vx*n, Brain::m_player->body->GetPosition().y + vy*n, Brain::m_player->input.angle);

    RayCastCallback ray_callback;
    b2Vec2 bot_pos(Brain::m_player->body->GetPosition().x, Brain::m_player->body->GetPosition().y);
    b2Vec2 player_pos(players[0]->body->GetPosition().x, players[0]->body->GetPosition().y);

    //std::cout<<bot_pos.y << " ;; " << player_pos.y <<std::endl;

    m_player->see_player = false;
    world->RayCast(&ray_callback, bot_pos, player_pos);
    if(ray_callback.m_fixture){
        if(ray_callback.m_fixture->GetBody() == players[0]->body)
          m_player->see_player = true;
    }
}

void Player::die(){
	alSourcePlay(soundSource[0]);
	alive = false;
	body->SetTransform(b2Vec2(-100, 0), 0);
	std::cout << "Player is dead!" << std::endl;
  isDead = true;
}

void Player::takeDmg(int dmg){
	int tmp = health;
	health -= dmg;
	if (health <= 0) {
		health = 0;
		if (tmp > 0) {
			deathFlag = true;
		}
		
	}
}
void Player::IncreaseHealth(int amount) {
	health += amount;
	health = health > maxHealth ? maxHealth : health;
}

void Player::Revive() {
	health = maxHealth;
	alive = true;
	equiped_weapon->reload();
}

void Player::SwapWeapon(Weapon* newWeapon) {
	Weapon* old = equiped_weapon;
	equiped_weapon = newWeapon;
	//delete(old);
}

ClassID Player::getClassID() {return PLAYER;}

//Function that finds the player we found in the search and adjusts his vertical/horizontal movement
void Move(int ip, int jp,std::vector<std::vector<int>>& pathMap){
    int i,j;
    int minu,mind,minl,minr;
    int minimum;
    float edge = 18.0/map.size();


    for(int k=1;k<players.size(); k++){
        i = map.size()-1-(floor((players[k]->body->GetPosition().y + 9.0)/18*map.size()));
        j = floor((players[k]->body->GetPosition().x + 9.0)/18*map.size());
        if(i == ip && j == jp){
            if(pathMap[i-1][j] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0){
                players[k]->input.vertical+=1;
                std::cout << "u" << std::endl;

            }else if(pathMap[i+1][j] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0){
                players[k]->input.vertical-=1;

            }else if(pathMap[i][j+1] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0){
                players[k]->input.horizontal+=1;

            }else if(pathMap[i][j-1] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0){
                players[k]->input.horizontal-=1;

            }
            else if(pathMap[i-1][j-1] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0 && map[i-1][j] != '#' && map[i][j-1] != '#'){
                players[k]->input.vertical+=1;
                players[k]->input.horizontal-=1;

            }
            else if(pathMap[i+1][j-1] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0 && map[i+1][j] != '#' && map[i][j-1] != '#'){
                players[k]->input.vertical-=1;
                players[k]->input.horizontal-=1;

            }
            else if(pathMap[i+1][j+1] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0 && map[i+1][j] != '#' && map[i][j+1] != '#'){
                players[k]->input.horizontal+=1;
                players[k]->input.vertical-=1;
//                 std::cout << "dr" << std::endl;
            }
            else if(pathMap[i-1][j+1] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0 && map[i-1][j] != '#' && map[i][j+1] != '#'){
                players[k]->input.horizontal+=1;
                players[k]->input.vertical+=1;
//                 std::cout << "ur" << std::endl;
            }

            //Backup in case block is up/down/right/left during diagonal moving
            else if (players[k]->input.vertical == 0 && players[k]->input.horizontal == 0){
                minu = pathMap[i-1][j];
                mind = pathMap[i+1][j];
                minl = pathMap[i][j-1];
                minr = pathMap[i][j+1];

                if(minu == -1)
                    minu = map.size()*map.size();
                if(mind == -1)
                    mind = map.size()*map.size();
                if(minl == -1)
                    minl = map.size()*map.size();
                if(minr == -1)
                    minr = map.size()*map.size();

                minimum = std::min(minu, std::min(mind, std::min(minl,minr)));

                if(minu == 0 || mind == 0 || minl == 0 || minr == 0){

                }else if(minimum == minu){
                    players[k]->input.vertical+=1;
                }else if(minimum == mind){
                    players[k]->input.vertical-=1;
                }else if(minimum == minl){
                    players[k]->input.horizontal-=1;
                }else{
                    players[k]->input.horizontal+=1;
                }
            }

            //Moves the bot towards the middle of the block if he is too close to the edge
            if(-(i+1) * edge > (players[k]->body->GetPosition().y - 9.0 - (players[k]->r + 0.05)) && players[k]->input.vertical!=1 && players[k]->input.vertical!=-1){
                players[k]->input.vertical+=1;

            }else if(-i * edge < (players[k]->body->GetPosition().y - 9.0 + (players[k]->r + 0.05))  && players[k]->input.vertical!=-1 && players[k]->input.vertical!=1){
                players[k]->input.vertical-=1;

            }
            if((j+1) * edge < (players[k]->body->GetPosition().x + 9.0 + (players[k]->r + 0.05)) && players[k]->input.horizontal!=-1 && players[k]->input.horizontal !=1){
                players[k]->input.horizontal-=1;


            }else if(j * edge > (players[k]->body->GetPosition().x + 9.0 - (players[k]->r + 0.05)) && players[k]->input.horizontal!=1  && players[k]->input.horizontal != -1){
                players[k]->input.horizontal+=1;
            }


            //Removing the player marker from the map and reducing the number of players that werent reached
            map[i][j] = ' ';
            num--;
        }

    }
}


//BFS search of the map starting from player to all of the bots and calls BotAim function
void BotMoves(){
    BotAim();
    int i, j, ip, jp;
    num = 0;

    //Matrix that contains the number of fields on the shortest path from each field to the player
    std::vector<std::vector<int>> pathMap(map.size());
    std::queue<std::pair<int,int>> queue;
    for(i=0;i<map.size();i++){
        pathMap[i] = std::vector<int>(map.size());
        for(j=0;j<map.size();j++){
          pathMap[i][j] = -1;
        }
    }




    //Player position based on coordinates
    ip = map.size()-1-(floor((players[0]->body->GetPosition().y + 9.0)/18*map.size()));
    jp = floor((players[0]->body->GetPosition().x + 9.0)/18*map.size());
    pathMap[ip][jp] = 0;

    //Bot positions based on coordinates
    for(int k=1;k<players.size();++k){
        i = map.size()-1-(floor((players[k]->body->GetPosition().y + 9.0)/18*map.size()));
        j = floor((players[k]->body->GetPosition().x + 9.0)/18*map.size());

        players[k]->input.horizontal=0;
        players[k]->input.vertical=0;


        if(i == ip && j == jp)
            continue;
        map[i][j] = 'B';
        num++;
    }
    queue.push(std::pair<int,int>{ip,jp});

    //BFS
    while(!queue.empty() && num > 0){
        i = queue.front().first;
        j = queue.front().second;
        queue.pop();
        if(map[i][j] == 'B'){
            Move(i, j, pathMap);
        }

        if(i - 1 >= 0 && map[i-1][j] != '#' && pathMap[i-1][j] == -1){
            queue.push(std::pair<int,int>{i-1, j});
            pathMap[i-1][j] = pathMap[i][j]+1;
        }
        if(i + 1 < map.size() && map[i+1][j] != '#' && pathMap[i+1][j] == -1){
            queue.push(std::pair<int,int>{i+1, j});
            pathMap[i+1][j] = pathMap[i][j]+1;
        }
        if(j - 1 >= 0 && map[i][j-1] != '#' && pathMap[i][j-1] == -1){
            queue.push(std::pair<int,int>{i, j-1});
            pathMap[i][j-1] = pathMap[i][j]+1;
        }
        if(j + 1 < map.size() && map[i][j+1] != '#' && pathMap[i][j+1] == -1){
            queue.push(std::pair<int,int>{i, j+1});
            pathMap[i][j+1] = pathMap[i][j]+1;
        }
        if(i - 1 >= 0 && j - 1 >= 0 && map[i-1][j-1] != '#' && pathMap[i-1][j-1] == -1){
            queue.push(std::pair<int,int>{i-1, j-1});
            pathMap[i-1][j-1] = pathMap[i][j]+1;
        }
        if(i - 1 >= 0 && j + 1 < map.size()  && map[i-1][j+1] != '#' && pathMap[i-1][j+1] == -1){
            queue.push(std::pair<int,int>{i-1, j+1});
            pathMap[i-1][j+1] = pathMap[i][j]+1;
        }
        if(i + 1 < map.size() && j - 1 >= 0 && map[i+1][j-1] != '#' && pathMap[i+1][j-1] == -1){
            queue.push(std::pair<int,int>{i+1, j-1});
            pathMap[i+1][j-1] = pathMap[i][j]+1;
        }
        if(i + 1 < map.size() && j + 1 < map.size() && map[i+1][j+1] != '#' && pathMap[i+1][j+1] == -1){
            queue.push(std::pair<int,int>{i+1, j+1});
            pathMap[i+1][j+1] = pathMap[i][j]+1;
        }

    }
    pathMap.clear();
    while(!queue.empty())
        queue.pop();
}

//Adjusting bot aiming
void BotAim(){
    float x1, x2;
    float y1, y2;
    float h, w;
    x1 = players[0]->body->GetPosition().x;
    y1 = players[0]->body->GetPosition().y;
    float angle;
    for(int k = 1; k < players.size(); ++k){
        if(players[k]->equiped_weapon->GetAmmo() == 0)
            players[k]->equiped_weapon->reload();
        x2 = players[k]->body->GetPosition().x;
        y2 = players[k]->body->GetPosition().y;
        h = tan(30*M_PI/180)*4;
        w = h*windowWidth/windowHeight;

        if(x2 > x1 + w || x2 < x1 - w || y2 > y1 + h || y2 < y1 - h)
            players[k]->input.shoot = false;
        else if(players[k]->see_player)
            players[k]->input.shoot = true;

        angle = atan2(y1-y2, x1-x2);
        players[k]->input.angle = angle;

    }
}
