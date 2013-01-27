#ifndef HPP_INTERFACE
#define HPP_INTERFACE

#include "ConfigData.hpp"
#include "Land.hpp"
#include "Screen.hpp"
#include "Toolbar.hpp"

class Screen;

class Interface{
  public:
    Interface();
    ~Interface();
    
    void init(Screen&, ConfigData& textStrings);
    void loop();
    
    bool isCursorUsed(sf::Vector2i);
    
    // Functions called when some events occur to notify the interface
    void leftButtonPressed(God&, sf::Vector2i);
    void mouseMoved(sf::Vector2i);
    
    Toolbar mainToolbar;
    
  private:
    
};

#endif
