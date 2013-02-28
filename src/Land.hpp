#ifndef HPP_LAND
#define HPP_LAND

#include <iostream>
#include <list>
#include <boost/serialization/list.hpp>

#include <boost/weak_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

#include <SFML/Graphics.hpp>

#include "Pixel.hpp"
#include "pixelType.hpp"

class Pixel;

typedef enum{
  canFallState_YES,
  canFallState_YES_WITH_DEPS,
  canFallState_NO
}canFallState;

class Land{
  public:
    Land();
    
    // Initialization
    void init(int, int);
    void newGame(int, int);
    
    // Loop
    void loop();
    
    // Writing pixels to the land
    void writePixelRectangle(int x, int y, int w, int h, pixelType type, pixelForegroundType fType = pixelForegroundType_INVALID, bool changeFTypeOnlyIfPreviousIsSolid = false);
    void writeEverythingBetweenTwoOrientedIdenticalSquares(int x1, int y1, int x2, int y2, int size, pixelType type, pixelForegroundType fType = pixelForegroundType_INVALID, bool changeFTypeOnlyIfPreviousIsSolid = false);
    void writePixelLine(int x1, int y1, int x2, int y2, pixelType type, pixelForegroundType fType = pixelForegroundType_INVALID, bool changeFTypeOnlyIfPreviousIsSolid = false);
    void writeSinglePixel(int x, int y, pixelType type, pixelForegroundType fType = pixelForegroundType_INVALID, bool changeFTypeOnlyIfPreviousIsSolid = false);
    
    // Pixels
    std::vector<std::vector<Pixel> > p; // Pixels array
    
    // Groups
    std::list<Group> g; // Groups list
    Group* getPixelGroup(); // Create a new group and return a pointer to it
    
    // Entities
    std::list<boost::weak_ptr<Entity> > entities; // List of entities (weak_ptr, see boost::weak_ptr)
    void registerEntity(boost::shared_ptr<Entity>); // Register a new entities to the entities list using a shared_ptr to an entity, the shared_ptr probably being contained in a pixel
    
    // Updating
    std::vector<std::list<std::pair<int, int> > > atu; // Areas to update used during the pixels loop
    std::vector<std::list<std::pair<int, int> > > atuNotif; // Areas to update used for notifications
    
    void switchAtuAndAtuNotif();
    
    void notifyForUpdatingThisRectangle(int, int, int, int);
    void notifyForUpdatingAroundThisPixel(int, int);
    void notifyForUpdatingThisPixel(int, int);
    void notifyEverything();
    
    // Various tests on pixels
    std::vector<pixelGravity> pixelGravityVector;
    std::vector<pixelForegroundPhysicalState> pixelForegroundPhysicalStateVector;
    bool aPixelOfThisGroupIsAdjacentToThisOne(int x, int y, Group* group);
    bool thisPixelExists(int x, int y);
    int howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType type, Group* group, int x1, int y1, int x2, int y2);
    int howManyPixelsOfThisTypeInThisRectangle(pixelType type, int x1, int y1, int x2, int y2);
    
    // Frame id
    int frame_id;
    
    // Size of the pixel arrays
    int width, height;
    
    // Serialization
    void saveGame(std::string);
    void loadGame(std::string);
    
    // Bool to redraw everything
    bool redrawEverything;

  private:
    // loop() steps
    void loopPixels();
      // loopPixels() functions
      void checkForGroupsSplittingAndUpdateDependencies();
      void resolveDependenciesBetweenGroups();
      void applyGravityOnAtu();
      void applyGravityOnGroupsOutsideAtu();
      void loopDirt();
      void loopWater();
      void destroyEmptyGroups();
    void loopEntities();
    
    // Falling functions used by loopPixels functions
    bool tryToMakeFall(int, int);
    bool tryToMakeFallAlongWithPixelsBelow(int, int);
    void makeFall(int, int);
    
    // Water functions used by loopWater()
    bool tryToPushThisWaterPixel(int, int);
};

#endif
