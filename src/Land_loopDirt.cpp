#include "Land.hpp"

void Land::loopDirt(){
  int i, j;
  
  // What's the thing with k and i?! What don't we just use k?
  //   Imagine width = 7, for example. If we just use k, then we'll study columns in this order :
  //     0, 1, 2, 3, 4, 5, 6
  //   This causes a problem! Pixels falling on the left side of a dirt pyramid won't act the same way as pixels falling on its right side. That's why we have to study pixels in this order :
  //     0, 2, 4, 6, 1, 3, 5 (even numbers first, odd numbers after, it could be inversed without problem)
  //   That's what i is for in the for loop (pun intended haha)
  for(unsigned int k = 0; k < atu.size(); k++){
    // Set i
    if((int)k < (width+1)/2) i = k*2;
    else i = (k - (width+1)/2)*2 + 1;
    // Set the first value of j
    j = height;
    for(std::list<std::pair<int, int> >::iterator it = atu[i].begin(); it != atu[i].end(); it++){ // Iteration over areas in this column
      // If j is below the current area, then we'll go all over this area, else we do nothing and continue with the next area
      if(j >= (*it).second){
        // If j is below the beginning of the area PLUS ONE, we set j = beginning of the area PLUS ONE (unless we're at the bottom of the map)
        // Now, why do we do that ? By doing that, we don't only check the areas to update, but we also check every pixel below an area to update.
        // And this is necessary ! Because, unlike other pixels, dirt pixels check the pixels at y-2 relatively to them !
        if((*it).first <= height-1 && j > (*it).first+1) j = (*it).first+1;
        else j = (*it).first;
        for(; j >= (*it).second; --j){
          // If this pixel is a dirt pixel and it felt at least one frame ago
          if(p[i][j].type == pixelType_DIRT && p[i][j].feltAtThisFrame < frame_id){
            // If the two blocks above this one are dirt, this one is crushed and try to move out
            if(p[i][j-1].type == pixelType_DIRT && p[i][j-2].type == pixelType_DIRT){
              // If the pixel does not touch the left side of the screen and the pixel on its left is non solid and none
              if(i != 0 && pixelForegroundPhysicalStateVector[p[i-1][j].fType] != pixelForegroundPhysicalState_SOLID && p[i-1][j].type == pixelType_NONE){
                // We move the pixel to the left
                swap(p[i][j], p[i-1][j]);
                // We tell it that it felt
                p[i-1][j].feltAtThisFrame = frame_id;
                // We notify
                notifyForUpdatingThisRectangle(i-2, j-1, i+1, j+2);
                // We move all the dirt pixels above
                int l = j-1; while(p[i][l].type == pixelType_DIRT && l >= 0) { swap(p[i][l], p[i][l+1]); l--; }
                // We notify
                notifyForUpdatingThisRectangle(i-1, l-1, i+1, j+1);
                j = l;
              }
              // If the pixel does not touch the right side of the screen and the pixel on its right is non solid and none
              else if(i != width-1 && pixelForegroundPhysicalStateVector[p[i+1][j].fType] != pixelForegroundPhysicalState_SOLID && p[i+1][j].type == pixelType_NONE){
                // We move the pixel to the right
                swap(p[i][j], p[i+1][j]);
                // We tell it that it felt
                p[i+1][j].feltAtThisFrame = frame_id;
                // We notify
                notifyForUpdatingThisRectangle(i-1, j-1, i+2, j+2);
                // We move all the dirt pixels above
                int l = j-1; while(p[i][l].type == pixelType_DIRT && l >= 0) { swap(p[i][l], p[i][l+1]); l--; }
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
}
