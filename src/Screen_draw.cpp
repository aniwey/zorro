#include "Screen.hpp"

void Screen::prepareDrawing(){
  window.clear();
}

void Screen::draw(Land& l, God& g, Interface& i){
  prepareDrawing();
  drawLand(l);
  drawInterface(i, g);
  drawCursor(g);
  endDrawing();
}

void Screen::drawLand(Land& l){
  writeLandToImage(l);
  writeImageToTexture();
  writeTextureToWindow();
}

void Screen::drawInterface(Interface& i, God& g){
  useInterfaceView();
  
  // Draw toolbars
    // Draw main toolbar
    drawToolbar(i.mainToolbar, g);

  useNormalView();
}

void Screen::drawToolbar(Toolbar& toolbar, God& g){
  // Draw toolbar's background
  sf::RectangleShape rect(toolbar.size);
  rect.setPosition(toolbar.position);
  rect.setFillColor(sf::Color(50, 50, 50, 128));
  window.draw(rect);
  
  // Draw toolbar's buttons
  for(unsigned int i = 0; i < toolbar.buttons.size(); ++i){
    drawToolbarButton(toolbar.buttons[i], toolbar.position + toolbar.halfsize, g);
  }
}

void Screen::drawToolbarButton(ToolbarButton& button, sf::Vector2f toolbarOrigin, God& g){
  // Draw button's image
  sf::Sprite spr;
  sf::Texture txt;
  spr.setPosition(toolbarOrigin + button.center - button.halfsize);
  txt.loadFromImage(*im.getImage(button.filename));
  spr.setTexture(txt);
  window.draw(spr);
  
  // Draw button's text
  if(button.hasText){
    sf::Text text(button.text->second, defaultFont, 12);
    text.setColor(sf::Color::Black);
    text.setPosition(toolbarOrigin + button.center + sf::Vector2f(-text.getGlobalBounds().width/2, -button.halfsize.y + button.imageSize.y));
    window.draw(text);
  }
  
  // Draw rectangle around if selected
  if(g.pixelTypeSelected == button.pixelTypeToSelect){
    sf::RectangleShape rect(button.size);
    rect.setPosition(toolbarOrigin + button.center - button.halfsize);
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    rect.setOutlineThickness(2);
    rect.setOutlineColor(sf::Color::Black);
    window.draw(rect);
  }
}

void Screen::drawCursor(God& g){
  // Calculate the gap we need to substract from currentLandCursorPosition.x*4 in order to draw the cursor at the correct place
  // It must take in account parity of pixelAddingDiameter (which explains the % 2)
  float outline = 1 * viewZoom;
  int size = g.getAddingDiameterDependingOnPixelTypeSelected(), gap;
  if(size % 2 == 1) gap = (size-1) * 2;
  else gap = size * 2;
  
  if(cursorIsUsedByTheInterface == false){ // If the cursor isn't currently used by the interface, we draw the normal cursor
    // Draw the point inside the rectangle
    sf::Vertex vertices[] = { sf::Vertex(cursorPosition, sf::Color(0, 0, 0)) };
    window.draw(vertices, 1, sf::Points);
  
    // Draw the rectangle
    sf::RectangleShape rect(sf::Vector2f(size*4 - outline*2, size*4 - outline*2));
    rect.setPosition(landCursorPosition.x*4 - gap + outline, landCursorPosition.y*4 - gap + outline);
    rect.setFillColor(sf::Color(0, 0, 0, 0));
    rect.setOutlineColor(sf::Color(0, 0, 0));
    rect.setOutlineThickness(outline);
    window.draw(rect);
  }
  else{ // Else, we draw the interface cursor
    sf::Vertex vertices[] = { sf::Vertex(sf::Vector2f(cursorPosition.x - 5, cursorPosition.y - 5), sf::Color(0, 0, 0)),
                              sf::Vertex(sf::Vector2f(cursorPosition.x + 5, cursorPosition.y + 5), sf::Color(0, 0, 0)),
                              sf::Vertex(sf::Vector2f(cursorPosition.x + 5, cursorPosition.y - 5), sf::Color(0, 0, 0)),
                              sf::Vertex(sf::Vector2f(cursorPosition.x - 5, cursorPosition.y + 5), sf::Color(0, 0, 0)) };
    window.draw(vertices, 4, sf::Lines);
  }
}

void Screen::endDrawing(){
  window.display();
}

