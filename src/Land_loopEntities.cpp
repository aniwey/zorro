#include "Land.hpp"

void Land::loopEntities(){  
  // Entities handling
  for(std::list<boost::weak_ptr<Entity>>::iterator it = entities.begin(); it != entities.end(); ++it){
    if(boost::shared_ptr<Entity> ent = (*it).lock()){
      if(ent->loop(*this)){
        notifyForUpdatingAroundThisPixel(ent->pixelX, ent->pixelY);
        p[ent->pixelX][ent->pixelY].create(pixelType_AIR, *this, ent->pixelX, ent->pixelY);
      }
    }
    else{
      entities.erase(it);
      it--;
    }
  }
}
