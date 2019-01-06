#include "../header/player.h"
#include "../header/weapon.h"
#include "../header/util.h"
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <Box2D/Box2D.h>
#include <queue>

extern b2World* world;
extern std::vector< std::vector<char> > map;
extern std::vector<Player*> players;
extern float windowWidth, windowHeight;


//Number of players that we havent reached in the BFS search
int num;

/*Player::Player(float x, float y, float r)
:   position({x, y}), r(r)
{
    Player();

};*/

Player::Player(){
    r = 0.15;
    speed = 0.03;
    health = 100;
    input.vertical = 0;
    input.horizontal = 0;
    input.shoot = false;
	input.angle= M_PI/2;
    equiped_weapon = new Weapon(0.0f, 0.0f, input.angle, 0.1);
    team = false;

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
	bodyDef.linearDamping = 2.0f;
    bodyDef.position.Set(0.0f, 0.0f);
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
};

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
		float percentage = health / 100.0;
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
}
void Player::die(){
	std::cout << "Player is dead!" << std::endl;
}

void Player::takeDmg(int dmg){
	health -= dmg;
	if (health <= 0) {
		health = 0;
		die();
	}
}

ClassID Player::getClassID() {return PLAYER;}

//Function that finds the player we found in the search and adjusts his vertical/horizontal movement
void Move(int ip, int jp,std::vector<std::vector<int>>& pathMap){
    int i,j;
    for(int k=1;k<players.size();++k){
        i = map.size()-1-(floor((players[k]->body->GetPosition().y + 9.0)/18*map.size()));
        j = floor((players[k]->body->GetPosition().x + 9.0)/18*map.size());
        if(i == ip && j == jp){
            if(pathMap[i-1][j] == pathMap[ip][jp]-1 && pathMap[ip][jp]-1 != 0){
                players[k]->input.vertical+=1;
                
            }else if(pathMap[i+1][j] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0){
                players[k]->input.vertical-=1;
                
            }else if(pathMap[i][j+1] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0){
                players[k]->input.horizontal+=1;
                
            }else if(pathMap[i][j-1] == pathMap[ip][jp]-1&& pathMap[ip][jp]-1 != 0){
                players[k]->input.horizontal-=1;

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
        
    }
    
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
        else
            players[k]->input.shoot = true;
        angle = atan2(y1-y2, x1-x2);
        players[k]->input.angle = angle;
    }
}
