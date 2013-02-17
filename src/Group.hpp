#ifndef HPP_GROUP
#define HPP_GROUP

#include <boost/serialization/list.hpp>
#include <boost/serialization/utility.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "GroupPixel.hpp"

class Land;

class Group{
  public:
    Group();
  
    Group* registerPixel(int, int);
    Group* registerPixel(boost::shared_ptr<GroupPixel>);
    Group* unregisterPixel(int, int, bool);
    
    void checkForSplitting(Land&);
    void changePixelGroupRecursively(Land&, int, int, Group*);
    
    bool hasPixels();
    
    bool resolveDependencies(Land&);
    
    bool canFall();
    
    std::list<boost::shared_ptr<GroupPixel> > pixels;
    bool canFallBool;
    bool checked; // Used for example by the dependencies resolving function
    
    boost::shared_ptr<GroupPixel> getGroupPixelSharedPtr(int, int);
    
    void removePixel(int, int);
  
  private:
    // Serialization
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar & pixels;
      ar & canFallBool;
    }
};

#endif
