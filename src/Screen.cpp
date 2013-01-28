#include "Screen.hpp"

Screen::Screen(){
  // Fonts
  defaultFont.loadFromFile("./fonts/LiberationMono-Regular.ttf");
}

Screen::~Screen(){
  window.close();
}

void Screen::init(ConfigData& mainConfig){
  // Width and height
  width = 800;
  height = 600;
  try{
    width = std::stoi(mainConfig.getString("resolution_width"));
    height = std::stoi(mainConfig.getString("resolution_height"));
  }
  catch(std::invalid_argument& ia){
	  std::cerr << "Bad resolution (invalid argument) : " << ia.what() << std::endl;
  }
  catch(std::out_of_range& oor){
    std::cerr << "Bad resolution (out of range) : " << oor.what() << std::endl;
  }

  // Window
  if(mainConfig.getString("fullscreen") == "yes"){ // If fullscreen mode is set to "yes" in the config, we activate fullscreen
    window.create(sf::VideoMode(width, height), "zorro2", sf::Style::Fullscreen);
  }
  else{ // Else, no fullscreen
    window.create(sf::VideoMode(width, height), "zorro2");
  }
  window.setFramerateLimit(60);
  window.setMouseCursorVisible(false);
  
  // Interface view
  interfaceView.setCenter(width/2, height/2);
  interfaceView.setSize(width, height);
  
  // Cursor locking
  cursorLockedState = cursorLockedState_NOT_LOCKED;
  
  // Mouse
  mousePosition = sf::Mouse::getPosition();
}

void Screen::adaptToLand(Land& l){
  // Land view
  viewIdealCenter = sf::Vector2f(l.width*2, l.height*2);
  view.setCenter(viewIdealCenter);
  viewIdealSize = sf::Vector2f(l.width*4, l.height*4);
  view.setSize(width, height);
  window.setView(view);
  viewZoom = 1;
  
  // Image size
  imageWidth = l.width;
  imageHeight = l.height;
  
  // Image, sprite and texture
  screenImage.create(imageWidth, imageHeight);
  screenSprite.setScale(4, 4);
  screenTexture.loadFromImage(screenImage);
  screenSprite.setTexture(screenTexture);
  
  // Cursor
  cursorPosition = getMouseCursorPosition();
  correctCursorPosition();
  createLandCursorPosition();
  cursorIsUsedByTheInterface = false;
}

void Screen::changeZoom(float new_zoom){
  view.setCenter(cursorPosition); // We set the view center under the cursor
  view.zoom(new_zoom/viewZoom); // We change zoom the view using the ratio new/old
  correctViewPosition(); // We correct the view position to avoind it being out of the land
  window.setView(view); // We apply the view to the window
  viewZoom = new_zoom; // We save the new zoom
}

sf::Vector2f Screen::getMouseCursorPosition(){
  return window.mapPixelToCoords(sf::Mouse::getPosition());
}

sf::Vector2i Screen::getTheoreticalMousePosition(){
  return window.mapCoordsToPixel(cursorPosition);
}

void Screen::correctViewPosition(){
  // If the view ideal size is larger than the real size on x
  if(viewIdealSize.x > view.getSize().x){
    // If the view is too much on the left
    if(view.getCenter().x - view.getSize().x/2 < viewIdealCenter.x - viewIdealSize.x/2)
      view.setCenter(view.getSize().x/2, view.getCenter().y);
    // Too much on the right
    if(view.getCenter().x + view.getSize().x/2 > viewIdealCenter.x + viewIdealSize.x/2)
      view.setCenter(viewIdealSize.x - view.getSize().x/2, view.getCenter().y);
  }
  else view.setCenter(viewIdealCenter.x, view.getCenter().y);
  
  // If the view ideal size is larger than the real size on y
  if(viewIdealSize.y > view.getSize().y){
    // If the view is too much on the top
    if(view.getCenter().y - view.getSize().y/2 < viewIdealCenter.y - viewIdealSize.y/2)
      view.setCenter(view.getCenter().x, view.getSize().y/2);
    // Too much on the bottom
    if(view.getCenter().y + view.getSize().y/2 > viewIdealCenter.y + viewIdealSize.y/2)
      view.setCenter(view.getCenter().x, viewIdealSize.y - view.getSize().y/2);
  }
  else view.setCenter(view.getCenter().x, viewIdealCenter.y);
}

