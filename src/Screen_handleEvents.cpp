#include "Screen.hpp"

void Screen::handleEvents(Land& l, God& g, Interface& i, bool& exit){
  bool justPressedLeftButton = false, justPressedSpace = false, thingsWereAlreadyAdded = false;
  int mouseMovementX = 0, mouseMovementY = 0;
  int landCursorMovementX = 0, landCursorMovementY = 0;
  
  // Active events
  while(window.pollEvent(event)){
    switch(event.type){
      case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button){
          case sf::Mouse::Left:
            justPressedLeftButton = true;
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
            else if(viewZoom == 0.5) changeZoom(1);
            sf::Mouse::setPosition(getTheoreticalMousePosition());
          break;
          case sf::Keyboard::Up:
            moveCursor(sf::Vector2f(0, -4));
            correctCursorPosition();
            i.theoreticalMouseMoved(getTheoreticalMousePosition());
            createLandCursorPosition();
            correctLandCursorPosition(l);
          break;
          case sf::Keyboard::Down:
            moveCursor(sf::Vector2f(0, 4));
            correctCursorPosition();
            i.theoreticalMouseMoved(getTheoreticalMousePosition());
            createLandCursorPosition();
            correctLandCursorPosition(l);
          break;
          case sf::Keyboard::Left:
            moveCursor(sf::Vector2f(-4, 0));
            correctCursorPosition();
            i.theoreticalMouseMoved(getTheoreticalMousePosition());
            createLandCursorPosition();
            correctLandCursorPosition(l);
          break;
          case sf::Keyboard::Right:
            moveCursor(sf::Vector2f(4, 0));
            correctCursorPosition();
            i.theoreticalMouseMoved(getTheoreticalMousePosition());
            createLandCursorPosition();
            correctLandCursorPosition(l);
          break;
          case sf::Keyboard::Space:
            justPressedSpace = true;
          break;
          default: // If we pressed any other letter
            g.pressedALetter(event.key.code); // We notify the god
          break;
        }
      break;
      case sf::Event::MouseWheelMoved:
        g.movedMouseWheel(event.mouseWheel.delta);
      break;
      case sf::Event::MouseMoved:
        // Calculate relative movement of the mouse
        mouseMovementX = event.mouseMove.x - mousePosition.x;
        mouseMovementY = event.mouseMove.y - mousePosition.y;
        mousePosition.x = event.mouseMove.x;
        mousePosition.y = event.mouseMove.y;
        // If the mouse isn't in the center
        if(mouseMovementX != 0 || mouseMovementY != 0){
          // Store the old land cursor position
            sf::Vector2f oldLandCursorPosition(landCursorPosition);
          // If we're pressing the right button
          if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
            // Move the view relatively to the mouse movement
            view.move(sf::Vector2f(-mouseMovementX, -mouseMovementY));
            correctViewPosition();
            window.setView(view);
            cursorPosition = (sf::Vector2f)getMouseCursorPosition();
          }
          else{
            // Move the cursor relatively to the mouse movement
            cursorPosition = (sf::Vector2f)getMouseCursorPosition();
          }
          // Correct cursorPosition and recalculate landCursorPosition
          correctCursorPosition();
          i.theoreticalMouseMoved(getTheoreticalMousePosition());
          createLandCursorPosition();
          correctLandCursorPosition(l);
          // Calculate relative movement of the land cursor
          landCursorMovementX += landCursorPosition.x - oldLandCursorPosition.x;
          landCursorMovementY += landCursorPosition.y - oldLandCursorPosition.y;
        }
      break;
      case sf::Event::Closed:
        exit = true;
      break;
      default: break;
    }
  }
  
  // Check if the cursor is used by the interface
  cursorIsUsedByTheInterface = i.isCursorUsed(getTheoreticalMousePosition());
  
  // If the cursor isn't used by the interface, make god add things
  if(cursorIsUsedByTheInterface == false){
    // If she just pressed space
    if(justPressedSpace){
      g.addThings(l, landCursorPosition.x, landCursorPosition.y, true, landCursorMovementX, landCursorMovementY);
      thingsWereAlreadyAdded = true;
    }
    // If she's pressing space and didn't just pressed it
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
      g.addThings(l, landCursorPosition.x, landCursorPosition.y, false, landCursorMovementX, landCursorMovementY);
      thingsWereAlreadyAdded = true;
    }
    // If we didn't already add things because of the space bar
    if(thingsWereAlreadyAdded == false){
      // If she just pressed the left button
      if(justPressedLeftButton){
        g.addThings(l, landCursorPosition.x, landCursorPosition.y, true, landCursorMovementX, landCursorMovementY);
      }
      // If she's pressing the left button and didn't just pressed it
      else if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        g.addThings(l, landCursorPosition.x, landCursorPosition.y, false, landCursorMovementX, landCursorMovementY);
      }
    }
  }
  else{ // Else, the cursor is used by the interface
    if(justPressedLeftButton){
      i.leftButtonPressed(g, getTheoreticalMousePosition());
    }
  }
  
  // Cursor locking
  if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){
    if(cursorLockedState == cursorLockedState_NOT_LOCKED)
      cursorLockedState = cursorLockedState_LOCKED_BUT_WE_DONT_KNOW_HOW;
  }
  else cursorLockedState = cursorLockedState_NOT_LOCKED;
}
