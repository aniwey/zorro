#ifndef HPP_IMAGEMANAGER
#define HPP_IMAGEMANAGER

#include <map>
#include <string>

#include <SFML/Graphics.hpp>

class ImageManager{
  public:
    ImageManager();
    ~ImageManager();
    
    sf::Image* getImage(std::string);
    
  private:
    std::map<std::string, sf::Image> images;
};

#endif
