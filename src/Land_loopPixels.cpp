#include "Land.hpp"

void Land::loopPixels(){
  checkForGroupsSplittingAndUpdateDependencies();
  resolveDependenciesBetweenGroups();
  applyGravityOnAtu();
  applyGravityOnGroupsOutsideAtu();
  loopDirt();
  loopWater();
  destroyEmptyGroups();
}

void Land::destroyEmptyGroups(){
  // We destroy all groups with no pixels
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    if((*it).hasPixels() == false){ // If the group has no pixels
      g.erase(it);
      it--;
    }
  }
}

void Land::applyGravityOnGroupsOutsideAtu(){
  // Apply gravity on group pixels of all groups if gravity wasn't already applied on them just before
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    if((*it).hasPixels() && (*it).canFall()){ // If this group can fall
      bool modif;
      do{
        modif = false;
        for(std::list<boost::shared_ptr<GroupPixel> >::iterator it2 = (*it).pixels.begin(); it2 != (*it).pixels.end(); it2++){ // Iteration over pixels in this group to update
          if(p[(*it2)->x][(*it2)->y].feltAtThisFrame < frame_id){ // If this pixel didn't just felt
            // We try to make it fall
            if(tryToMakeFallAlongWithPixelsBelow((*it2)->x, (*it2)->y)){
              modif = true;
              break;
            }
          }
        }
      }while(modif == true);
    }
  }
}

void Land::applyGravityOnAtu(){
  // Apply gravity on alone pixel & group pixels with no dependencies
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      // We search the correct bottom of the area : it's in fact (*it).first, unless the area begins at the bottom of the land, in which case we take height-1 as the real bottom
      int bottom = (*it).first;
      if(bottom > height-1) bottom = height-1;
      
      // We try to make fall the very first pixel of the area, along with pixels below it
      tryToMakeFallAlongWithPixelsBelow(i, bottom);
      
      // We try to make fall every pixel in the area, EXCEPT the very first and the very last pixel
      bool aPixelFelt = false;
      int heightOfTheFirstPixelOfThisFallingSuccessionWhichFelt;
      for(int j = bottom - 1; j > (*it).second; --j){
        if(tryToMakeFall(i, j)){ // If the pixel felt
          if(aPixelFelt == false){ // And no pixel already felt
            aPixelFelt = true; // Now a pixel felt
            heightOfTheFirstPixelOfThisFallingSuccessionWhichFelt = j; // And we save its position
          }
        }
        else{ // The pixel didn't felt
          if(aPixelFelt){ // If a pixel already felt before
            aPixelFelt = false; // Now no pixel felt
            notifyForUpdatingThisRectangle(i-1, j, i+1, heightOfTheFirstPixelOfThisFallingSuccessionWhichFelt+2); // We notify
          }
        }
      }
      if(aPixelFelt) // If a pixel felt and we didn't notify, we do it now
        notifyForUpdatingThisRectangle(i-1, (*it).second, i+1, heightOfTheFirstPixelOfThisFallingSuccessionWhichFelt+2);
      
      // We try to make fall the very last pixel, and we save the return value : true if it felt, false otherwise
      bool theVeryLastPixelFelt = tryToMakeFall(i, (*it).second);
      if(theVeryLastPixelFelt) notifyForUpdatingThisRectangle(i-1, (*it).second-1, i+1, (*it).second+2); // If it felt, we notify
      
      // Here, we have made fall every pixel in the area which was able to fall. But if we made fall the very last pixel, maybe it allowed for above pixels to fall to ?
      if(theVeryLastPixelFelt){
        // That's why we now try to make fall every above pixels until we reach either the top of the map or the beginning of the next area
        // We first set the y position where we'll stop (top of the land or beginning of the next area)
        int yStop;
        std::list<std::pair<int, int> >::iterator nextArea = it;
        nextArea++;
        if(nextArea == atu[i].end()) // If we were working on the last area, then yStop equals the top of the land
          yStop = 0;
        else // Else, yStop equals just before the beginning of the next area
          yStop = (*nextArea).first + 1;
        
        // Then, we try to make fall any pixel above until we reach yStop or until we don't make fall anything
        for(int j = (*it).second - 1; j >= yStop; --j){
          if(tryToMakeFall(i, j) == false){ // If we don't make fall anything
            // We notify
            notifyForUpdatingThisRectangle(i-1, j, i+1, (*it).second+1);
            // We break
            break;
          }
        }
        // If we're here, it means we reached yStop : we notify
        notifyForUpdatingThisRectangle(i-1, yStop-1, i+1, (*it).second+1);
      }
    }
  }
}

