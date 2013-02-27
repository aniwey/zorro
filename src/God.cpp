#include "God.hpp"

God::God(){
  pixelTypeSelected = pixelType_INVALID;
  pixelFTypeSelected = pixelForegroundType_INVALID;
  pixelAddingDiameter = 10;
}

void God::addThings(Land& l, int oldLandCursorX, int oldLandCursorY, int newLandCursorX, int newLandCursorY, bool activeAddingEvent){
  int diameter = getAddingDiameterDependingOnPixelTypeSelected();

  // If the pixel type selected can be added massively
  if(pixelTypeSelected != pixelType_SEED){
    // If the land cursor didn't move this loop
    if(oldLandCursorX == newLandCursorX && oldLandCursorY == newLandCursorY)
      l.writePixelRectangle(oldLandCursorX - diameter/2, oldLandCursorY - diameter/2, diameter, diameter, pixelTypeSelected, pixelFTypeSelected);
    // Else, the land cursor moved this loop
    else{
      l.writeEverythingBetweenTwoOrientedIdenticalSquares(oldLandCursorX - diameter/2, oldLandCursorY - diameter/2, newLandCursorX - diameter/2, newLandCursorY - diameter/2, diameter, pixelTypeSelected, pixelFTypeSelected);
    }
  }
  // Else, it can't be added massively, but it can if we just did an active adding event
  else if(activeAddingEvent){
    l.writeSinglePixel(oldLandCursorX, oldLandCursorY, pixelTypeSelected, pixelFTypeSelected);
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
      pixelTypeSelected = pixelType_NONE;
      pixelFTypeSelected = pixelForegroundType_AIR;
    break;
    case sf::Keyboard::D:
      pixelTypeSelected = pixelType_DIRT;
      pixelFTypeSelected = pixelForegroundType_INVALID;
    break;
    case sf::Keyboard::S:
      pixelTypeSelected = pixelType_NONE;
      pixelFTypeSelected = pixelForegroundType_STONE;
    break;
    case sf::Keyboard::B:
      pixelTypeSelected = pixelType_SEED;
      pixelFTypeSelected = pixelForegroundType_INVALID;
    break;
    case sf::Keyboard::W:
      pixelTypeSelected = pixelType_NONE;
      pixelFTypeSelected = pixelForegroundType_WATER;
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
