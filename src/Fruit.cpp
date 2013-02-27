#include "Land.hpp"

#include "Fruit.hpp"

Fruit::Fruit(Land& l, int x, int y): Entity(l.frame_id, x, y){
  // Get color of the pixel that's under us
  leavesColorUnderUs = l.p[pixelX][pixelY].color;
  
  // Set type
  l.p[pixelX][pixelY].type = pixelType_FRUIT;
  
  // Set color
  l.p[pixelX][pixelY].setColorBasedOnType();
  
  // Set group : we keep the group of the pixel under us
  
  // Set feltAtThisFrame : we keep the value from the pixel under us
    
  // Set private parameters
  step = fruitStep_RIPENING;
  time = 0;
  dyingStep = fruitDyingStep_NOTHING;
  dyingTime = 0;
}

Fruit::~Fruit(){
  
}

bool Fruit::loop(Land& l){
  last_frame_id = l.frame_id;
  
  // If the pixel where the fruit is just felt
  if(l.p[pixelX][pixelY].feltAtThisFrame == l.frame_id){
    // If we were landing and we could begin dying, then we're not ladning anymore
    if(dyingStep == fruitDyingStep_LANDING_COULD_BEGIN_DYING) dyingStep = fruitDyingStep_NOTHING;
  }
  
  // Handling of the dying step
  switch(dyingStep){
    // If we're not landing, not dying, anything
    case fruitDyingStep_NOTHING:
      dyingTime = 0; // We reset the timer so that the next time we're landing we have to wait a lot before beginning dying
    break;
    // If we're landing and could begin dying
    case fruitDyingStep_LANDING_COULD_BEGIN_DYING:
      if(dyingTime > 120){
        if(l.howManyPixelsOfThisTypeInThisRectangle(pixelType_FRUIT, pixelX-2, pixelY-2, pixelX+2, pixelY+2) > 7 || l.howManyPixelsOfThisTypeInThisRectangle(pixelType_FRUIT, pixelX, pixelY-2, pixelX, pixelY+2) > 3){
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
        l.p[pixelX][pixelY].color = getMidColor(sf::Color(237, 123, 0), sf::Color(107, 55, 0), 1-(float)dyingTime/60);
        l.notifyForUpdatingThisPixel(pixelX, pixelY);
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
      // If the pixel below is non solid and none and we're not at the bottom of the map
      if(pixelY < l.height-1 && l.pixelForegroundPhysicalStateVector[l.p[pixelX][pixelY+1].fType] != pixelForegroundPhysicalState_SOLID && l.p[pixelX][pixelY+1].type == pixelType_NONE){
        l.p[pixelX][pixelY+1] = l.p[pixelX][pixelY]; // We go down
        l.p[pixelX][pixelY].type = pixelType_LEAVES; l.p[pixelX][pixelY].resetEntityPointer(); l.p[pixelX][pixelY].color = leavesColorUnderUs; // We set the leaves pixel under us
        l.p[pixelX][pixelY+1].group = 0; // We have no group now
        step = fruitStep_JUST_FALLING; // We're just falling now, since we're out of the tree
        if(dyingStep == fruitDyingStep_NOTHING) dyingStep = fruitDyingStep_LANDING_COULD_BEGIN_DYING; // We try to be landing, which can be changed by the isGoingToFall callback
        l.p[pixelX][pixelY+1].youJustMovedTo(pixelX, pixelY+1); // We notify the pixel
        l.notifyForUpdatingThisRectangle(pixelX-1, pixelY-2, pixelX+1, pixelY+1); // We notify
      }
      // Else, if the pixel below is LEAVES
      else if(pixelY < l.height-1 && l.p[pixelX][pixelY+1].type == pixelType_LEAVES){
        Group* group = l.p[pixelX][pixelY+1].group; // We save the group of the leaves pixel where we are going
        sf::Color color = l.p[pixelX][pixelY+1].color; // We save the color of the leaves pixel where we are going
        l.p[pixelX][pixelY+1] = l.p[pixelX][pixelY]; // We go down
        l.p[pixelX][pixelY+1].group = group; // We take the group of the leaves pixel where we are
        l.p[pixelX][pixelY].type = pixelType_LEAVES; l.p[pixelX][pixelY].resetEntityPointer(); l.p[pixelX][pixelY].color = leavesColorUnderUs; // We set the leaves pixel under us
        leavesColorUnderUs = color; // We save the color
        l.p[pixelX][pixelY+1].youJustMovedTo(pixelX, pixelY+1); // We notify the pixel
        l.notifyForUpdatingThisRectangle(pixelX-1, pixelY-2, pixelX+1, pixelY+1); // We notify
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
