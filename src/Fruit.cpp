#include "Fruit.hpp"

Fruit::Fruit(Land& l, int x, int y): Entity(l.frame_id){
  // Get the pixel that's under us
  pixelUnderUs = l.p[x][y];
  
  // Set type
  l.p[x][y].type = pixelType_FRUIT;
  
  // Set color
  l.p[x][y].setColorBasedOnType();
  
  // Set group : we keep the group of the pixel under us
    
  // Set private parameters
  step = fruitStep_RIPENING;
  time = 0;
  dyingStep = fruitDyingStep_NOTHING;
  dyingTime = 0;
}

Fruit::~Fruit(){
  
}

void Fruit::isGoingToFall(){
  // If we're landing, we're not anymore because we felt
  if(dyingStep == fruitDyingStep_LANDING_COULD_BEGIN_DYING) dyingStep = fruitDyingStep_NOTHING;
}

bool Fruit::loop(Land& l, int x, int y){
  last_frame_id = l.frame_id;
  
  // Handling of the dying step
  switch(dyingStep){
    // If we're not landing, not dying, anything
    case fruitDyingStep_NOTHING:
      dyingTime = 0; // We reset the timer so that the next time we're landing we have to wait a lot before beginning dying
    break;
    // If we're landing and could begin dying
    case fruitDyingStep_LANDING_COULD_BEGIN_DYING:
      if(dyingTime > 120){
        if(l.howManyPixelsOfThisTypeInThisRectangle(pixelType_FRUIT, x-2, y-2, x+2, y+2) > 7 || l.howManyPixelsOfThisTypeInThisRectangle(pixelType_FRUIT, x, y-2, x, y+2) > 3){
          dyingStep = fruitDyingStep_DYING;
          dyingTime = 0;
        }
      }
      else dyingTime++;
    break;
    // If we're dying
    case fruitDyingStep_DYING:
      if(dyingTime > 60){
        return true; // We destroy ourselves
      }
      else{
        l.p[x][y].color = getMidColor(sf::Color(237, 123, 0), sf::Color(107, 55, 0), 1-(float)dyingTime/60);
        dyingTime++;
      }
    break;
  }
  
  // Handling of the normal step
  switch(step){
    case fruitStep_RIPENING: // If the fruit is ripening
      if(time > 400) step = fruitStep_FALLING_INSIDE_LEAVES;
      else time++;
    break;
    case fruitStep_FALLING_INSIDE_LEAVES: // If the fruit is falling inside leaves
      // If the pixel below is gaseous
      if(y < l.getHeight()-1 && l.pixelPhysicalStateVector[l.p[x][y+1].type] == pixelPhysicalState_GASEOUS){
        swap(l.p[x][y], pixelUnderUs); std::swap(l.p[x][y].group, pixelUnderUs.group);
        l.p[x][y+1] = pixelUnderUs; pixelUnderUs.entity = 0;
        l.p[x][y+1].group = 0;
        step = fruitStep_JUST_FALLING; // We're just falling now, since we're out of the tree
        if(dyingStep == fruitDyingStep_NOTHING) dyingStep = fruitDyingStep_LANDING_COULD_BEGIN_DYING; // We try to be landing, which can be changed by the isGoingToFall callback
      }
      // Else, if the pixel below is LEAVES
      else if(y < l.getHeight()-1 && l.p[x][y+1].type == pixelType_LEAVES){
        swap(l.p[x][y], pixelUnderUs); std::swap(l.p[x][y].group, pixelUnderUs.group);
        swap(pixelUnderUs, l.p[x][y+1]); std::swap(pixelUnderUs.group, l.p[x][y+1].group);
      }
      // Else, we didn't felt inside leaves or out of leaves, that means we're lading, we try to be landing, which can be changed by the isGoingToFall callback
      else{
        if(dyingStep == fruitDyingStep_NOTHING) dyingStep = fruitDyingStep_LANDING_COULD_BEGIN_DYING;
      }
    break;
    case fruitStep_JUST_FALLING: // If we're just falling, we try to be landing next time, which can be changed by the isGoingToFall callback
      if(dyingStep == fruitDyingStep_NOTHING) dyingStep = fruitDyingStep_LANDING_COULD_BEGIN_DYING;
    break;
  }
  
  return false;
}
