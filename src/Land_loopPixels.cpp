#include "Land.hpp"

void Land::loopPixels(){
  // Set the checked bool to false for every group
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
  }
  
  // If there's a pixel with a group in the areas to update, we add its group to gtu
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        if(p[i][j].group != 0 && p[i][j].group->checked == false){ // If this pixel has a group which isn't already checked
          gtu.push_back(p[i][j].group); // We add this group to gtu
          p[i][j].group->checked = true; // And we check it
        }
      }
    }
  }
  
  // We test splitting on every group in gtu  
  for(std::list<Group*>::iterator it = gtu.begin(); it != gtu.end(); it++){ // Iteration over groups to update
    (*it)->checkForSplitting(*this);
  }
  
  gtu.clear();
  
  // Set the checked bool to false for every group
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
  }
  
  // If there's a pixel with a group in the areas to update, we add its group to gtu
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        if(p[i][j].group != 0 && p[i][j].group->checked == false){ // If this pixel has a group which isn't already checked
          gtu.push_back(p[i][j].group); // We add this group to gtu
          p[i][j].group->checked = true; // And we check it
        }
      }
    }
  }
    
  // Update dependencies of group pixels in the areas to update
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        if(p[i][j].group != 0){ // If this pixel has a group
          // We set the default dependency, which will be applied if nothing relevant is found below the pixel
          p[i][j].group->pixels[GroupPixel(i, j)].type = GroupDependencyType_BOTTOM_OF_THE_MAP;
          
          // We search below until we find a pixel with a group / which can't fall / gaseous
          for(int k = j+1; k < height; ++k){
            if(p[i][k].group != 0){ // Else, if this pixel below has a group
              if(p[i][j].group != p[i][k].group){ // If its group is different from ours
                // We now depend on it
                p[i][j].group->pixels[GroupPixel(i, j)].type = GroupDependencyType_GROUP;
                p[i][j].group->pixels[GroupPixel(i, j)].pixelWeDependOn = GroupPixel(i, k);
                // And it now depends on us
                p[i][k].group->pixels[GroupPixel(i, k)].aPixelDependsOnUs = true;
                p[i][k].group->pixels[GroupPixel(i, k)].pixelWhichDependsOnUs = GroupPixel(i, j);
              }
              else{ // Else, same group as ours, no dependency
                p[i][j].group->pixels[GroupPixel(i, j)].type = GroupDependencyType_NOTHING;
              }
              break;
            }
            // Else, if this pixel below can't fall, we set the dependency
            else if(pixelGravityVector[p[i][k].type] == pixelGravity_CANT_FALL){
              p[i][j].group->pixels[GroupPixel(i, j)].type = GroupDependencyType_CANT_FALL_PIXEL;
              break;
            }
            // Else, if this pixel below is gaseous, there's no dependency
            else if(pixelPhysicalStateVector[p[i][k].type] == pixelPhysicalState_GASEOUS){
              p[i][j].group->pixels[GroupPixel(i, j)].type = GroupDependencyType_NOTHING;
              break;
            }
          }
        }
      }
    }
  }
  
  //std::cout << g.size() << std::endl;
  
  // Set the checked bool to false for every group
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    (*it).checked = false;
  }
  
  // Resolve dependencies between groups, determine which one can fall and which one can't
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups in gtu
    (*it).resolveDependencies(*this);
  }
    
  // Apply gravity on alone pixel & group pixels with no dependencies
  for(unsigned int i = 0; i < atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        if(j < height-1){ // If we're not at the bottom of the land
          tryToMakeFallAlongWithPixelsBelow(i, j);
        }
      }
      // Here, we have updated every pixels in our area. But maybe updating them just allowed for above pixels to be updated too !
      // That's why we now try to make fall every above pixels until we reach either the top of the map or the beginning or the next area
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
        if(tryToMakeFall(i, j) == false) break;
      }
    }
  }

  // Apply gravity on group pixels of all groups if gravity wasn't already applied on them just before
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups in 
    if((*it).canFall()){ // If this group can fall
      for(std::map<GroupPixel, GroupDependency>::iterator it2 = (*it).pixels.begin(); it2 != (*it).pixels.end(); it2++){ // Iteration over pixels in this group to update
        if(p[(*it2).first.first][(*it2).first.second].feltAtThisFrame < frame_id){ // If this pixel didn't just felt
          tryToMakeFallAlongWithPixelsBelow((*it2).first.first, (*it2).first.second); // We try to make it fall
        }
      }
    }
  }
  
  // Dirt loop
  loopDirt();
    
  // We clear gtu
  gtu.clear();
    
  // We destroy all groups with no pixels
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); it++){ // Iteration over groups
    if((*it).hasPixels() == false){ // If the group has no pixels
      g.erase(it);
      it--;
    }
  }
}

bool Land::tryToMakeFall(int x, int y){
  if(pixelPhysicalStateVector[p[x][y+1].type] == pixelPhysicalState_GASEOUS){ // If the pixel below is gaseous
    return makeFall(x, y);
  }
  return false;
}

void Land::tryToMakeFallAlongWithPixelsBelow(int x, int y){
  int maxHeight;

  // Calculate maxHeight (height to which we'll make pixels fall)
  for(maxHeight = y; maxHeight < height-1; ++maxHeight){
    // If the pixel has no group but may not fall OR has a group but its group can't fall
    if((p[x][maxHeight].group == 0 && pixelGravityVector[p[x][maxHeight].type] != pixelGravity_MAY_FALL)
        ||
       (p[x][maxHeight].group != 0 && p[x][maxHeight].group->canFall() == false)){
      // We break the loop, since this pixel can't fall..
      break;
    }
  }
  
  // Make fall pixels from y to maxHeight-1 if there's a gaseous pixel at maxHeight
  if(pixelPhysicalStateVector[p[x][maxHeight].type] == pixelPhysicalState_GASEOUS){
    for(int j = maxHeight-1; j >= y; --j){
      makeFall(x, j);
    }
  }
}

bool Land::makeFall(int x, int y){
  if(p[x][y].group == 0){ // If we don't belong to a group
    if(pixelGravityVector[p[x][y].type] == pixelGravity_MAY_FALL){ // If we can fall
      // Then we swap with the pixel below
      swap(p[x][y], p[x][y+1]);
      // We tell the pixel that he moved
      p[x][y+1].youJustMovedTo(x, y+1);
      // We set that it felt at this frame
      p[x][y+1].feltAtThisFrame = frame_id;
      // And we notify that the area will have to be updated
      notifyForUpdatingThisRectangle(x-1, y-1, x+1, y+2);
      // We return
      return true;
    }
  }
  else{ // Else, we belong to a group
    if(p[x][y].group->canFall()){ // If our group can fall
      // Then we swap with the pixel below
      swap(p[x][y], p[x][y+1]);
      // We tell the pixel that he moved
      p[x][y+1].youJustMovedTo(x, y+1);
      // We unregister the old pixel and register the new one, so that our group can track us
      p[x][y+1].group->unregisterPixel((*this), x, y, true);
      p[x][y+1].group->registerPixel(x, y+1);
      // We set that it felt at this frame
      p[x][y+1].feltAtThisFrame = frame_id;
      // And we notify that the area will have to be updated
      notifyForUpdatingThisRectangle(x-1, y-1, x+1, y+2);
      // We return
      return true;
    }
  }
  
  return false;
}
