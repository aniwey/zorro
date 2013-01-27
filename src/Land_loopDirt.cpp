#include "Land.hpp"

void Land::loopDirt(){
  int i, j;
  
  // What's the thing with k and i?! What don't we just use k?
  //   Imagine width = 7, for example. If we just use k, then we'll study columns in this order :
  //     0, 1, 2, 3, 4, 5, 6
  //   This causes a problem! Pixels falling on the right side of a dirt pyramid will be teleported down.. That's why we have to study pixels in this order :
  //     0, 2, 4, 6, 1, 3, 5 (even numbers first, odd numbers after, it could be inversed without problem)
  //   That's what i is for in the for loop (pun intended haha)
  bool margin;
  for(int k = 0; k < width; k++){
    // Set i
    if(k < (width+1)/2) i = k*2;
    else i = (k - (width+1)/2)*2 + 1;
    // Set margin
    if(pixelPhysicalStateVector[p[i][height-1].type] == pixelPhysicalState_GASEOUS) margin = true;
    else margin = false;
    j = height-1;
    while(j >= 0){
      // If this is a dirt pixel
      if(p[i][j].type == pixelType_DIRT){
        // If the two blocks above this one are dirt, this one is crushed and try to move out
        if(p[i][j-1].type == pixelType_DIRT && p[i][j-2].type == pixelType_DIRT && margin == false){
          // If the pixel does not touch the left side of the screen and the pixel on its left is gaseous
          if(i != 0 && pixelPhysicalStateVector[p[i-1][j].type] == pixelPhysicalState_GASEOUS){
            // We move the pixel to the left
            swap(p[i][j], p[i-1][j]);
            // We move all the dirt pixels above
            int l = j-1; while(p[i][l].type == pixelType_DIRT && l >= 0) { swap(p[i][l], p[i][l+1]); l--; }
            j = l;
          }
          // If the pixel does not touch the right side of the screen and the pixel on its right is gaseous
          else if(i != width-1 && pixelPhysicalStateVector[p[i+1][j].type] == pixelPhysicalState_GASEOUS){
            // We move the pixel to the right
            swap(p[i][j], p[i+1][j]);
            // We move all the dirt pixels above
            int l = j-1; while(p[i][l].type == pixelType_DIRT && l >= 0) { swap(p[i][l], p[i][l+1]); l--; }
            j = l;
          }
        }
      }
      else if(pixelPhysicalStateVector[p[i][j].type] == pixelPhysicalState_GASEOUS) margin = true;
      else margin = false;
      // Go up
        j--;
    }
  }
}
