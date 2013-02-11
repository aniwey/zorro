#include "Entity.hpp"

Entity::Entity(int frame_id, int x, int y): last_frame_id(frame_id), pixelX(x), pixelY(y){

}

Entity::Entity(): pixelX(0), pixelY(0){
  last_frame_id = 0;
}

Entity::~Entity(){

}
