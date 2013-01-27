#include "Land.hpp"

void Land::loop(){
  loopDirt();
  if(lastPixelGroupGiven != 0){ // If there's at least one group in the land
    switchPixelGroupsSign();
    loopGroupsSplitting();
  }
  loopGravity();
  loopEntities();
  frame_id++;
}
