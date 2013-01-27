#include "ToolbarButton.hpp"

ToolbarButton::ToolbarButton(ImageManager& im, std::string _filename, int x, int y, pixelType _pixelTypeToSelect, bool _displayed): 
                             filename(_filename), pixelTypeToSelect(_pixelTypeToSelect), displayed(_displayed){
  center.x = x;
  center.y = y;
  
  updateImageSize(im);
  updateSizeAndHalfsize();
  
  hasText = false;
}

void ToolbarButton::updateImageSize(ImageManager& im){
  imageSize = (sf::Vector2f)(*im.getImage(filename)).getSize();
}

void ToolbarButton::updateSizeAndHalfsize(){
  size = (sf::Vector2f)imageSize;
  if(hasText) size.y += 12;
  halfsize.x = size.x/2;
  halfsize.y = size.y/2;
}

void ToolbarButton::setText(MapStringStringIterator _text){
  text = _text;
  hasText = true;
  updateSizeAndHalfsize();
}
