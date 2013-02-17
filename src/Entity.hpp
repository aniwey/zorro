#ifndef HPP_ENTITY
#define HPP_ENTITY

#include <boost/serialization/access.hpp>

class Land;

class Entity{
  public:
    Entity();
    Entity(int, int, int);
    virtual ~Entity();
    
    virtual bool loop(Land&) = 0; // Function called each loop, return true if the entity should be destroyed
    
    int last_frame_id; // Contains the identifier of the last frame when the entity looped
    int pixelX, pixelY;
    
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar & last_frame_id;
      ar & pixelX;
      ar & pixelY;  
    }
};

#endif
