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
    void writePixelRectangle(int x, int y, int w, int h, pixelType type);
    void writeEverythingBetweenTwoOrientedIdenticalSquares(int x1, int y1, int x2, int y2, int size, pixelType type);
    void writePixelLine(int x1, int y1, int x2, int y2, pixelType type);
    void writeSinglePixel(int x, int y, pixelType type);
    
    // Pixel array
    std::vector<std::vector<Pixel> > p;
    
    // Pixel groups stuff
    std::list<Group> g; // Groups list
    std::list<Group*> gtu; // Groups to update
    Group* getPixelGroup();
    
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
    std::vector<pixelPhysicalState> pixelPhysicalStateVector;
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
    
    // Entities
    std::list<boost::weak_ptr<Entity> > entities;
    void registerEntity(boost::shared_ptr<Entity>);
    
    // Bool to redraw everything
    bool redrawEverything;

  private:
    // Loop steps
    void loopDirt();
    void loopGravity();
    void loopEntities();
    void loopPixels();
      // loopPixels functions
      bool tryToMakeFall(int, int);
      bool tryToMakeFallAlongWithPixelsBelow(int, int);
      void makeFall(int, int);
};

#endif