void Land::resolveDependenciesBetweenGroups(){
  // Set the checked bool to false for every group
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
  }
  
  // Resolve dependencies between groups, determine which one can fall and which one can't
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
    if((*it).hasPixels()) (*it).resolveDependencies(*this);
  }
}

void Land::checkForGroupsSplittingAndUpdateDependencies(){
  // Set the checked bool to false for every group
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
  }
  
  // Check for splitting all groups
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){
    if((*it).checked == false){
      (*it).checked = true; // It's checked now
      (*it).checkForSplitting(*this); // And we check for splitting
    }
  }
  
  /*
  // If there's at least one group
  if(g.size() != 0){
    // Check for splitting groups inside the areas to update
    for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
      for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
        for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
          if(p[i][j].group != 0 && p[i][j].group->checked == false){
            p[i][j].group->checked = true; // It's checked now
            p[i][j].group->checkForSplitting(*this); // And we check for splitting
          }
        }
      }
    }
  }
  */
  
  // Update dependencies of group pixels in the areas to update
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        if(p[i][j].group != 0){ // If this pixel has a group        
          // We get the group pixel corresponding to the pixel and we save it for future operations
          boost::shared_ptr<GroupPixel> groupPixelWeAreWorkingOn = p[i][j].group->getGroupPixelSharedPtr(i, j);
        
          // We save the old dependency
          GroupDependencyType oldDependencyType = groupPixelWeAreWorkingOn->depType;
          
          // We set the default dependency, which will be applied if nothing relevant is found below the pixel
          groupPixelWeAreWorkingOn->depType = GroupDependencyType_BOTTOM_OF_THE_MAP;
          
          // We search below until we find a pixel with a group / which can't fall / solid / non solid but none
          for(int k = j+1; k < height; ++k){
            if(p[i][k].group != 0){ // If this pixel below has a group
              if(p[i][j].group != p[i][k].group){ // If its group is different from ours
                // We get the group pixel corresponding to it and we save it for future operations
                boost::shared_ptr<GroupPixel> groupPixelWeAreGoingToDependOn = p[i][k].group->getGroupPixelSharedPtr(i, k);
              
                // If we were already depending on a pixel
                if(oldDependencyType == GroupDependencyType_GROUP){
                  // And we can access it
                  if(boost::shared_ptr<GroupPixel> pixelWeDependOn = groupPixelWeAreWorkingOn->pixelWeDependOn.lock()){
                    // If the group pixel we were depending on isn't the group pixel we found
                    if(*pixelWeDependOn != GroupPixel(i, k)){
                      // We tell the group pixel we were depending on that no pixel depends on it anymore
                      pixelWeDependOn->aPixelDependsOnUs = false;
                    }
                  }
                }
                
                // We now depend on the new pixel
                groupPixelWeAreWorkingOn->depType = GroupDependencyType_GROUP;
                groupPixelWeAreWorkingOn->pixelWeDependOn = boost::weak_ptr<GroupPixel>(groupPixelWeAreGoingToDependOn);
                
                // If it already has a pixel depending on it
                if(groupPixelWeAreGoingToDependOn->aPixelDependsOnUs == true){
                  // And we can access it
                  if(boost::shared_ptr<GroupPixel> pixelWhichDependsOnTheGroupPixelWeAreGoingToDependOn = groupPixelWeAreGoingToDependOn->pixelWhichDependsOnUs.lock()){
                    // If this pixel depending on it isn't already us
                    if(*pixelWhichDependsOnTheGroupPixelWeAreGoingToDependOn != GroupPixel(i, j)){
                      // We tell the pixel which depends on it that he depends on us now !
                      pixelWhichDependsOnTheGroupPixelWeAreGoingToDependOn->pixelWeDependOn = boost::weak_ptr<GroupPixel>(groupPixelWeAreWorkingOn);
                    }
                  }
                }
                
                // Aaaand we finally tell it that we depend on it now
                groupPixelWeAreGoingToDependOn->aPixelDependsOnUs = true;
                groupPixelWeAreGoingToDependOn->pixelWhichDependsOnUs = boost::weak_ptr<GroupPixel>(groupPixelWeAreWorkingOn);
              }
              else{ // Else, same group as ours, no dependency
                groupPixelWeAreWorkingOn->depType = GroupDependencyType_NOTHING;
              }
              break;
            }
            // Else, if the foreground is solid or the pixel below can't fall, we set the dependency
            else if(pixelForegroundPhysicalStateVector[p[i][k].fType] == pixelForegroundPhysicalState_SOLID || pixelGravityVector[p[i][k].type] == pixelGravity_CANT_FALL){
              groupPixelWeAreWorkingOn->depType = GroupDependencyType_CANT_FALL_PIXEL;
              break;
            }
            // Else, if the foreground isn't solid but the pixel is none
            else if(pixelForegroundPhysicalStateVector[p[i][k].fType] != pixelForegroundPhysicalState_SOLID && p[i][k].type == pixelType_NONE){
              groupPixelWeAreWorkingOn->depType = GroupDependencyType_NOTHING;
              break;
            }
          }
        }
      }
    }
  }
}

