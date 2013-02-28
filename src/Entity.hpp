#ifndef HPP_ENTITY
#define HPP_ENTITY

#include <boost/serialization/access.hpp>

class Land;

class Entity{
  public:
    Entity();
    Entity(int, int);
    virtual ~Entity();
    
    virtual bool loop(Land&) = 0; // Function called each loop, return true if the entity should be destroyed
    
    int pixelX, pixelY;
    
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar & pixelX;
      ar & pixelY;  
    }
};

#endif
