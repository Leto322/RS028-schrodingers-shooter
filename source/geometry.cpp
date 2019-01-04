#include "../header/geometry.h"
#include "../header/util.h"

std::vector< std::vector<char> > map;
extern std::vector<Block> walls;
extern std::vector<Block> ground;
extern GLuint textureNames[2];
// extern std::vector<Line> walls
// m_x = m_x/855*18-9;
// m_y = -m_y/855*18.4+9.2;




/*            3'
 *             ######## 2'
 *           # #    # #
 *       0'######## 1'#
 *         #   #  #   #
 *         # 3 #  #   # 2
 *         # #    # #
 *         ########
 *        0       1
 *
 *
 */



//Definicije konstruktora i metoda za klasu Block
Block::Block(b2Vec2 A, double edge)
{
    m_A = A;
    m_edge = edge;
    m_vertexes[0] = b2Vec2(m_A.x-edge/2,m_A.y-edge/2);
    m_vertexes[1] = b2Vec2(m_A.x+edge/2,m_A.y-edge/2);
    m_vertexes[2] = b2Vec2(m_A.x+edge/2,m_A.y+edge/2);
    m_vertexes[3] = b2Vec2(m_A.x-edge/2,m_A.y+edge/2);
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(m_A.x, m_A.y);
    m_body = world->CreateBody(&groundBodyDef);
    //m_body->SetUserData(this);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(m_edge/2, m_edge/2);
    m_body->CreateFixture(&groundBox, 0.0f);
}



void ScaleVec(b2Vec2 * A){
    A->x = A->x - 9;
    A->y = -A->y + 9;
}


void LoadWalls()
{
    std::string lineFile;
    b2Vec2 A;
    std::ifstream myfile ("walls.txt");
    int n = 0;
    int i;
    int j;
    std::vector<char> line;
    double edge = 0;
     if (myfile.is_open()){
        while(getline(myfile,lineFile)){
            n = lineFile.length();
            for(i=0;i<n;i++){
                line.push_back(lineFile[i]);
            }
            map.push_back(line);
            line.clear();

        }
    }
    edge = 18.0/n;

    for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                std::cout << map[i][j];
            }
            std::cout << std::endl;
    }

     for(i=0;i<n;i++){
            for(j=0;j<n;j++){
                if(map[i][j] == '#'){
                    A.x = j*edge+edge/2;
                    A.y = i*edge+edge/2;
                    ScaleVec(&A);
                    walls.push_back(Block(A, edge));
                }
//                 else{
//                     A.x = j*edge+edge/2;
//                     A.y = i*edge+edge/2;
//                     ScaleVec(&A);
//                     ground.push_back(Block(A, edge));
//                 }
            }
    }

    myfile.close();
//     n=walls.size();
//     for (i=0;i<n;i++){
// //         AddWall(walls[i].m_A.x, walls[i].m_A.y, walls[i].m_edge/2, walls[i].m_edge/2);
//         AddWall(walls[i]);
//     }
}



/*            3'
 *             ######## 2'
 *           # #    # #
 *       0'######## 1'#
 *         #   #  #   #
 *         # 3 #  #   # 2
 *         # #    # #
 *         ########
 *        0       1
 *
 *
 */


void DrawWalls(){
    int n = walls.size();
    int i;
    for(i=0;i<n;i++){
        glPushMatrix();
            glColor3f(1,1,1);
            glBindTexture(GL_TEXTURE_2D, textureNames[1]);
            glBegin(GL_QUADS);
                //front quad
                glNormal3f(0, -1, 0);
                glTexCoord2f(0,0);
                glVertex3f(walls[i].m_vertexes[0].x, walls[i].m_vertexes[0].y, 0);
                glTexCoord2f(1,0);
                glVertex3f(walls[i].m_vertexes[1].x, walls[i].m_vertexes[1].y, 0);
                glTexCoord2f(1,1);
                glVertex3f(walls[i].m_vertexes[1].x, walls[i].m_vertexes[1].y, walls[i].m_edge);
                glTexCoord2f(0,1);
                glVertex3f(walls[i].m_vertexes[0].x, walls[i].m_vertexes[0].y, walls[i].m_edge);
                //right quad
                glNormal3f(1, 0, 0);
                glTexCoord2f(0,0);
                glVertex3f(walls[i].m_vertexes[1].x, walls[i].m_vertexes[1].y, 0);
                glTexCoord2f(1,0);
                glVertex3f(walls[i].m_vertexes[2].x, walls[i].m_vertexes[2].y, 0);
                glTexCoord2f(1,1);
                glVertex3f(walls[i].m_vertexes[2].x, walls[i].m_vertexes[2].y, walls[i].m_edge);
                glTexCoord2f(0,1);
                glVertex3f(walls[i].m_vertexes[1].x, walls[i].m_vertexes[1].y, walls[i].m_edge);
                //left quad
                glNormal3f(-1, 0, 0);
                glTexCoord2f(0,0);
                glVertex3f(walls[i].m_vertexes[3].x, walls[i].m_vertexes[3].y, 0);
                glTexCoord2f(1,0);
                glVertex3f(walls[i].m_vertexes[0].x, walls[i].m_vertexes[0].y, 0);
                glTexCoord2f(1,1);
                glVertex3f(walls[i].m_vertexes[0].x, walls[i].m_vertexes[0].y, walls[i].m_edge);
                glTexCoord2f(0,1);
                glVertex3f(walls[i].m_vertexes[3].x, walls[i].m_vertexes[3].y, walls[i].m_edge);
                //back quad
                glNormal3f(0, 1, 0);
                glTexCoord2f(0,0);
                glVertex3f(walls[i].m_vertexes[2].x, walls[i].m_vertexes[2].y, 0);
                glTexCoord2f(1,0);
                glVertex3f(walls[i].m_vertexes[3].x, walls[i].m_vertexes[3].y, 0);
                glTexCoord2f(1,1);
                glVertex3f(walls[i].m_vertexes[3].x, walls[i].m_vertexes[3].y, walls[i].m_edge);
                glTexCoord2f(0,1);
                glVertex3f(walls[i].m_vertexes[2].x, walls[i].m_vertexes[2].y, walls[i].m_edge);
                //top quad
                glNormal3f(0, 0, 1);
                glTexCoord2f(0,0);
                glVertex3f(walls[i].m_vertexes[0].x, walls[i].m_vertexes[0].y, walls[i].m_edge);
                glTexCoord2f(1,0);
                glVertex3f(walls[i].m_vertexes[1].x, walls[i].m_vertexes[1].y, walls[i].m_edge);
                glTexCoord2f(1,1);
                glVertex3f(walls[i].m_vertexes[2].x, walls[i].m_vertexes[2].y, walls[i].m_edge);
                glTexCoord2f(0,1);
                glVertex3f(walls[i].m_vertexes[3].x, walls[i].m_vertexes[3].y, walls[i].m_edge);
            glEnd();
            glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();
    }
}

// void AddWall(float x, float y, float w, float h){
//     b2BodyDef groundBodyDef;
//     groundBodyDef.position.Set(x, y);
//     b2Body* groundBody = world->CreateBody(&groundBodyDef);
//     b2PolygonShape groundBox;
//     groundBox.SetAsBox(w, h);
//     groundBody->CreateFixture(&groundBox, 0.0f);
// }

//ClassID Block::getClassID(){return BLOCK;}
