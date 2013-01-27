#include "Interface.hpp"

Interface::Interface(){
  
}

Interface::~Interface(){
  
}

void Interface::init(Screen& s, ConfigData& textStrings){
  // Create toolbars
  mainToolbar.init(0, 0, 0, -72, s.getWidth(), 72, 0);
    mainToolbar.addButton(s.im, "dirt", textStrings.getIterator("menu_dirt"), 0, 0, pixelType_DIRT, true);
    mainToolbar.addButton(s.im, "bush", textStrings.getIterator("menu_seed"), -120, 0, pixelType_SEED, true);
    mainToolbar.addButton(s.im, "air", textStrings.getIterator("menu_air"), -60, 0, pixelType_AIR, true);
    mainToolbar.addButton(s.im, "stone", textStrings.getIterator("menu_stone"), 60, 0, pixelType_STONE, true);
}

void Interface::loop(){
  mainToolbar.loop();
}

void Interface::leftButtonPressed(God& g, sf::Vector2i mousePosition){
  mainToolbar.leftButtonPressed(g, mousePosition);
}

void Interface::mouseMoved(sf::Vector2i mousePosition){
  mainToolbar.mouseMoved(mousePosition);
}

bool Interface::isCursorUsed(sf::Vector2i mousePosition){
  if(mainToolbar.isCursorUsed(mousePosition))
    return true;
    
  return false;
}
