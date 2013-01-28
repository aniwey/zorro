#include "Screen.hpp"

void Screen::writeLandToImage(Land& l){
  for(int i = 0; i < l.width; ++i){
    for(int j = 0; j < l.height; ++j){
      screenImage.setPixel(i, j, l.p[i][j].color);
    }
  }
}

void Screen::writeImageToTexture(){
  screenTexture.update(screenImage);
}

void Screen::writeTextureToWindow(){
  window.draw(screenSprite);
}
