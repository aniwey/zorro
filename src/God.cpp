#include "God.hpp"

God::God(){
  pixelTypeSelected = pixelType_DIRT;
  pixelAddingDiameter = 10;
}

void God::addThings(Land& l, int x, int y, bool justClicked, int landCursorMovementX, int landCursorMovementY){
  int diameter = getAddingDiameterDependingOnPixelTypeSelected();
  
  // If the pixel type selected can be added massively
  if(pixelTypeSelected != pixelType_SEED){
    // If the mouse didn't move this loop
    if(landCursorMovementX == 0 && landCursorMovementY == 0)
      l.writePixelRectangle(x - diameter/2, y - diameter/2, diameter, diameter, pixelTypeSelected);
    // Else, the mouse moved this loop
    else{
      l.writeEverythingBetweenTwoOrientedIdenticalRectangles(x - diameter/2, y - diameter/2, (x-landCursorMovementX) - diameter/2, (y-landCursorMovementY) - diameter/2, diameter, pixelTypeSelected);
    }
  }
  // Else, it can't be added massively, but it can if we just clicked
  else if(justClicked){
    l.writeSinglePixel(x, y, pixelTypeSelected);
  }
}

void God::movedMouseWheel(int delta){
  pixelAddingDiameter += delta;
  if(pixelAddingDiameter < PIXELADDINGDIAMETER_MIN) pixelAddingDiameter = PIXELADDINGDIAMETER_MIN;
  if(pixelAddingDiameter > PIXELADDINGDIAMETER_MAX) pixelAddingDiameter = PIXELADDINGDIAMETER_MAX;
}

void God::pressedALetter(sf::Keyboard::Key letter){
  switch(letter){
    case sf::Keyboard::A:
      pixelTypeSelected = pixelType_AIR;
    break;
    case sf::Keyboard::D:
      pixelTypeSelected = pixelType_DIRT;
    break;
    case sf::Keyboard::S:
      pixelTypeSelected = pixelType_STONE;
    break;
    case sf::Keyboard::B:
      pixelTypeSelected = pixelType_SEED;
    break;
    default: break;
  }
}

int God::getAddingDiameterDependingOnPixelTypeSelected(){
  if(pixelTypeSelected == pixelType_SEED){
    return 1;
  }
  else return pixelAddingDiameter;
}