bool Screen::isMouseOutOfImage(){
  sf::Vector2f pos = getMouseCursorPosition();
  // If the mouse is too much on the left
  if(pos.x < viewIdealCenter.x - viewIdealSize.x/2)
    return true;
  if(pos.x > viewIdealCenter.x + viewIdealSize.x/2)
    return true;
  if(pos.y < viewIdealCenter.y - viewIdealSize.y/2)
    return true;
  if(pos.y > viewIdealCenter.y + viewIdealSize.y/2)
    return true;
    
  return false;
}

void Screen::moveCursor(sf::Vector2f move, bool doNotTakeCursorLockingInAccount){
  // Calculate how much the land cursor would move after this cursor movement
  sf::Vector2i movement_on_land = getLandCursorPositionFromThisPosition(getCorrectCursorPositionFromThisPosition(cursorPosition + move)) - getLandCursorPositionFromThisPosition(cursorPosition);
  
  // If we take in account cursor locking and it isn't lock, then we may modify the move vector
  if(doNotTakeCursorLockingInAccount == false && cursorLockedState != cursorLockedState_NOT_LOCKED){
    // If the cursor is locked but we don't know how
    if(cursorLockedState == cursorLockedState_LOCKED_BUT_WE_DONT_KNOW_HOW){
      // If, taking in account our previsions, the land cursor would have mainly move horizontally if it wasn't locked, then we lock it horizontally
      if(movement_on_land.x != 0 && abs(movement_on_land.x) >= abs(movement_on_land.y))
        cursorLockedState = cursorLockedState_LOCKED_HORIZONTALLY;
      // Else, if it would have moved vertically, then we move it vertically
      else if(movement_on_land.y)
        cursorLockedState = cursorLockedState_LOCKED_VERTICALLY;
    }
    // If the cursor is locked horizontally, we don't let it moving on the y axis
    if(cursorLockedState == cursorLockedState_LOCKED_HORIZONTALLY)
      move.y = 0;
    // If the cursor is locked vertically, we don't let it moving on the x axis
    else if(cursorLockedState == cursorLockedState_LOCKED_VERTICALLY)
      move.x = 0;
  }
  
  // We finally apply the movement
  cursorPosition += move;
}

sf::Vector2f Screen::getCorrectCursorPositionFromThisPosition(sf::Vector2f pos){
  // Avoid cursor going out of the view
  float left = view.getCenter().x - view.getSize().x/2;
  float top = view.getCenter().y - view.getSize().y/2;
  float right = view.getCenter().x + view.getSize().x/2;
  float bottom = view.getCenter().y + view.getSize().y/2;
  
  if(pos.x < left) pos.x = left;
  if(pos.y < top) pos.y = top;
  if(pos.x > right) pos.x = right;
  if(pos.y > bottom) pos.y = bottom;
  
  return pos;
}

bool Screen::correctCursorPosition(){
  sf::Vector2f pos = getCorrectCursorPositionFromThisPosition(cursorPosition);
  if(pos != cursorPosition){
    cursorPosition = pos;
    return true;
  }
  return false;
}

sf::Vector2i Screen::getLandCursorPositionFromThisPosition(sf::Vector2f pos){
  pos.x /= 4;
  pos.y /= 4;
  return (sf::Vector2i)pos;
}

void Screen::createLandCursorPosition(){
  landCursorPosition = getLandCursorPositionFromThisPosition(cursorPosition);
}

void Screen::correctLandCursorPosition(Land& l){
  if(landCursorPosition.x < 0) landCursorPosition.x = 0;
  if(landCursorPosition.y < 0) landCursorPosition.y = 0;
  if(landCursorPosition.x > l.width-1) landCursorPosition.x = l.width-1;
  if(landCursorPosition.y > l.height-1) landCursorPosition.y = l.height-1;
}

void Screen::useInterfaceView(){
  window.setView(interfaceView);
}

void Screen::useNormalView(){
  window.setView(view);
}
