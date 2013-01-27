#include "Toolbar.hpp"

Toolbar::Toolbar(){
  
}

Toolbar::~Toolbar(){
  
}

void Toolbar::init(int x, int y, int rX, int rY, int width, int height, float _retractRatio){
  normalPosition.x = x;
  normalPosition.y = y;
  retractedPosition.x = rX;
  retractedPosition.y = rY;
  size.x = width;
  size.y = height;
  retractRatio = _retractRatio;
  
  setPositionDependingOnRetractRatio();
  retractState = toolbarRetractState_NOTHING;
  
  halfsize.x = size.x/2;
  halfsize.y = size.y/2;
}

void Toolbar::addButton(ImageManager& im, std::string filename, MapStringStringIterator text, int x, int y, pixelType pixelTypeToSelect, bool displayed){
  addButton(im, filename, x, y, pixelTypeToSelect, displayed); // Add the button with no text
  buttons[buttons.size()-1].setText(text); // And add the text
}

void Toolbar::addButton(ImageManager& im, std::string filename, int x, int y, pixelType pixelTypeToSelect, bool displayed){
  buttons.push_back(ToolbarButton(im, filename, x, y, pixelTypeToSelect, displayed)); // Add the button
}

void Toolbar::loop(){
  switch(retractState){
    case toolbarRetractState_NOTHING: break;
    case toolbarRetractState_TOWARD_NORMAL_POSITION:
      retractRatio -= 0.1;
      if(correctRetractRatio()) retractState = toolbarRetractState_NOTHING;
      setPositionDependingOnRetractRatio();
    break;
    case toolbarRetractState_TOWARD_RETRACTED_POSITION:
      retractRatio += 0.1;
      if(correctRetractRatio()) retractState = toolbarRetractState_NOTHING;
      setPositionDependingOnRetractRatio();
    break;
  }
}

void Toolbar::retract(bool yes){
  if(yes) retractState = toolbarRetractState_TOWARD_RETRACTED_POSITION;
  else retractState = toolbarRetractState_TOWARD_NORMAL_POSITION;
}

bool Toolbar::correctRetractRatio(){
  if(retractRatio > 1){
    retractRatio = 1;
    return true;
  }
  else if(retractRatio < 0){
    retractRatio = 0;
    return true;
  }
  
  return false;
}

void Toolbar::leftButtonPressed(God& g, sf::Vector2i mousePosition){
  for(unsigned int i = 0; i < buttons.size(); ++i){
    // If the mouse is out of the button, we do nothing
    if(mousePosition.x < halfsize.x + buttons[i].center.x - buttons[i].halfsize.x)
      ;
    else if(mousePosition.x > halfsize.x + buttons[i].center.x + buttons[i].halfsize.x)
      ;
    else if(mousePosition.y < halfsize.y + buttons[i].center.y - buttons[i].halfsize.y)
      ;
    else if(mousePosition.y > halfsize.y + buttons[i].center.y + buttons[i].halfsize.y)
      ;
    else{ // Else, it meanes we clicked the button! We change god's selected pixel type
      g.pixelTypeSelected = buttons[i].pixelTypeToSelect;
    }
  }
}

void Toolbar::mouseMoved(sf::Vector2i mousePosition){
  if(mousePosition.y <= normalPosition.y){
    retract(true);
  }
  else if(mousePosition.y > position.y + size.y){
    retract(false);
  }
}

bool Toolbar::isCursorUsed(sf::Vector2i mousePosition){
  if(mousePosition.x > position.x + size.x)
    return false;
  else if(mousePosition.x < position.x)
    return false;
  else if(mousePosition.y > position.y + size.y)
    return false;
  else if(mousePosition.y < position.y)
    return false;
    
  return true;
}

void Toolbar::setPositionDependingOnRetractRatio(){
  position.x = getMidInteger(normalPosition.x, retractedPosition.x, retractRatio);
  position.y = getMidInteger(normalPosition.y, retractedPosition.y, retractRatio);
}
