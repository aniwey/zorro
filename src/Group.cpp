#include "Group.hpp"

#include "Land.hpp"

Group::Group(){

}

bool Group::hasPixels(){
  return !pixels.empty();
}

bool Group::resolveDependencies(Land& l){
  // If we weren't already checked, we check for our dependencies
  if(checked == false){
    checked = true;
    canFallBool = true; // We presume it can fall, let's try to prove the contrary !
    for(std::map<GroupPixel, GroupDependency>::iterator it = pixels.begin(); it != pixels.end(); it++){ // Iteration over pixels
      switch((*it).second.type){ // Switch on the dependency type of the pixel
        case GroupDependencyType_NOTHING:
          // The pixel has no dependency -> it changes nothing
        break;
        case GroupDependencyType_BOTTOM_OF_THE_MAP:
          canFallBool = false; // We can't fall
          return false; // The pixel is blocked by the bottom of the map : we clearly won't be able to fall
        break;
        case GroupDependencyType_CANT_FALL_PIXEL:
          canFallBool = false; // We can't fall
          return false; // The pixel is blocked by a pixel which can't fall : we clearly won't be able to fall
        break;
        case GroupDependencyType_GROUP:
          // The pixel's dependency is a group : if the group can't fall we return false, else we do nothing
          if(l.p[(*it).second.pixelWeDependOn.first][(*it).second.pixelWeDependOn.second].group->resolveDependencies(l) == false){
            canFallBool = false; // We can't fall
            return false;
          }
        break;
      }
    }
    
    // If finally we found no reason proving that we couldn't fall, we return true : we can fall !
    canFallBool = true;
    return true;
  }
  // Else, we were already checked, we return our canFallBool
  else{
    return canFallBool;
  }
}

bool Group::canFall(){
  return canFallBool;
}

Group* Group::registerPixel(int x, int y){
  pixels[GroupPixel(x, y)].type = GroupDependencyType_NOTHING; // This new pixel has no dependency
  pixels[GroupPixel(x, y)].aPixelDependsOnUs = false; // And no pixel depends on it
  return this;
}

Group* Group::unregisterPixel(Land& l, int x, int y, bool removeTheDependencyLink){
  // If we are allowed to remove the dependency link (which must be done if the pixel really quit the group without entering a new one)
  if(removeTheDependencyLink){
    // If a pixel depends on us, we must tell it that it doesn't depend on anything anymore
    if(pixels[GroupPixel(x, y)].aPixelDependsOnUs){
      l.p[pixels[GroupPixel(x, y)].pixelWhichDependsOnUs.first][pixels[GroupPixel(x, y)].pixelWhichDependsOnUs.second].group->
        pixels[GroupPixel(pixels[GroupPixel(x, y)].pixelWhichDependsOnUs.first, pixels[GroupPixel(x, y)].pixelWhichDependsOnUs.second)].type = GroupDependencyType_NOTHING;
    }

    // If we depend on a pixel, we must tell it that no pixel depend on it anymore
    if(pixels[GroupPixel(x, y)].type == GroupDependencyType_GROUP){
      l.p[pixels[GroupPixel(x, y)].pixelWeDependOn.first][pixels[GroupPixel(x, y)].pixelWeDependOn.second].group->
        pixels[GroupPixel(pixels[GroupPixel(x, y)].pixelWeDependOn.first, pixels[GroupPixel(x, y)].pixelWeDependOn.second)].aPixelDependsOnUs = false;
    }
  }

  // Remove the pixel
  pixels.erase(GroupPixel(x, y));
  
  // Return 0 (no pointer)
  return 0;
}

void Group::checkForSplitting(Land& l){
  while(hasPixels()){ // While the group has pixels
    l.g.push_back(Group());
    changePixelGroupRecursively(l, (*pixels.begin()).first.first, (*pixels.begin()).first.second, &l.g.back());
  }
}

void Group::changePixelGroupRecursively(Land& l, int x, int y, Group* newGroup){
  // We register to the new group
  l.p[x][y].group = newGroup->registerPixel(x, y);
  
  // We keep the dependency
  newGroup->pixels[GroupPixel(x, y)] = pixels[GroupPixel(x, y)];
  
  // We unregister from the old group
  unregisterPixel(l, x, y, false);
  
  // And we try to also change adjacent pixels of our old group
  if(x < l.width-1 && l.p[x+1][y].group == this){
    changePixelGroupRecursively(l, x+1, y, newGroup);
  }
  if(x > 0 && l.p[x-1][y].group == this){
    changePixelGroupRecursively(l, x-1, y, newGroup);
  }
  if(y < l.height-1 && l.p[x][y+1].group == this){
    changePixelGroupRecursively(l, x, y+1, newGroup);
  }
  if(y > 0 && l.p[x][y-1].group == this){
    changePixelGroupRecursively(l, x, y-1, newGroup);
  }
}
