#include "Land.hpp"

Land::Land(){
  // Set pixelGravityVector (constants)
  pixelGravityVector.resize(pixelType_TOTAL);
  pixelGravityVector[pixelType_NONE] = pixelGravity_NOT_RELEVANT;
  pixelGravityVector[pixelType_DIRT] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_SEED] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_LEAVES] = pixelGravity_MAY_FALL;
  pixelGravityVector[pixelType_FRUIT] = pixelGravity_MAY_FALL;
  
  // Set pixelForegroundPhysicalStateVector (constants)
  pixelForegroundPhysicalStateVector.resize(pixelForegroundType_TOTAL);
  pixelForegroundPhysicalStateVector[pixelForegroundType_AIR] = pixelForegroundPhysicalState_GASEOUS;
  pixelForegroundPhysicalStateVector[pixelForegroundType_STONE] = pixelForegroundPhysicalState_SOLID;
  pixelForegroundPhysicalStateVector[pixelForegroundType_WATER] = pixelForegroundPhysicalState_LIQUID;
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
  writePixelRectangle(0, 0, width, height, pixelType_NONE, pixelForegroundType_AIR);
  
  /*
  for(int i = 0; i < width; ++i){ // Iteration over the columns
    for(int j = 0; j < height; ++j){ // Iteration over the lines
      switch(randomBetween(0, 6)){
        case 0:
          // still air
        break;
        case 1:
          writeSinglePixel(i, j, pixelType_DIRT);
        break;
        case 2:
          writeSinglePixel(i, j, pixelType_NONE, pixelForegroundType_STONE);
        break;
        case 3:
          writeSinglePixel(i, j, pixelType_DIRT);
          writeSinglePixel(i, j, pixelType_SEED);
        break;
        case 4:
          writeSinglePixel(i, j, pixelType_DIRT, pixelForegroundType_WATER);
        break;
        case 5:
          writeSinglePixel(i, j, pixelType_DIRT, pixelForegroundType_WATER);
          writeSinglePixel(i, j, pixelType_SEED, pixelForegroundType_WATER);
        break;
      }
    }
  }*/
  
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
