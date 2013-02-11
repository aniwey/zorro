#ifndef HPP_GROUP
#define HPP_GROUP

#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>

typedef std::pair<int, int> GroupPixel;

enum GroupDependencyType{
  GroupDependencyType_NOTHING,
  GroupDependencyType_CANT_FALL_PIXEL,
  GroupDependencyType_GROUP,
  GroupDependencyType_BOTTOM_OF_THE_MAP
};

class Group;

struct GroupDependency{
  // Our dependency
  GroupDependencyType type;
  GroupPixel pixelWeDependOn;
  
  // The pixel which depends on us
  bool aPixelDependsOnUs;
  GroupPixel pixelWhichDependsOnUs;
  
  template<class Archive>
  void serialize(Archive & ar, const unsigned int){
    ar & type;
    ar & pixelWeDependOn;
    ar & aPixelDependsOnUs;
    ar & pixelWhichDependsOnUs;
  }
};

class Land;

class Group{
  public:
    Group();
  
    Group* registerPixel(int, int);
    Group* unregisterPixel(Land&, int, int, bool);
    
    void checkForSplitting(Land&);
    void changePixelGroupRecursively(Land&, int, int, Group*);
    
    bool hasPixels();
    
    bool resolveDependencies(Land&);
    
    bool canFall();
    
    std::map<GroupPixel, GroupDependency> pixels;
    bool canFallBool;
    bool checked; // Used for example by the dependencies resolving function
  
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
