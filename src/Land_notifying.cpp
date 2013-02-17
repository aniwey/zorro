#include "Land.hpp"

void Land::switchAtuAndAtuNotif(){
  // We set atu from atuNotif
  atu = atuNotif;
  
  // We erase atuNotif to allow for new notifications during the loop
  for(unsigned int i = 0; i < atuNotif.size(); ++i)
    atuNotif[i].clear();
}

void Land::notifyEverything(){
  notifyForUpdatingThisRectangle(0, 0, width-1, height-1);
}

void Land::notifyForUpdatingThisRectangle(int x1, int y1, int x2, int y2){
  bool areaWasAddedForThisColumn;
  
  // We possibly swap parameters if they're incorrect
  if(x1 > x2) std::swap(x1, x2);
  if(y1 > y2) std::swap(y1, y2);
  
  // We possibly correct parameters if they're out of land
  if(x1 < 0) x1 = 0;
  if(y1 < 0) y1 = 0;
  if(x2 >= width) x2 = width-1;
  if(y2 >= height) y2 = height-1;
  
  // We add areas
  for(int i = x1; i <= x2; ++i){ // Iteration over needed columns
    areaWasAddedForThisColumn = false;
    for(std::list<std::pair<int, int> >::iterator it = atuNotif[i].begin(); it != atuNotif[i].end(); it++){ // Iteration over areas already defined in this column
      // If we're below this area
      if(y1 > (*it).first){
        // We add our area before this one and we break
        atuNotif[i].insert(it, std::pair<int, int>(y2, y1));
        areaWasAddedForThisColumn = true;
        break;
      }
      // Else, we intersect with this area (or we're the same one) -> we replace it to make one only big area and we break
      else if(y2 >= (*it).second){
        if(y2 > (*it).first) (*it).first = y2;
        if(y1 < (*it).second) (*it).second = y1;
        areaWasAddedForThisColumn = true;
        // Here we merged areas, but maybe we included a following area while doing that. If so, we must destroy it
        std::list<std::pair<int, int> >::iterator newIt = it;
        newIt++;
        for(; newIt != atuNotif[i].end(); newIt++){ // Iteration over remaining areas
          if((*newIt).first >= (*it).second){ // If we include it, or at least a part of it
            // We extend ourselves if it would make us bigger
            if((*newIt).second < (*it).second) (*it).second = (*newIt).second;
            // And we erase it
            atuNotif[i].erase(newIt);
            // And we don't break because maybe we include other areas, so we decrease newIt to continue the for loop normally
            newIt--;
          }
          else break; // Else we break
        }
        break;
      }
    }
    // If we weren't added yet
    if(areaWasAddedForThisColumn == false){
      // We add our area at the end of the list
      atuNotif[i].push_back(std::pair<int, int>(y2, y1));
    }
  }
}

void Land::notifyForUpdatingAroundThisPixel(int x, int y){
  notifyForUpdatingThisRectangle(x-1, y-1, x+1, y+1);
}

void Land::notifyForUpdatingThisPixel(int x, int y){
  notifyForUpdatingThisRectangle(x, y, x, y);
}
