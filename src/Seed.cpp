#include "Seed.hpp"

Seed::Seed(Land& l, int x, int y): Entity(l.frame_id){
  // Set time
  time = 0;
  
  // Set step
    // If we were added on a pixel of dirt -> we're already planted
    if(l.p[x][y].type == pixelType_DIRT){
      step = seedStep_PLANTED;
    }
    // Else, if we're already landing on a solid pixel
    else if((y < l.getHeight()-1 && l.pixelPhysicalStateVector[l.p[x][y+1].type] == pixelPhysicalState_SOLID) || y == l.getHeight()-1){
      step = seedStep_LANDING;
    }
    // Else, we're just falling
    else{
      step = seedStep_FALLING;
    }
    
  // Create the seed pixel
  l.p[x][y].type = pixelType_SEED;
  l.p[x][y].setColorBasedOnType();
  l.p[x][y].group = 0;
}

Seed::~Seed(){
  
}

void Seed::isGoingToFall(){
  // If the seed isn't normally falling and isn't already dying
  if(step != seedStep_FALLING && step != seedStep_DYING){
    // Then it's dying
    step = seedStep_DYING;
    time = 0;
  }
}

bool Seed::loop(Land& l, int x, int y){
  last_frame_id = l.frame_id;
  
  switch(step){
    case seedStep_FALLING:
      // If there's no more air under us, we move to the next step
      if((y < l.getHeight()-1 && l.p[x][y+1].type != pixelType_AIR) || y == l.getHeight()-1){
        step = seedStep_LANDING; // We move to the next step
        time = 0; // And we reset the time
      }
    break;
    case seedStep_LANDING:
      // If there's dirt under us, we go into the dirt and we move to the next step
      if(y < l.getHeight()-1 && l.p[x][y+1].type == pixelType_DIRT){
        l.p[x][y+1].create(pixelType_AIR, l, x, y+1); // We make space above in order to go there
        swap(l.p[x][y], l.p[x][y+1]); // We move to the down
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
        l.p[x][y].setGroup(l.getPixelGroup()); // We create a group for us and our future leaves
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
      
      growLeaves(l, x, y, false);
    break;
    case seedStep_GROWING_WITH_FRUITS:
      if(time > 20){
        growLeaves(l, x, y, true);
        time = 0;
      }
      else{
        growLeaves(l, x, y, false);
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
    if(l.pixelPhysicalStateVector[l.p[xGrow][yGrow].type] == pixelPhysicalState_GASEOUS && l.aPixelOfThisGroupIsAdjacentToThisOne(xGrow, yGrow, l.p[xSeed][ySeed].group)){
      l.p[xGrow][yGrow].create(pixelType_LEAVES, l, xGrow, yGrow);
      l.p[xGrow][yGrow].setGroup(l.p[xSeed][ySeed].group);
      return true;
    }
    // Else, if we can grow a fruit and the nine pixels here are leaves of our group and we're not trying to grow it on top of the seed
    else if(andMaybeAFruit && 
            l.howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType_LEAVES, l.p[xSeed][ySeed].group, xGrow-1, yGrow-1, xGrow+1, yGrow+1) == 9 &&
            ((yGrow > ySeed) || (xGrow != xSeed))){
      l.p[xGrow][yGrow].create(pixelType_FRUIT, l, xGrow, yGrow);
      return true;
    }
  }
  return false;
}