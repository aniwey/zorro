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
  
  // Other sizes
  imageWidth = width/4;
  imageHeight = height/4;

  // Window
  if(mainConfig.getString("fullscreen") == "yes"){ // If fullscreen mode is set to "yes" in the config, we activate fullscreen
    window.create(sf::VideoMode(width, height), "zorro2", sf::Style::Fullscreen);
  }
  else{ // Else, no fullscreen
    window.create(sf::VideoMode(width, height), "zorro2");
  }
  window.setFramerateLimit(60);
  window.setMouseCursorVisible(false);
  
  // Image, sprite and texture
  screenImage.create(imageWidth, imageHeight);
  screenSprite.scale(4, 4);
  screenTexture.loadFromImage(screenImage);
  
  // Land view
  view.setCenter(width/2, height/2);
  view.setSize(width, height);
  window.setView(view);
  viewZoom = 1;
  
  // Interface view
  interfaceView.setCenter(width/2, height/2);
  interfaceView.setSize(width, height);
  
  // Cursor
  cursorPosition = getMouseCursorPosition();
  correctCursorPosition();
  createLandCursorPosition();
  cursorIsUsedByTheInterface = false;
  
  // Cursor locking
  cursorLockedState = cursorLockedState_NOT_LOCKED;
  
  // Mouse
  mousePosition = sf::Mouse::getPosition();
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
  if(viewZoom <= 1){ // If the zoom is normal or < 1, then we need to correct the view. Else, it means that we actually want to go out of the land by unzooming
    int x = view.getCenter().x - (view.getSize().x/2), y = view.getCenter().y - (view.getSize().y/2); // View coordinates
    if(x < 0) view.move(-x, 0);
    if(y < 0) view.move(0, -y);
    if(x + view.getSize().x > width) view.move(width - x - view.getSize().x, 0);
    if(y + view.getSize().y > height) view.move(0, height - y - view.getSize().y);
  }
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

void Screen::correctCursorPosition(){
  cursorPosition = getCorrectCursorPositionFromThisPosition(cursorPosition);
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
  if(landCursorPosition.x > l.getWidth()-1) landCursorPosition.x = l.getWidth()-1;
  if(landCursorPosition.y > l.getHeight()-1) landCursorPosition.y = l.getHeight()-1;
}

void Screen::useInterfaceView(){
  window.setView(interfaceView);
}

void Screen::useNormalView(){
  window.setView(view);
}
