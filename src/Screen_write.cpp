#include "Screen.hpp"

void Screen::writeLandToImage(Land& l){
  for(int i = 0; i < l.getWidth(); ++i){
    for(int j = 0; j < l.getHeight(); ++j){
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
