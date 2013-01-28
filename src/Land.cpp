#include "Land.hpp"

Land::Land(){
  // Set pixelGravityVector (constants)
  pixelGravityVector.resize(pixelType_TOTAL);
  pixelGravityVector[pixelType_AIR] = pixelGravity_NOT_RELEVANT;
  pixelGravityVector[pixelType_DIRT] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_STONE] = pixelGravity_CANT_FALL;
  pixelGravityVector[pixelType_SEED] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_LEAVES] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_FRUIT] = pixelGravity_MAY_FALL;
  
  // Set pixelPhysicalStateVector (constants)
  pixelPhysicalStateVector.resize(pixelType_TOTAL);
  pixelPhysicalStateVector[pixelType_AIR] = pixelPhysicalState_GASEOUS;
  pixelPhysicalStateVector[pixelType_DIRT] = pixelPhysicalState_SOLID;
  pixelPhysicalStateVector[pixelType_STONE] = pixelPhysicalState_SOLID;
  pixelPhysicalStateVector[pixelType_SEED] = pixelPhysicalState_SOLID;
  pixelPhysicalStateVector[pixelType_LEAVES] = pixelPhysicalState_SOLID;
  pixelPhysicalStateVector[pixelType_FRUIT] = pixelPhysicalState_SOLID;
}

void Land::init(int x, int y){
  // Set land's  width and height
  width = x;
  height = y;
  
  // Memory allocation of the pixel array
  try{
    p.resize(width);
    for(int i = 0; i < width; ++i){
      p[i].resize(height);
    }
  }
  catch(std::bad_alloc const&){
    std::cerr << "Memory allocation fail!" << std::endl;
  }
}

void Land::newGame(int x, int y){
  init(x, y);
  
  // We fill the land with air
  writePixelRectangle(0, 0, width, height, pixelType_AIR);
  
  // At first, pixel groups will be positive (but it doesn't matter anyway, it change each frame)
  pixelGroupsArePositive = true;
  lastPixelGroupGiven = 0;
  frame_id = 0;
}

void Land::switchPixelGroupsSign(){
  if(pixelGroupsArePositive){
    pixelGroupsArePositive = false;
    lastPixelGroupGiven = 0;
  }
  else{
    pixelGroupsArePositive = true;
    lastPixelGroupGiven = 0;
  }
}

int Land::getPixelGroup(){
  if(pixelGroupsArePositive) return ++lastPixelGroupGiven;
  return --lastPixelGroupGiven;
}

void Land::changePixelGroupRecursively(int x, int y, int oldGroup, int newGroup){
  // We change our group
  p[x][y].group = newGroup;
  
  // And we try to also change the group of adjacent pixels of our old group
  if(x < width-1 && p[x+1][y].group == oldGroup){
    changePixelGroupRecursively(x+1, y, oldGroup, newGroup);
  }
  if(x > 0 && p[x-1][y].group == oldGroup){
    changePixelGroupRecursively(x-1, y, oldGroup, newGroup);
  }
  if(y < height-1 && p[x][y+1].group == oldGroup){
    changePixelGroupRecursively(x, y+1, oldGroup, newGroup);
  }
  if(y > 0 && p[x][y-1].group == oldGroup){
    changePixelGroupRecursively(x, y-1, oldGroup, newGroup);
  }
}

bool Land::aPixelOfThisGroupIsAdjacentToThisOne(int x, int y, int group){
  if(x < width-1 && p[x+1][y].group == group)
    return true;

  if(x > 0 && p[x-1][y].group == group)
    return true;

  if(y < height-1 && p[x][y+1].group == group)
    return true;

  if(y > 0 && p[x][y-1].group == group)
    return true;

  return false;
}

bool Land::thisPixelExists(int x, int y){
  // If the pixel given is out of the land, return false
  if(x >= width || y >= height || x < 0 || y < 0)
    return false;
  // Else, return true;
  return true;
}

int Land::howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType type, int group, int x1, int y1, int x2, int y2){
  int count = 0;
  
  // We first correct x, y, w and h if they don't fit in the land
  if(x1 < 0) x1 = 0;
  if(y1 < 0) y1 = 0;
  if(x2 > width-1) x2 = width-1;
  if(y2 > height-1) y2 = height-1;
  
  // And we count pixels
  for(int i = x1; i <= x2; ++i){
    for(int j = y1; j <= y2; ++j){
      // If this pixel match the type and the group, we add 1 to count
      if(p[i][j].type == type && p[i][j].group == group)
        count++;
    }
  }
  
  return count; // We return the count of pixels
}

int Land::howManyPixelsOfThisTypeInThisRectangle(pixelType type, int x1, int y1, int x2, int y2){
  int count = 0;
  
  // We first correct x, y, w and h if they don't fit in the land
  if(x1 < 0) x1 = 0;
  if(y1 < 0) y1 = 0;
  if(x2 > width-1) x2 = width-1;
  if(y2 > height-1) y2 = height-1;
  
  // And we count pixels
  for(int i = x1; i <= x2; ++i){
    for(int j = y1; j <= y2; ++j){
      // If this pixel match the type and the group, we add 1 to count
      if(p[i][j].type == type)
        count++;
    }
  }
  
  return count; // We return the count of pixels
}
