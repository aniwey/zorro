#include "Land.hpp"

#include "Seed.hpp"

Seed::Seed(Land& l, int x, int y): Entity(l.frame_id, x, y){
  // Set time
  time = 0;
  
  // Set step
    // If we were added on a pixel of dirt -> we're already planted
    if(l.p[pixelX][pixelY].type == pixelType_DIRT){
      step = seedStep_PLANTED;
    }
    // Else, if we're already landing on a solid pixel or a non-solid not-none pixel
    else if((pixelY < l.height-1 && (l.pixelForegroundPhysicalStateVector[l.p[pixelX][pixelY+1].fType] == pixelForegroundPhysicalState_SOLID || (l.pixelForegroundPhysicalStateVector[l.p[pixelX][pixelY+1].fType] != pixelForegroundPhysicalState_SOLID && l.p[pixelX][pixelY+1].type != pixelType_NONE))) || pixelY == l.height-1){
      step = seedStep_LANDING;
    }
    // Else, we're just falling
    else{
      step = seedStep_FALLING;
    }
    
  // Create the seed pixel
  l.p[pixelX][pixelY].type = pixelType_SEED;
  l.p[pixelX][pixelY].setColorBasedOnType();
  if(l.p[pixelX][pixelY].group != 0) l.p[pixelX][pixelY].group = l.p[pixelX][pixelY].group->unregisterPixel(x, y, true);
  l.p[pixelX][pixelY].feltAtThisFrame = -1;
}

Seed::~Seed(){
  
}

bool Seed::loop(Land& l){
  last_frame_id = l.frame_id;
  
  // If the pixel where the seed is just felt
  if(l.p[pixelX][pixelY].feltAtThisFrame == l.frame_id){
    // If the seed isn't normally falling and isn't already dying
    if(step != seedStep_FALLING && step != seedStep_DYING){
      // Then it's dying
      step = seedStep_DYING;
      time = 0;
    }
  }
  
  switch(step){
    case seedStep_FALLING:
      // If there's no more space to fall under us, we move to the next step
      if((pixelY < l.height-1 && (l.pixelForegroundPhysicalStateVector[l.p[pixelX][pixelY+1].fType] == pixelForegroundPhysicalState_SOLID || (l.pixelForegroundPhysicalStateVector[l.p[pixelX][pixelY+1].fType] != pixelForegroundPhysicalState_SOLID && l.p[pixelX][pixelY+1].type != pixelType_NONE))) || pixelY == l.height-1){
        step = seedStep_LANDING; // We move to the next step
        time = 0; // And we reset the time
      }
    break;
    case seedStep_LANDING:
      // If there's dirt under us, we go into the dirt and we move to the next step
      if(pixelY < l.height-1 && l.p[pixelX][pixelY+1].type == pixelType_DIRT){
        l.p[pixelX][pixelY+1].create(l, pixelX, pixelY+1, pixelType_NONE); // We make space below in order to go there
        swap(l.p[pixelX][pixelY], l.p[pixelX][pixelY+1]); // We move to the down
        l.p[pixelX][pixelY+1].youJustMovedTo(pixelX, pixelY+1); // We notify the pixel
        l.notifyForUpdatingThisRectangle(pixelX-1, pixelY-1, pixelX+1, pixelY+1); // We notify
        step = seedStep_PLANTED; // We move to the next step
        time = 0; // And we reset the time
      }
      // Else if the time is high enough, we destroy the seed
      else if(time > 60){
        return true;
      }
      // Else, we increment the time
      else time++;
    break;
    case seedStep_PLANTED:
      // If time is high enough
      if(time > 60){
        step = seedStep_GROWING; // We move to the next step
        l.p[pixelX][pixelY].group = l.getPixelGroup()->registerPixel(pixelX, pixelY); // We create a group for us and our future leaves
        l.notifyForUpdatingAroundThisPixel(pixelX, pixelY);
        time = 0; // And we reset time
      }
      // Else we increment the time spent planted
      else time++;
    break;
    case seedStep_GROWING:
      // If time is enough
      if(time > 60){
        step = seedStep_GROWING_WITH_FRUITS;
        time = 0;
      }
      // Else we increment the time spent growing with no fruits
      else{
        time++;
      }
      
      growLeaves(l, pixelX, pixelY, false);
    break;
    case seedStep_GROWING_WITH_FRUITS:
      if(time > 20){
        growLeaves(l, pixelX, pixelY, true);
        time = 0;
      }
      else{
        growLeaves(l, pixelX, pixelY, false);
        time++;
      }
    break;
    case seedStep_DYING:
      // If time is high enough
      if(time > 60){
        // We return true : we're going to be destroyed
        return true;
      }
      // Else we increment the time spent dying
      else time++;
    break;
  }
  
  return false;
}

