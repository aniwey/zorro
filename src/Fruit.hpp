#ifndef HPP_FRUIT
#define HPP_FRUIT

#include "color.hpp"
#include "Entity.hpp"
#include "Land.hpp"

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
    Fruit(Land&, int, int);
    ~Fruit();
    
    bool loop(Land&, int, int);
    void isGoingToFall();
    
  private:
    fruitStep step;
    int time;
    fruitDyingStep dyingStep;
    int dyingTime;
    Pixel pixelUnderUs;
};

#endif
