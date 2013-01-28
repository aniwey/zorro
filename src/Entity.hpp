#ifndef HPP_ENTITY
#define HPP_ENTITY

#include <boost/serialization/access.hpp>

class Land;

class Entity{
  public:
    Entity();
    Entity(int frame_id);
    virtual ~Entity();
    
    virtual bool loop(Land&, int, int) = 0; // Function called each loop, return true if the entity should be destroyed
    virtual void isGoingToFall(){ } // Callback called when the pixel containing the entity fell because of an external function
    
    int last_frame_id; // Contains the identifier of the last frame when the entity looped
    
  private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &, const unsigned int){}
};

#endif
