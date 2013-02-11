#ifndef HPP_GOD
#define HPP_GOD

#include <SFML/Graphics.hpp>

#include "Land.hpp"

#define PIXELADDINGDIAMETER_MIN 1
#define PIXELADDINGDIAMETER_MAX 100

class God{
  public:
    God();
    
    // Functions reacting to various events, called by Screen methods
    void addThings(Land&, int, int, int, int, bool);
    void movedMouseWheel(int);
    void pressedALetter(sf::Keyboard::Key);
    
    // "get" functions
    int getAddingDiameterDependingOnPixelTypeSelected();
    
    pixelType pixelTypeSelected;
    
  private:
    int pixelAddingDiameter;
};

#endif
