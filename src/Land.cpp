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
  
  // Memory allocation of atuNotif
  atuNotif.resize(width);
  
  // At first, we're gonna redraw everything
  redrawEverything = true;
}

void Land::newGame(int x, int y){
  init(x, y);
  
  // We fill the land with air
  writePixelRectangle(0, 0, width, height, pixelType_AIR);
  
  // First frame is 0
  frame_id = 0;
}

bool Land::thisPixelExists(int x, int y){
  // If the pixel given is out of the land, return false
  if(x >= width || y >= height || x < 0 || y < 0)
    return false;
  // Else, return true;
  return true;
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

void Land::registerEntity(boost::shared_ptr<Entity> ent){
  entities.push_back(boost::weak_ptr<Entity>(ent));
}