bool Land::tryToMakeFall(int x, int y){
  if(pixelForegroundPhysicalStateVector[p[x][y+1].fType] != pixelForegroundPhysicalState_SOLID && // If the pixel below is non solid
     p[x][y+1].type == pixelType_NONE && // And the pixel is none
     ((p[x][y].group != 0 && p[x][y].group->canFall()) || (p[x][y].group == 0 && pixelGravityVector[p[x][y].type] == pixelGravity_MAY_FALL))){ // And the pixel has a group which can fall OR has no group but may fall
    makeFall(x, y);
    return true;
  }
  return false;
}

bool Land::tryToMakeFallAlongWithPixelsBelow(int x, int y){
  int maxHeight;

  // Calculate maxHeight (height to which we'll make pixels fall)
  for(maxHeight = y; maxHeight < height-1; ++maxHeight){
    // If the pixel has no group and may not fall OR has a group but its group can't fall OR already felt this frame
    if((p[x][maxHeight].group == 0 && pixelGravityVector[p[x][maxHeight].type] != pixelGravity_MAY_FALL)
        ||
       (p[x][maxHeight].group != 0 && p[x][maxHeight].group->canFall() == false)
        ||
       (p[x][y].feltAtThisFrame == frame_id)){
      // We break the loop, since this pixel can't fall..
      break;
    }
  }
  
  // If there's a non solid pixel at maxHeight && the pixel is none
  if(pixelForegroundPhysicalStateVector[p[x][maxHeight].fType] != pixelForegroundPhysicalState_SOLID && p[x][maxHeight].type == pixelType_NONE){
    // Make fall pixels from y to maxHeight-1
    for(int j = maxHeight-1; j >= y; --j){
      makeFall(x, j);
    }
    
    // If we made fall at least one pixel
    if(maxHeight-1 >= y){
      // Notify
      notifyForUpdatingThisRectangle(x-1, y-1, x+1, maxHeight+1);
    
      // Return true
      return true;
    }
  }
  
  return false;
}

void Land::makeFall(int x, int y){
  // We swap with the pixel below
  swap(p[x][y], p[x][y+1]);
  // We tell the pixel that it moved
  p[x][y+1].youJustMovedTo(x, y+1);
  // We set that it felt at this frame
  p[x][y+1].feltAtThisFrame = frame_id;
  
  // And if we belong to a group..
  if(p[x][y+1].group != 0){
    // We unregister the old pixel and register the new one, so that our group can track us
    p[x][y+1].group->unregisterPixel(x, y, true);
    p[x][y+1].group->registerPixel(x, y+1);
  }
}
