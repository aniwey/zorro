#include "Land.hpp"

void Land::loopGravity(){
  // Preparing ggs array
  ggs.resize(abs(lastPixelGroupGiven));
  for(unsigned int i = 0; i < ggs.size(); ++i){
    ggs[i].canFall = canFallState_YES;
    ggs[i].deps.clear();
    ggs[i].dependenciesResolved = false;
  }

  // Writing canFall states for all pixel on the last line of the land
  for(int i = 0; i < width; ++i){
    if(p[i][height-1].group != 0){
      ggs[abs(p[i][height-1].group)-1].canFall = canFallState_NO;
    }
  }

  // Writing canFall states and dependencies
  bool margin;
  std::list<int> margin_deps;
  for(int i = 0; i < width; ++i){
    // For this column, we set the first value on margin, depending on the first pixel of the column
    if(pixelPhysicalStateVector[p[i][height-1].type] == pixelPhysicalState_GASEOUS)
      margin = true;
    else{
      margin = false;
      if(p[i][height-1].group != 0){
        ggs[abs(p[i][height-1].group)-1].canFall = canFallState_NO;
      }
    }
    
    for(int j = height-2; j >= 0; --j){
      // If this pixel is in a group which can still fall
      if(p[i][j].group != 0 && ggs[abs(p[i][j].group)-1].canFall != canFallState_NO){
        // If the pixel below has a group and it's not the same as ours, then we add this group to our deps list
        if(p[i][j+1].group != 0 && p[i][j+1].group != p[i][j].group){
          ggs[abs(p[i][j].group)-1].canFall = canFallState_YES_WITH_DEPS;
          ggs[abs(p[i][j].group)-1].deps.push_back(abs(p[i][j+1].group)-1);
        }
        
        // Else if the pixel below doesn't have a group
        else if(p[i][j+1].group == 0){
          // If the pixel below can't move, then we can't move either
          if(pixelGravityVector[p[i][j+1].type] == pixelGravity_CANT_FALL){
            ggs[abs(p[i][j].group)-1].canFall = canFallState_NO;
          }
          // If the pixel below can move, then we may move too, depending on the current margin
          else if(pixelGravityVector[p[i][j+1].type] == pixelGravity_MAY_FALL){
            if(margin == false) ggs[abs(p[i][j].group)-1].canFall = canFallState_NO;
            else{
              ggs[abs(p[i][j].group)-1].canFall = canFallState_YES_WITH_DEPS;
              ggs[abs(p[i][j].group)-1].deps.splice(ggs[abs(p[i][j].group)-1].deps.end(), margin_deps);
            }
          }
          // Else, no change for us
        }
        // Else, the pixel below have a group but it's the same as our -> we do nothing
        
        // Add ourselves to the margin_deps list if margin is true
        if(margin) margin_deps.push_back(abs(p[i][j].group)-1);
      }
      // Else, if this pixel is in a group which can't fall
      else if(p[i][j].group != 0){
        // No margin!
        margin = false;
      }
      // Else, if this pixel can't move
      else if(pixelGravityVector[p[i][j].type] == pixelGravity_CANT_FALL){
        // No margin!
        margin = false;
      }
      // Else, if this pixel is gaseous
      else if(pixelPhysicalStateVector[p[i][j].type] == pixelPhysicalState_GASEOUS){
        // Margin!
        margin = true;
        margin_deps.clear();
      }
      // Else, we don't change the margin
    }
    margin_deps.clear();
  }
  
  // Resolving dependencies
  for(unsigned int i = 0; i < ggs.size(); ++i){
    if(ggs[i].canFall == canFallState_YES_WITH_DEPS){
      resolveGroupGravityStructDependencies(i);
      if(ggs[i].canFall == canFallState_YES_WITH_DEPS){
        ggs[i].canFall = canFallState_YES;
      }
    }
  }
  
  // Make fall those who can
  for(int i = 0; i < width; ++i){
    for(int j = height-2; j >= 0; --j){
      // If this pixel is in a group which can fall
      if(p[i][j].group != 0 && ggs[abs(p[i][j].group)-1].canFall == canFallState_YES){
        // If the pixel below is gaseous, then we just make it fall
        if(pixelPhysicalStateVector[p[i][j+1].type] == pixelPhysicalState_GASEOUS){
          if(p[i][j].entity != 0) p[i][j].entity->isGoingToFall(); // Entity falling callback
          swap(p[i][j], p[i][j+1]);
        }
      }
      // Else, if this pixel isn't in a group but we have to handle its gravity and the pixel below is gaseous
      else if(p[i][j].group == 0 && 
              pixelGravityVector[p[i][j].type] == pixelGravity_MAY_FALL && 
              pixelPhysicalStateVector[p[i][j+1].type] == pixelPhysicalState_GASEOUS){
        if(p[i][j].entity != 0) p[i][j].entity->isGoingToFall(); // Entity falling callback
        swap(p[i][j], p[i][j+1]);
      }
    }
  }
}
