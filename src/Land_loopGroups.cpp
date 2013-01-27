#include "Land.hpp"

void Land::loopGroupsSplitting(){
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < height; ++j){
      // If this pixel is in a group and its group is negative when groups should be positive or is positive when groups should be negative
      if(p[i][j].group != 0 && ((pixelGroupsArePositive && p[i][j].group < 0) || (!pixelGroupsArePositive && p[i][j].group > 0))){
        changePixelGroupRecursively(i, j, p[i][j].group, getPixelGroup());
      }
    }
  }
}

bool Land::resolveGroupGravityStructDependencies(int i){
  // If we're not trying to resolve deps of the original group asking for resolving, unless it's the first call of the recursive function
  if(ggs[i].dependenciesResolved == false){
    ggs[i].dependenciesResolved = true;
    // We iterate over the list of deps of this groupGravityStruct
    for(std::list<int>::iterator it = ggs[i].deps.begin(); it != ggs[i].deps.end(); ++it){
      // Switch for the canFall state of this dependency
      switch(ggs[*it].canFall){
        // The dependency can fall -> it changes nothing for us
        case canFallState_YES: break;
        // It can fall, but with deps -> we resolve its deps too
        case canFallState_YES_WITH_DEPS:
          if(resolveGroupGravityStructDependencies(*it)){
            ggs[i].canFall = canFallState_NO;
            return true;
          }
        break;
        // It can't fall -> we can't fall either
        case canFallState_NO:
          ggs[i].canFall = canFallState_NO;
          return true;
        break;
      }
    }
  }
  return false;
}
