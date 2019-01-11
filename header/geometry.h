#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../header/util.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Box2D/Box2D.h>
#include <cmath>
#include <GL/glut.h>



// Struktura Block
struct Block : public Colider{
    Block(b2Vec2 A,double edge);
    b2Vec2 m_A;
    double m_edge;
    b2Vec2 m_vertexes[4];
    b2Body* m_body;

    virtual ClassID getClassID();
};




extern b2World* world;

//Funkcija koja ucitava zidove iz fajla
void LoadWalls();
b2Vec2 ScaleVec(const b2Vec2 & A);
// void AddWall(float x, float y, float w, float h);
// void AddWall(Block& wall);
void DrawWalls();



#endif
