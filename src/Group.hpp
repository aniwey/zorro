#ifndef HPP_GROUP
#define HPP_GROUP

#include <boost/serialization/list.hpp>
#include <boost/serialization/utility.hpp>

#include <boost/weak_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/serialization/shared_ptr.hpp>

enum GroupDependencyType{
  GroupDependencyType_NOTHING,
  GroupDependencyType_CANT_FALL_PIXEL,
  GroupDependencyType_GROUP,
  GroupDependencyType_BOTTOM_OF_THE_MAP
};

class Group;

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
