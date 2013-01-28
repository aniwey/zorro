#ifndef HPP_FRUIT
#define HPP_FRUIT

#include <boost/serialization/base_object.hpp>

#include <SFML/Graphics/Color.hpp>

#include "color.hpp"
#include "Entity.hpp"

typedef enum{
  fruitStep_RIPENING,
  fruitStep_FALLING_INSIDE_LEAVES,
  fruitStep_JUST_FALLING
}fruitStep;

typedef enum{
  fruitDyingStep_NOTHING,
  fruitDyingStep_LANDING_COULD_BEGIN_DYING,
  fruitDyingStep_DYING
}fruitDyingStep;

class Fruit:public Entity{
  public:
    Fruit(){};
    Fruit(Land&, int, int);
    ~Fruit();
    
    bool loop(Land&, int, int);
    void isGoingToFall();
    
  private:
    // Serialization
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      boost::serialization::base_object<Entity>(*this);
      ar & step;
      ar & time;
      ar & dyingStep;
      ar & dyingTime;
      ar & leavesColorUnderUs;
    }
  
    fruitStep step;
    int time;
    fruitDyingStep dyingStep;
    int dyingTime;
    sf::Color leavesColorUnderUs;
};

#endif
