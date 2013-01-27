#include "Screen.hpp"

void Screen::writeLandToImage(Land& l){
  for(int i = 0; i < l.getWidth(); ++i){
    for(int j = 0; j < l.getHeight(); ++j){
      /*switch(abs(l.p[i][j].group)){
        case 1: screenImage.setPixel(i, j, sf::Color::Blue); break;
        case 2: screenImage.setPixel(i, j, sf::Color::Magenta); break;
        case 3: screenImage.setPixel(i, j, sf::Color::Cyan); break;
        case 4: screenImage.setPixel(i, j, sf::Color::Black); break;
        case 5: screenImage.setPixel(i, j, sf::Color::Red); break;
        default: screenImage.setPixel(i, j, l.p[i][j].color); break;
      }*/
      screenImage.setPixel(i, j, l.p[i][j].color);
    }
  }
}

void Screen::writeImageToTexture(){
  screenTexture.update(screenImage);
  screenSprite.setTexture(screenTexture);
}

void Screen::writeTextureToWindow(){
  window.draw(screenSprite);
}
