#include "Land.hpp"

void Land::loopWater(){
  for(unsigned int i = 0; i < atu.size(); ++i){
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){
        // If this pixel felt at least one frame ago and is a water pixel
        if(p[i][j].feltAtThisFrame < frame_id && p[i][j].type == pixelType_WATER){
          // If there's also a water pixel above us
          if(p[i][j-1].type == pixelType_WATER){
            // If we can push ourselves from here
            if(tryToPushThisWaterPixel(i, j)){
              // We move all the water pixels above
              int l = j-1; while(p[i][l].type == pixelType_WATER && l >= 0) { swap(p[i][l], p[i][l+1]); l--; }
              // We notify
              notifyForUpdatingThisRectangle(i-1, l-1, i+1, j+1);
              j = l;
            }
          }
        }
      }
    }
  }
}

bool Land::tryToPushThisWaterPixel(int x, int y){
  bool canPushLeft = true, canPushRight = true;
  
  for(int i = 1; i < width/2; ++i){
    // Try to push to the left
    if(canPushLeft && x-i >= 0){
      switch(pixelPhysicalStateVector[p[x-i][y].type]){
        case pixelPhysicalState_GASEOUS:
          swap(p[x][y], p[x-i][y]);
          notifyForUpdatingAroundThisPixel(x, y);
          notifyForUpdatingAroundThisPixel(x-i, y);
          return true;
        break;
        case pixelPhysicalState_SOLID:
          canPushLeft = false;
        break;
        default: break;
      }
    }
    // Try to push to the right
    if(canPushRight && x+i <= width-1){
      switch(pixelPhysicalStateVector[p[x+i][y].type]){
        case pixelPhysicalState_GASEOUS:
          swap(p[x][y], p[x+i][y]);
          notifyForUpdatingAroundThisPixel(x, y);
          notifyForUpdatingAroundThisPixel(x+i, y);
          return true;
        break;
        case pixelPhysicalState_SOLID:
          canPushRight = false;
        break;
        default: break;
      }
    }
  }
  
  return false;
}
