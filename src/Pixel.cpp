#include "Pixel.hpp"

#include <iostream>

#include "Fruit.hpp"
#include "Land.hpp"
#include "Seed.hpp"

Pixel::Pixel(): color(sf::Color(0, 0, 0)), group(0), entity(), feltAtThisFrame(0){
  
}

Pixel::~Pixel(){
  resetEntityPointer();
}

void Pixel::create(pixelType _type, Land& l, int x, int y){
  // If we had an entity, we stop pointing to our entity
  resetEntityPointer();
  
  if(!createEntity(_type, l, x, y)){ // If no entity is created
    // Then it's our duty to set attributes
    type = _type;
    setColorBasedOnType();
    if(group != 0) group = group->unregisterPixel(x, y, true);
    feltAtThisFrame = -1;
  }
}

void Pixel::resetEntityPointer(){
  entity = boost::shared_ptr<Entity>();
}

bool Pixel::createEntity(pixelType _type, Land& l, int x, int y){
  // Potentially add entity
  switch(_type){
    case pixelType_SEED:
      entity = boost::shared_ptr<Entity>(new Seed(l, x, y));
      l.registerEntity(entity);
      return true;
    break;
    case pixelType_FRUIT:
      entity = boost::shared_ptr<Entity>(new Fruit(l, x, y));
      l.registerEntity(entity);
      return true;
    break;
    default:
      // No entity for this kind of pixel
      return false;
    break;
  }
}

void Pixel::youJustMovedTo(int x, int y){
  if(entity){
    entity->pixelX = x;
    entity->pixelY = y;
  }
}

void Pixel::setColor(int r, int g, int b){
  color = sf::Color(r, g, b);
}

void Pixel::setColorBasedOnType(){
  switch(type){
    case pixelType_AIR:
      setColor(255, 255, 255);
    break;
    case pixelType_DIRT:
      setColor(205, 31, 31);
    break;
    case pixelType_STONE:
      setColor(80, 75, 107);
    break;
    case pixelType_SEED:
      setColor(121, 0, 0);
    break;
    case pixelType_LEAVES:
      setColor(0, randomBetween(128, 255), 0);
    break;
    case pixelType_FRUIT:
      setColor(237, 123, 0);
    break;
    case pixelType_WATER:
      setColor(95, 101, 208);
    break;
    default:
      setColor(0, 0, 0);
    break;
  }
}
