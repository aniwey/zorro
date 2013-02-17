#include "Land.hpp"

void Land::loop(){
  /*
  std::cout << "Groups : " << g.size() << std::endl;
  
  int sum = 0;
  std::cout << "Areas to update : ";
  for(unsigned int i = 0; i < atuNotif.size(); ++i){
    sum += atuNotif[i].size();
    for(std::list<std::pair<int, int> >::iterator it = atuNotif[i].begin(); it != atuNotif[i].end(); it++){
      std::cout << "(" << i << ")" << " " << (*it).first << " " << (*it).second << std::endl << "                  ";
    }
  }
  std::cout << sum << std::endl;
  
  std::cout << std::endl;
  
  std::cout << "Groups : " << g.size() << std::endl;
  
  int nb = 0;
  for(std::list<Group>::iterator it = g.begin(); it != g.end(); ++it){
    std::cout << "         Group " << nb << std::endl;
    for(std::list<boost::shared_ptr<GroupPixel> >::iterator it2 = (*it).pixels.begin(); it2 != (*it).pixels.end(); ++it2){
      std::cout << "         " << (*it2)->x << " " << (*it2)->y << " : ";
      switch((*it2)->depType){
        case GroupDependencyType_BOTTOM_OF_THE_MAP:
          std::cout << "bottom of the map";
        break;
        case GroupDependencyType_GROUP:
          std::cout << "group";
        break;
        case GroupDependencyType_CANT_FALL_PIXEL:
          std::cout << "can't fall pixel";
        break;
        case GroupDependencyType_NOTHING:
          std::cout << "nothing";
        break;
      }
      std::cout << std::endl;
    }
    nb++;
  }
  std::cout << std::endl << std::endl;*/
  
  loopPixels();
  loopEntities();
  frame_id++;
}