void Seed::growLeaves(Land& l, int xSeed, int ySeed, bool withFruits){
  int i, j, d;
  bool aLeafGrew = false;
  
  // Andres circle algorithm
  for(int r = 1; r <= 7; r++){
    i = 0;
    j = r;
    d = r-1;
    while(j >= i && aLeafGrew == false){
      aLeafGrew = growLeafHere(l, xSeed+j, ySeed-i, xSeed, ySeed, withFruits); // 0 -> pi/4
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed+i, ySeed-j, xSeed, ySeed, withFruits); // pi/4 -> pi/2
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed-i, ySeed-j, xSeed, ySeed, withFruits); // pi/2 -> 3pi/4
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed-j, ySeed-i, xSeed, ySeed, withFruits); // 3pi/4 -> pi
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed-j, ySeed+i, xSeed, ySeed, withFruits); // -pi -> -3pi/4
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed-i, ySeed+j, xSeed, ySeed, withFruits); // -3pi/4 -> -pi/2
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed+i, ySeed+j, xSeed, ySeed, withFruits); // -pi/2 -> -pi/4
      if(!aLeafGrew) aLeafGrew = growLeafHere(l, xSeed+j, ySeed+i, xSeed, ySeed, withFruits); // -pi/4 -> 0
      if(!aLeafGrew){
        if(d >= 2*i){
          d = d-2*i-1;
          i = i+1;
        }      
        else if(d <= 2*(r-j)){
          d = d+2*j-1;
          j = j-1;
        }
        else{
          d = d+2*(j-i-1);
          j = j-1;
          i = i+1;
        }
      }
    }
  }
}

bool Seed::growLeafHere(Land &l, int xGrow, int yGrow, int xSeed, int ySeed, bool andMaybeAFruit){
  // If the pixel exists, then we may grow something here!
  if(l.thisPixelExists(xGrow, yGrow)){
    // If the pixel here is gaseous and there's a pixel of our group adjacent to it, then we grow a leaf
    if(l.pixelForegroundPhysicalStateVector[l.p[xGrow][yGrow].fType] != pixelForegroundPhysicalState_SOLID && l.p[xGrow][yGrow].type == pixelType_NONE && l.aPixelOfThisGroupIsAdjacentToThisOne(xGrow, yGrow, l.p[xSeed][ySeed].group)){
      l.p[xGrow][yGrow].create(l, xGrow, yGrow, pixelType_LEAVES);
      l.p[xGrow][yGrow].group = l.p[xSeed][ySeed].group->registerPixel(xGrow, yGrow);
      // We notify
      l.notifyForUpdatingAroundThisPixel(xGrow, yGrow);
      return true;
    }
    // Else, if we can grow a fruit and the nine pixels here are leaves of our group and we're not trying to grow it on top of the seed
    else if(andMaybeAFruit && 
            l.howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType_LEAVES, l.p[xSeed][ySeed].group, xGrow-1, yGrow-1, xGrow+1, yGrow+1) == 9 &&
            ((yGrow > ySeed) || (xGrow != xSeed))){
      l.p[xGrow][yGrow].create(l, xGrow, yGrow, pixelType_FRUIT);
      // We notify
      l.notifyForUpdatingAroundThisPixel(xGrow, yGrow);
      return true;
    }
  }
  return false;
}
