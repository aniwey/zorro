#ifndef HPP_LAND
#define HPP_LAND

#include <iostream>
#include <list>
#include <vector>

#include <SFML/Graphics.hpp>

#include "Pixel.hpp"
#include "pixelType.hpp"

class Pixel;

typedef enum{
  canFallState_YES,
  canFallState_YES_WITH_DEPS,
  canFallState_NO
}canFallState;

typedef struct{
  canFallState canFall;
  std::list<int> deps;
  bool dependenciesResolved;
}groupGravityStruct;

class Land{
  public:
    Land();
    
    // Initialization
    void init(int, int);
    
    // Loop
    void loop();
    
    // Writing pixels to the land
    void writePixelRectangle(int x, int y, int w, int h, pixelType type);
    void writeEverythingBetweenTwoOrientedIdenticalRectangles(int x1, int y1, int x2, int y2, int size, pixelType type);
    void writePixelLine(int x1, int y1, int x2, int y2, pixelType type);
    void writeSinglePixel(int x, int y, pixelType type);
    
    // "get" functions
    int getWidth(){ return width; }
    int getHeight(){ return height; }
    
    // Pixel array
    std::vector<std::vector<Pixel> > p;
    
    // Pixel groups stuff
    bool pixelGroupsArePositive;
    int lastPixelGroupGiven;
    int getPixelGroup();
    void switchPixelGroupsSign();
    void changePixelGroupRecursively(int x, int y, int oldGroup, int newGroup);
    
    // Various tests on pixels
    std::vector<pixelGravity> pixelGravityVector;
    std::vector<pixelPhysicalState> pixelPhysicalStateVector;
    bool aPixelOfThisGroupIsAdjacentToThisOne(int x, int y, int group);
    bool thisPixelExists(int x, int y);
    int howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType type, int group, int x1, int y1, int x2, int y2);
    int howManyPixelsOfThisTypeInThisRectangle(pixelType type, int x1, int y1, int x2, int y2);
    
    // Frame id
    int frame_id;

  private:
    // Size of the pixel arrays
    int width, height;
    
    // Pixel groups handling
    std::vector<groupGravityStruct> ggs; // Used to handle pixel groups gravity, declared here to avoid allocating of new memory each loop
    bool resolveGroupGravityStructDependencies(int);
    
    // Loop steps
    void loopDirt();
    void loopGroups();
    void loopGroupsSplitting();
    void loopEntities();
};

#endif
