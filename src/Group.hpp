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
    // Constructor
    Group();
  
    // Pixel registering and unregistering
    
      // Register a pixel using its coordinates
      // This will allocate a place in memory for a new GroupPixel object and add a boost::shared_ptr to it in our pixels list
      Group* registerPixel(int, int);
      
      // Register a pixel using a shared_ptr pointing to it
      // This doesn't allocate a new place in memory, this is useful if we just want to transfer a GroupPixel from one group to another
      Group* registerPixel(boost::shared_ptr<GroupPixel>); // Register a pixel
      
      // Unregister a pixel from the group
      // If removeTheDependencyLink is true, the unregistration will tell the group pixel we depend on that no pixel depends on it anymore, and will tell the group pixel which depends on us that it doesn't depend on any pixel anymore
      // When moving a GroupPixel from one group to another, we should not remove the dependency link
      Group* unregisterPixel(int x, int y, bool removeTheDependencyLink);
    
    // Huge operations
    void checkForSplitting(Land&); // Split all pixels in the pixels list in different groups if needed
    void changePixelGroupRecursively(Land&, int, int, Group*); // Used by checkForSplitting function
    bool resolveDependencies(Land&); // Resolve the group's dependency chain : will set canFallBool to the correct state depending on the chain
    
    // Simple tests
    bool hasPixels(); // Return true if the pixels list isn't empty
    bool canFall(); // Return true if canFallBool is true
    
    // Pixels list functions
    boost::shared_ptr<GroupPixel> getGroupPixelSharedPtr(int, int); // Return a shared_ptr to the group pixel corresponding to the coordinates
    void removePixel(int, int); // Remove a pixel from the pixels list
    
    // Attributes
    std::list<boost::shared_ptr<GroupPixel> > pixels; // Pixels which belong to the group
    bool canFallBool; // Used to know if a group can fall or not
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
