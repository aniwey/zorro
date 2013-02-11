#include "ConfigData.hpp"
#include "God.hpp"
#include "Interface.hpp"
#include "Land.hpp"
#include "random.hpp"
#include "Screen.hpp"

#include <iostream>

int main(void){
  // Create ConfigData objects and load configuration files
  ConfigData mainConfig;
  mainConfig.readFromFile("./config/cfg.txt");
  ConfigData textStrings;
  textStrings.readFromFile("./config/localization/" + mainConfig.getString("language") + ".txt");
  
  // Create the screen object
  Screen s;
  s.init(mainConfig);
  
  // Create the god object
  God g;
  
  // Create the land object
  Land l;
  l.newGame(s.getWidth()/4 - 20, s.getHeight()/4 - 20);
  s.adaptToLand(l);
  
  // Create the interface object
  Interface i;
  i.init(s, textStrings);
  
  // Draw a first time
  s.draw(l, g, i);
  
  // Other stuff
  bool exit = 0;
  initRandom();
  
  while(!exit){
    std::cout << l.entities.size() << std::endl;
    l.switchAtuAndAtuNotif();
    l.loop();
    s.handleEvents(l, g, i, exit);
    i.loop();
    s.draw(l, g, i);
  }
  
  return 0;
}
