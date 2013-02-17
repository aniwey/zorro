#ifndef HPP_GROUP_PIXEL
#define HPP_GROUP_PIXEL

#include <boost/weak_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

enum GroupDependencyType{
  GroupDependencyType_NOTHING,
  GroupDependencyType_CANT_FALL_PIXEL,
  GroupDependencyType_GROUP,
  GroupDependencyType_BOTTOM_OF_THE_MAP
};

class GroupPixel{
  public:
    GroupPixel();
    GroupPixel(int, int);
  
    // Our position
    int x, y;
  
    // Our dependency
    GroupDependencyType depType;
    boost::weak_ptr<GroupPixel> pixelWeDependOn;
  
    // The pixel which depends on us
    bool aPixelDependsOnUs;
    boost::weak_ptr<GroupPixel> pixelWhichDependsOnUs;
    
    // ==
    bool operator == (const GroupPixel& gp) const{
      return ((x == gp.x) && (y == gp.y));
    }
    
    // !=
    bool operator != (const GroupPixel& gp) const{
      return !((x == gp.x) && (y == gp.y));
    }
  
  private:
    // Serialization
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int){
      ar & x;
      ar & y;
      ar & depType;
      ar & pixelWeDependOn;
      ar & pixelWhichDependsOnUs;
    }
};


#endif
