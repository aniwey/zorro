#include "Pixel.hpp"

#include <iostream>

#include "Fruit.hpp"
#include "Seed.hpp"

Pixel::Pixel(): color(sf::Color(0, 0, 0)), group(0), entity(0){
  
}

Pixel::~Pixel(){
  destroyEntity();
}

void Pixel::create(pixelType _type, Land& l, int x, int y){
  destroyEntity();
  if(!createEntity(_type, l, x, y)){ // If no entity is created
    // Then it's our duty to set attributes
    type = _type;
    setColorBasedOnType();
    setGroup(0);
  }
}

bool Pixel::createEntity(pixelType _type, Land& l, int x, int y){
  // Potentially add entity
  switch(_type){
    case pixelType_SEED:
      entity = new Seed(l, x, y);
      return true;
    break;
    case pixelType_FRUIT:
      entity = new Fruit(l, x, y);
      return true;
    break;
    default:
      // No entity for this kind of pixel
      return false;
    break;
  }
}

void Pixel::destroyEntity(){
  if(entity != 0){
    delete entity;
    entity = 0;
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
    default:
      setColor(0, 0, 0);
    break;
  }
}

void Pixel::setGroup(int _group){
  group = _group;
}
