#ifndef HPP_SEED
#define HPP_SEED

#include <cmath>

#include "Entity.hpp"
#include "Land.hpp"

class Land;

typedef enum{
  seedStep_FALLING,
  seedStep_LANDING,
  seedStep_PLANTED,
  seedStep_GROWING,
  seedStep_GROWING_WITH_FRUITS,
  seedStep_DYING
}seedStep;

class Seed:public Entity{
  public:
    Seed(Land&, int, int);
    ~Seed();
    
    bool loop(Land&, int, int);
    void isGoingToFall();
    
  private:
    // Variables
    seedStep step; // Used to store the step (see the enumeration above)
    int time; // Used to count number of loops we spend on certain steps
    
    // Functions
    void growLeaves(Land&, int xSeed, int ySeed, bool withFruits);
    bool growLeafHere(Land&, int xGrow, int yGrow, int xSeed, int ySeed, bool andMaybeAFruit);
};

#endif
