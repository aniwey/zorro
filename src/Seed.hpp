#ifndef HPP_SEED
#define HPP_SEED

#include <cmath>

#include <boost/serialization/base_object.hpp>

#include "Entity.hpp"

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
    Seed(){};
    Seed(Land&, int, int);
    ~Seed();
    
    bool loop(Land&);
    
  private:
    // Serialization
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar & boost::serialization::base_object<Entity>(*this);
      ar & step;
      ar & time;
    }
  
    // Variables
    seedStep step; // Used to store the step (see the enumeration above)
    int time; // Used to count number of loops we spend on certain steps
    
    // Functions
    void growLeaves(Land&, int xSeed, int ySeed, bool withFruits);
    bool growLeafHere(Land&, int xGrow, int yGrow, int xSeed, int ySeed, bool andMaybeAFruit);
};

#endif
