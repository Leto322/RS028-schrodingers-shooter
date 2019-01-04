#ifndef UTIL_H
#define UTIL_H

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

#endif
