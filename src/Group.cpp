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
    for(std::list<boost::shared_ptr<GroupPixel> >::iterator it = pixels.begin(); it != pixels.end(); it++){ // Iteration over pixels
      switch((*it)->depType){ // Switch on the dependency type of the pixel
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
          // The pixel's dependency is a group : if we can access the pixel it depends on
          if(boost::shared_ptr<GroupPixel> grpPx = (*it)->pixelWeDependOn.lock()){
            // If the group can't fall we return false, else we do nothing
            if(l.p[grpPx->x][grpPx->y].group->resolveDependencies(l) == false){
              canFallBool = false; // We can't fall
              return false;
            }
          }
          // Else, the pixel's dependency is nothing, since we can't access the pixel it should depend on..
          else{
            (*it)->depType = GroupDependencyType_NOTHING;
          }
        break;
      }
    }
    
    // If finally we found no reason proving that we couldn't fall, we let canFallBool to true and we return true : we can fall !
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
  pixels.push_back(boost::shared_ptr<GroupPixel>(new GroupPixel(x, y)));
  pixels.back()->depType = GroupDependencyType_NOTHING; // This new pixel has no dependency
  pixels.back()->aPixelDependsOnUs = false; // And no pixel depends on it
  return this;
}

Group* Group::registerPixel(boost::shared_ptr<GroupPixel> grpPx){
  pixels.push_back(grpPx);
  return this;
}

Group* Group::unregisterPixel(int x, int y, bool removeTheDependencyLink){
  // We first store the group pixel we should be working on
  boost::shared_ptr<GroupPixel> groupPixelWeAreWorkingOn = getGroupPixelSharedPtr(x, y);
  
  // If we are allowed to remove the dependency link (which must be done if the pixel really quit the group without entering a new one)
  if(removeTheDependencyLink){
    // If a pixel depends on us
    if(groupPixelWeAreWorkingOn->aPixelDependsOnUs){
      // And we can access it
      if(boost::shared_ptr<GroupPixel> pixelWhichDependsOnUs = groupPixelWeAreWorkingOn->pixelWhichDependsOnUs.lock()){
        // We must tell it that it doesn't depend on anything anymore
        pixelWhichDependsOnUs->depType = GroupDependencyType_NOTHING;
      }
      // Else we can't access it : no pixel depends on us..
      else groupPixelWeAreWorkingOn->aPixelDependsOnUs = false;
    }

    // If we depend on a pixel
    if(groupPixelWeAreWorkingOn->depType == GroupDependencyType_GROUP){
      // And we can access it
      if(boost::shared_ptr<GroupPixel> pixelWeDependOn = groupPixelWeAreWorkingOn->pixelWeDependOn.lock()){
        // We must tell it that no pixel depend on it anymore
        pixelWeDependOn->aPixelDependsOnUs = false;
      }
      // Else we can't access it : we don't depend on a pixel anymore..
      else groupPixelWeAreWorkingOn->depType = GroupDependencyType_NOTHING;
    }
  }

  // Remove the pixel
  removePixel(x, y);
  
  // Return 0 (no pointer)
  return 0;
}

void Group::checkForSplitting(Land& l){
  while(hasPixels()){ // While the group has pixels
    Group* newGroup = l.getPixelGroup();
    changePixelGroupRecursively(l, (*pixels.begin())->x, (*pixels.begin())->y, newGroup);
    newGroup->checked = true;
  }
}

void Group::changePixelGroupRecursively(Land& l, int x, int y, Group* newGroup){
  // We register to the new group by copying the old GroupPixel, in order to keep the dependency
  l.p[x][y].group = newGroup->registerPixel(getGroupPixelSharedPtr(x, y));
  
  // We remove the pixel from us
  removePixel(x, y);
  
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

boost::shared_ptr<GroupPixel> Group::getGroupPixelSharedPtr(int x, int y){
  // We iterate over our pixels
  for(std::list<boost::shared_ptr<GroupPixel> >::iterator it = pixels.begin(); it != pixels.end(); ++it){
    // If this is the one we're searching for
    if((*it)->x == x && (*it)->y == y)
      return (*it); // We return it
  }
  
  // If we couldn't find the pixel we were searching for, we return a null shared_ptr
  std::cout << "Error : did not find a pixel in a group :/" << std::endl;
  return boost::shared_ptr<GroupPixel>();
}

void Group::removePixel(int x, int y){
  // We iterate over our pixels
  for(std::list<boost::shared_ptr<GroupPixel> >::iterator it = pixels.begin(); it != pixels.end(); ++it){
    // If this is the pixel we're searching for
    if((*it)->x == x && (*it)->y == y){
      // We erase it and we return
      pixels.erase(it);
      return;
    }
  }
  
  std::cout << "Error : we tried to remove a pixel from a group unsuccessfully :/" << std::endl;
}
