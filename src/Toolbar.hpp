#ifndef HPP_TOOLBAR
#define HPP_TOOLBAR

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "algo.hpp"
#include "ConfigData.hpp"
#include "God.hpp"
#include "ImageManager.hpp"
#include "ToolbarButton.hpp"

typedef enum{
  toolbarRetractState_NOTHING,
  toolbarRetractState_TOWARD_NORMAL_POSITION,
  toolbarRetractState_TOWARD_RETRACTED_POSITION
}toolbarRetractState;

class Toolbar{
  public:
    Toolbar();
    ~Toolbar();
    
    void init(int, int, int, int, int, int, float);
    void loop(); // Called every global loop, it handles retract ratio and state
    
    // Functions called when some events occur to notify the toolbar
    void leftButtonPressed(God&, sf::Vector2i);
    void theoreticalMouseMoved(sf::Vector2i);
    
    // Other functions
    bool isCursorUsed(sf::Vector2i); // Return true if the toolbar is currently using the cursor
    
    // Position, size, etc
    sf::Vector2f position, normalPosition, retractedPosition;
    sf::Vector2f size, halfsize;
    
    // Buttons and functions to manipulate them
    std::vector<ToolbarButton> buttons;
    void addButton(ImageManager&, std::string, MapStringStringIterator, int, int, pixelType, bool);
    void addButton(ImageManager&, std::string, int, int, pixelType, bool);
    
  private:
    // Retract ratio and state
    float retractRatio;
    toolbarRetractState retractState;
  
    // Functions
    bool correctRetractRatio(); // Return true if a correction was needed
    void setPositionDependingOnRetractRatio();
    void retract(bool); // Change retract state
};

#endif
