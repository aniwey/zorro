#ifndef HPP_PIXEL
#define HPP_PIXEL

#include <algorithm>
#include <iostream>

#include <SFML/Graphics/Color.hpp>

#include "Entity.hpp"
#include "pixelType.hpp"
#include "random.hpp"

class Pixel{
  public:
    Pixel();
    ~Pixel();
    
    friend void swap(Pixel& a, Pixel& b){
      std::swap(a.type, b.type);
      std::swap(a.color, b.color);
      std::swap(a.group, b.group);
      if(a.entity != 0 || b.entity != 0) std::swap(a.entity, b.entity);
    }
    
    // Pixel creation
    void create(pixelType, Land&, int, int);
    void addEntity(Land&, int, int);
    void setColorBasedOnType();
    void destroyEntity();
    
    // Other functions
    void setColor(int, int, int);
    void setGroup(int);
    
    // Variables
    pixelType type; // Type of the pixel
    sf::Color color; // Color of the pixel
    int group; // Group of the pixel (optional), if =0, the pixel has no group
    Entity* entity; // Pointer towards the entity the pixel is attached to (optional)
  
  private:
    // Private functions used by create() method
    bool createEntity(pixelType, Land&, int, int); // Return true if an entity was created
};

#endif
