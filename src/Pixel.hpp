#ifndef HPP_PIXEL
#define HPP_PIXEL

#include <algorithm>
#include <iostream>

#include <boost/serialization/access.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <boost/shared_ptr.hpp>

#include <SFML/Graphics/Color.hpp>

#include "Entity.hpp"
#include "Fruit.hpp"
#include "Group.hpp"
#include "pixelType.hpp"
#include "random.hpp"
#include "Seed.hpp"
#include "SFML_serialization_functions.hpp"

class Pixel{
  public:
    Pixel();
    ~Pixel();
    
    friend void swap(Pixel& a, Pixel& b){
      std::swap(a.type, b.type);
      std::swap(a.color, b.color);
      std::swap(a.group, b.group);
      std::swap(a.feltAtThisFrame, b.feltAtThisFrame);
      std::swap(a.entity, b.entity);
    }
    
    // Pixel creation
    void create(Land&, int, int, pixelType, pixelForegroundType = pixelForegroundType_INVALID);
    void addEntity(Land&, int, int);
    void setColorBasedOnType();
    
    void resetEntityPointer();
    
    // Other functions
    void setColor(int, int, int);
    
    // Callbacks
    void youJustMovedTo(int, int);
    
    // Variables
    pixelType type; // Type of the pixel
    pixelForegroundType fType; // Foreground type of the pixel
    sf::Color color; // Color of the pixel
    Group* group; // Group of the pixel (optional), if = 0, the pixel has no group
    boost::shared_ptr<Entity> entity; // Pointer towards the entity the pixel is attached to (optional)
    int feltAtThisFrame;
  
  private:
    // Serialization
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar.template register_type<Seed>();
      ar.template register_type<Fruit>();
      ar & type;
      ar & fType;
      ar & color;
      ar & group;
      ar & entity;
      ar & feltAtThisFrame;
    }
  
    // Private functions used by create() method
    bool createEntity(pixelType, Land&, int, int); // Return true if an entity was created
};

#endif
