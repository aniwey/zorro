#include "Land.hpp"

void Land::loopWater(){
  for(unsigned int i = 0; i < atu.size(); ++i){
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){
        // If the foreground of this pixel is water, then we'll try to do something !
        if(p[i][j].fType == pixelForegroundType_WATER){
          // If we're not at the bottom of the map and the foreground of the pixel below is gaseous, we go down
          if(j < height-1 && pixelForegroundPhysicalStateVector[p[i][j+1].fType] == pixelForegroundPhysicalState_GASEOUS){
            std::swap(p[i][j].fType, p[i][j+1].fType);
            notifyForUpdatingThisRectangle(i-1, j-1, i+1, j+2); // We notify
          }
          // Else, we didn't make it fall, we try to work on the water's physics
          // If we're not at the top of the map and there's a water pixel above us
          else if(j > 0 && p[i][j-1].fType == pixelForegroundType_WATER){
            // If we can push ourselves from here
            if(tryToPushThisWaterPixel(i, j)){
              // We move all the water pixels above
              int l = j-1; while(p[i][l].fType == pixelForegroundType_WATER && l >= 0) { l--; }
              l++;
              // Here l is the last pixel with water foreground : we swap with j
              std::swap(p[i][j].fType, p[i][l].fType);
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
      switch(pixelForegroundPhysicalStateVector[p[x-i][y].fType]){
        case pixelForegroundPhysicalState_GASEOUS:
          std::swap(p[x][y].fType, p[x-i][y].fType); 
          notifyForUpdatingAroundThisPixel(x, y);
          notifyForUpdatingAroundThisPixel(x-i, y);
          return true;
        break;
        case pixelForegroundPhysicalState_SOLID:
          canPushLeft = false;
        break;
        default: break;
      }
    }
    // Try to push to the right
    if(canPushRight && x+i <= width-1){
      switch(pixelForegroundPhysicalStateVector[p[x+i][y].fType]){
        case pixelForegroundPhysicalState_GASEOUS:
          std::swap(p[x][y].fType, p[x+i][y].fType); 
          notifyForUpdatingAroundThisPixel(x, y);
          notifyForUpdatingAroundThisPixel(x+i, y);
          return true;
        break;
        case pixelForegroundPhysicalState_SOLID:
          canPushRight = false;
        break;
        default: break;
      }
    }
  }
  
  return false;
}
