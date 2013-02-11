#include "Land.hpp"

Group* Land::getPixelGroup(){
  g.push_back(Group());
  return &g.back();
}

bool Land::aPixelOfThisGroupIsAdjacentToThisOne(int x, int y, Group* group){
  if(x < width-1 && p[x+1][y].group == group)
    return true;

  if(x > 0 && p[x-1][y].group == group)
    return true;

  if(y < height-1 && p[x][y+1].group == group)
    return true;

  if(y > 0 && p[x][y-1].group == group)
    return true;

  return false;
}

int Land::howManyPixelsOfThisTypeAndThisGroupInThisRectangle(pixelType type, Group* group, int x1, int y1, int x2, int y2){
  int count = 0;
  
  // We first correct x, y, w and h if they don't fit in the land
  if(x1 < 0) x1 = 0;
  if(y1 < 0) y1 = 0;
  if(x2 > width-1) x2 = width-1;
  if(y2 > height-1) y2 = height-1;
  
  // And we count pixels
  for(int i = x1; i <= x2; ++i){
    for(int j = y1; j <= y2; ++j){
      // If this pixel match the type and the group, we add 1 to count
      if(p[i][j].type == type && p[i][j].group == group)
        count++;
    }
  }
  
  return count; // We return the count of pixels
}
