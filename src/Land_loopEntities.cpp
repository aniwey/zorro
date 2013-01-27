#include "Land.hpp"

void Land::loopEntities(){
  // Entities handling
  for(int i = 0; i < width; ++i){
    for(int j = height-1; j >= 0; --j){
      if(p[i][j].entity != 0 && p[i][j].entity->last_frame_id < frame_id){
        if(p[i][j].entity->loop(*this, i, j)){
          p[i][j].create(pixelType_AIR, *this, i, j);
        }
      }
    }
  }
}
