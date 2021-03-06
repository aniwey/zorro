#include "Screen.hpp"

void Screen::writeLandToImage(Land& l){
  /*
  Group* g1, *g2, *g3, *g4, *g5;
  std::list<Group>::iterator it;
  it = l.g.begin();
  
  if(l.g.size() > 0)
    g1 = &(*it);
    
  it++;
  if(l.g.size() > 1)
    g2 = &(*it);
  
  it++;
  if(l.g.size() > 2)
    g3 = &(*it);
    
  it++;
  if(l.g.size() > 3)
    g4 = &(*it);
    
  it++;
  if(l.g.size() > 4)
    g5 = &(*it);
  
  for(int i = 0; i < l.width; ++i){
    for(int j = 0; j < l.height; ++j){
      if(l.p[i][j].group == 0)
        screenImage.setPixel(i, j, l.p[i][j].color);
      else if(l.p[i][j].group == g1)
        screenImage.setPixel(i, j, sf::Color::Red);
      else if(l.p[i][j].group == g2)
        screenImage.setPixel(i, j, sf::Color::Blue);
      else if(l.p[i][j].group == g3)
        screenImage.setPixel(i, j, sf::Color::Yellow);
      else if(l.p[i][j].group == g4)
        screenImage.setPixel(i, j, sf::Color::Magenta);
      else if(l.p[i][j].group == g5)
        screenImage.setPixel(i, j, sf::Color::Cyan);
      else
        screenImage.setPixel(i, j, l.p[i][j].color);
    }
  }

  if(l.redrawEverything == false){
  for(unsigned int i = 0; i < l.atuNotif.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = l.atuNotif[i].begin(); it != l.atuNotif[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        screenImage.setPixel(i, j, getMidColor(sf::Color::Blue, l.p[i][j].color, 0.7));
      }
    }
  }
  }*/
  
  // If we're not supposed to redraw everything at this loop, we only redraw areas just notified for updating
  if(l.redrawEverything == false){
    for(unsigned int i = 0; i < l.atu.size(); ++i){ // Iteration over the columns
      for(std::list<std::pair<int, int> >::iterator it = l.atu[i].begin(); it != l.atu[i].end(); it++){ // Iteration over areas in this column
        for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
          writePixelToImage(l, i, j);
        }
      }
    }
  }
  // Else, we redraw everything
  else{
    for(int i = 0; i < l.width; ++i){ // Iteration over the columns
      for(int j = 0; j < l.height; ++j){ // Iteration over the lines
        writePixelToImage(l, i, j);
      }
    }
    l.redrawEverything = false; // We set redrawEverything to false
  }
  
  /* ADD THAT TO SEE AREAS TO UPDATE
  for(int i = 0; i < l.width; ++i){ // Iteration over the columns
    for(int j = 0; j < l.height; ++j){ // Iteration over areas in this column
      screenImage.setPixel(i, j, l.p[i][j].color);
    }
  }
  
  for(unsigned int i = 0; i < l.atu.size(); ++i){ // Iteration over the columns
    for(std::list<std::pair<int, int> >::iterator it = l.atu[i].begin(); it != l.atu[i].end(); it++){ // Iteration over areas in this column
      for(int j = (*it).first; j >= (*it).second; --j){ // Iteration over pixels in this area
        screenImage.setPixel(i, j, getMidColor(sf::Color::Blue, l.p[i][j].color, 0.7));
      }
    }
  }
  */
}

void Screen::writePixelToImage(Land& l, int x, int y){
  switch(l.p[x][y].fType){
    case pixelForegroundType_AIR:
      screenImage.setPixel(x, y, l.p[x][y].color);
    break;
    case pixelForegroundType_WATER:
      if(l.p[x][y].type == pixelType_NONE)
        screenImage.setPixel(x, y, sf::Color(119, 135, 255));
      else
        screenImage.setPixel(x, y, getMidColor(sf::Color(119, 135, 255), l.p[x][y].color, 0.2));
    break;
    case pixelForegroundType_STONE:
      screenImage.setPixel(x, y, sf::Color(80, 75, 107));
    break;
    default: break;
  }
}

void Screen::writeImageToTexture(){
  screenTexture.update(screenImage);
}

void Screen::writeTextureToWindow(){
  window.draw(screenSprite);
}
