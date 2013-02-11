#include "Screen.hpp"

void Screen::handleEvents(Land& l, God& g, Interface& i, bool& exit){
  sf::Vector2f newCursorPosition = cursorPosition;
  sf::Vector2i oldLandCursorPosition;
  bool activeAddingEvent = false;
  bool putMouseOnCursor = false;

  // If the cursor was locked by right clicking during the previous loop but we're not right clicking anymore
  if(cursorLockedState == cursorLockedState_LOCKED_BY_RIGHT_CLICKING && sf::Mouse::isButtonPressed(sf::Mouse::Right) == false){
    // We move the mouse on the cursor, which we unlock
    putMouseOnCursorCenter();
    cursorLockedState = cursorLockedState_NOT_LOCKED;
  }
  
  // If the cursor was locked by CTRL but we don't press this key anymore
  if(cursorLockedState != cursorLockedState_LOCKED_BY_RIGHT_CLICKING && cursorLockedState != cursorLockedState_NOT_LOCKED && sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) == false){
    // We move the mouse on the cursor, which we unlock
    putMouseOnCursorCenter();
    cursorLockedState = cursorLockedState_NOT_LOCKED;
  }

  // Active events
  while(window.pollEvent(event)){
    switch(event.type){
      case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button){
          case sf::Mouse::Left:
            activeAddingEvent = true;
          break;
          default: break;
        }
      break;
      case sf::Event::KeyPressed:
        switch(event.key.code){
          case sf::Keyboard::Escape: // If we pressed escape
            exit = true; // We quit the game
          break;
          case sf::Keyboard::Z: // If we pressed z
            if(viewZoom == 1) changeZoom(0.5);
            else if(viewZoom == 0.5) changeZoom(0.25);
            else if(viewZoom == 0.25) changeZoom(0.125);
            else if(viewZoom == 0.125) changeZoom(0.0625);
            else if(viewZoom == 0.0625) changeZoom(1);
            putMouseOnCursor = true;
          break;
          case sf::Keyboard::Up:
            newCursorPosition.y -= 1;
            putMouseOnCursor = true;
          break;
          case sf::Keyboard::Down:
            newCursorPosition.y += 1;
            putMouseOnCursor = true;
          break;
          case sf::Keyboard::Left:
            newCursorPosition.x -= 1;
            putMouseOnCursor = true;
          break;
          case sf::Keyboard::Right:
            newCursorPosition.x += 1;
            putMouseOnCursor = true;
          break;
          case sf::Keyboard::Space:
            activeAddingEvent = true;
          break;
          case sf::Keyboard::P: l.saveGame("./saves/test.txt"); break;
          case sf::Keyboard::M: l.loadGame("./saves/test.txt"); adaptToLand(l); break;
          default: // If we pressed any other letter
            g.pressedALetter(event.key.code); // We notify the god
          break;
        }
      break;
      case sf::Event::MouseWheelMoved:
        g.movedMouseWheel(event.mouseWheel.delta);
      break;
      case sf::Event::MouseMoved:
        // If we're pressing the right button
        if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
          // Move the view relatively to the mouse movement
          view.move(sf::Vector2f(mousePosition.x - event.mouseMove.x, mousePosition.y - event.mouseMove.y));
          correctViewPosition();
          window.setView(view);
          cursorLockedState = cursorLockedState_LOCKED_BY_RIGHT_CLICKING;
        }
        else{
          // Set the new cursor position : under the mouse
          newCursorPosition = (sf::Vector2f)getMouseCursorPosition();
        }
      break;
      case sf::Event::Closed:
        exit = true;
      break;
      default: break;
    }
  }
  
  // Cursor locking
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){ // If we're pressing CTRL
    if(cursorLockedState == cursorLockedState_NOT_LOCKED) // And the cursor isn't lock
      cursorLockedState = cursorLockedState_LOCKED_BUT_WE_DONT_KNOW_HOW; // Then we lock it, but we don't know how yet
  }
  
  // We save the old land cursor position
  oldLandCursorPosition = landCursorPosition;
  
  // We move the cursor if it isn't locked by right clicking
  if(cursorLockedState != cursorLockedState_LOCKED_BY_RIGHT_CLICKING){
    moveCursor(newCursorPosition - cursorPosition);
    if(correctCursorPosition()) putMouseOnCursor = true;
    i.mouseMoved(mousePosition);
    createLandCursorPosition();
    correctLandCursorPosition(l);
  }
  
  // Put mouse on cursor if needed
  if(putMouseOnCursor){
    putMouseOnCursorCenter();
  }
  
  // Store the old mouse position
  mousePosition.x = sf::Mouse::getPosition().x;
  mousePosition.y = sf::Mouse::getPosition().y;
  
  // Check if the cursor is used by the interface, only if the cursor isn't lock by right clicking
  if(cursorLockedState != cursorLockedState_LOCKED_BY_RIGHT_CLICKING){
    cursorIsUsedByTheInterface = i.isCursorUsed((sf::Vector2i)cursorPosition);
  }
  
  // If the cursor isn't used by the interface, make god add things
  if(cursorIsUsedByTheInterface == false){
    // If god just made an active adding event
    if(activeAddingEvent){
      g.addThings(l, oldLandCursorPosition.x, oldLandCursorPosition.y, landCursorPosition.x, landCursorPosition.y, true);
    }
    // Else, if god is pressing the space bar or the left button
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Mouse::isButtonPressed(sf::Mouse::Left)){
      g.addThings(l, oldLandCursorPosition.x, oldLandCursorPosition.y, landCursorPosition.x, landCursorPosition.y, false);
    }
  }
  else{ // Else, the cursor is used by the interface
    if(activeAddingEvent){
      i.leftButtonPressed(g, (sf::Vector2i)cursorPosition);
    }
  }
}
