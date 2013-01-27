#include "Land.hpp"

void Land::loop(){
  loopDirt();
  loopGroups();
  loopEntities();
  frame_id++;
}
