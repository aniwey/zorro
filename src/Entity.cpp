#include "Entity.hpp"

Entity::Entity(int frame_id){
  last_frame_id = frame_id;
}

Entity::Entity(){
  last_frame_id = 0;
}

Entity::~Entity(){

}
