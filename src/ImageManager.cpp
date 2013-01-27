#include "ImageManager.hpp"

ImageManager::ImageManager(){
  
}

ImageManager::~ImageManager(){
  
}

sf::Image* ImageManager::getImage(std::string filename){
  if(images.count(filename) == 0){
    images[filename] = sf::Image();
    images[filename].loadFromFile(std::string("./images/") + filename + std::string(".png"));
  }
  
  return &images[filename];
}
