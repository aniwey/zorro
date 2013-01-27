#ifndef HPP_TOOLBARBUTTON
#define HPP_TOOLBARBUTTON

#include <string>

#include <SFML/Graphics.hpp>

#include "ConfigData.hpp"
#include "ImageManager.hpp"
#include "pixelType.hpp"

class ToolbarButton{
  public:
    ToolbarButton(ImageManager&, std::string, int, int, pixelType, bool);
    
    // Position, size etc
    sf::Vector2f center, imageSize, size, halfsize;
    
    // Data
    std::string filename;
    bool hasText;
    MapStringStringIterator text;
    pixelType pixelTypeToSelect;
    bool displayed;
    
    // Functions to manipulate data
    void setText(MapStringStringIterator); // Set text and then recalculate size and halfsize
  
  private:
    void updateImageSize(ImageManager&); // Look at the image in the image manager and save its size
    void updateSizeAndHalfsize(); // Update size and half size, based on imageSize and on text
};

#endif
