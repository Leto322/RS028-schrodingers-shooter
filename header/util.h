#ifndef UTIL_H
#define UTIL_H

#include <Box2D/Box2D.h>

enum ClassID{
  COLIDER,
  PLAYER,
  BULLET,
  BLOCK
};

//Empty Base Class of all classes that have collision logic
class Colider{
public:
  virtual ClassID getClassID() { return COLIDER; }
};

class RayCastCallback : public b2RayCastCallback{
public:
  RayCastCallback(): m_fixture(NULL)
  {};

  float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
    m_fixture = fixture;
    return fraction;
  }

  b2Fixture* m_fixture;
};

#endif
