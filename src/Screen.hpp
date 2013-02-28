#ifndef HPP_SCREEN
#define HPP_SCREEN

#include <cmath>
#include <stdexcept>
#include <string>

#include <boost/lexical_cast.hpp>

#include <SFML/Graphics.hpp>

#include "ConfigData.hpp"
#include "God.hpp"
#include "ImageManager.hpp"
#include "Interface.hpp"
#include "Land.hpp"

class Interface;
class Toolbar;
class ToolbarButton;

typedef enum{
  cursorLockedState_NOT_LOCKED,
  cursorLockedState_LOCKED_BUT_WE_DONT_KNOW_HOW,
  cursorLockedState_LOCKED_HORIZONTALLY,
  cursorLockedState_LOCKED_VERTICALLY,
  cursorLockedState_LOCKED_BY_RIGHT_CLICKING
}cursorLockedStateEnum;

class Screen{
  public:
    Screen();
    ~Screen();
    
    // Initalization
    void init(ConfigData&);
    
    // Drawing
    void draw(Land&, God&, Interface&);

    // Events    
    void handleEvents(Land&, God&, Interface& i, bool&);
    
    // "get" functions
    int getWidth(){ return width; }
    int getHeight(){ return height; }
    
    // Image manager
    ImageManager im;
    
    // Functions
    void adaptToLand(Land&, bool = false); // If the boolean is true, it means this is the first time we adapt to the land
    
  private:
    // Window
    int width, height;
    sf::RenderWindow window;
    void writeTextureToWindow();
    
    // Texture
    sf::Texture screenTexture;
    sf::Sprite screenSprite; // The sprite containing the texture
    void writeImageToTexture();
    
    // Image
    int imageWidth, imageHeight;
    sf::Image screenImage;
    void writeLandToImage(Land&);
    void writePixelToImage(Land&, int, int);
    
    // Land view, just called "view"
    sf::View view;
    float viewZoom;
    void changeZoom(float);
    void correctViewPosition(); // Possibly correct view position to avoid the view being outside of the land
    void useNormalView();
    sf::Vector2f viewIdealCenter, viewIdealSize;
    
    // Interface view
    sf::View interfaceView;
    void useInterfaceView();
    
    // Events
    sf::Event event;
    
    // Time
    sf::Clock fpsClock;
    sf::Text fpsText;
    int fpsClockStep;
    
    // Fonts
    sf::Font defaultFont;
    
    // Mouse
    sf::Vector2f getMouseCursorPosition(); // Get cursor position relatively to the mouse
    sf::Vector2i mousePosition;
    bool isMouseOutOfImage();
    void putMouseOnCursorCenter();
    
    // Cursor
    sf::Vector2f cursorPosition; // Precise cursor position
    void moveCursor(sf::Vector2f, bool = false);
    sf::Vector2f getCorrectCursorPositionFromThisPosition(sf::Vector2f);
    bool correctCursorPosition(); // Return true if it was corrected
    
    sf::Vector2i landCursorPosition; // Position of the cursor as it will modify land pixels
    sf::Vector2i getLandCursorPositionFromThisPosition(sf::Vector2f);
    void createLandCursorPosition();
    void correctLandCursorPosition(Land&);
    
    bool cursorIsUsedByTheInterface;
    
    // Cursor locking
    cursorLockedStateEnum cursorLockedState;
    
    // Drawing functions
    void prepareDrawing();
    void drawLand(Land&);
    void drawInterface(Interface&, God&);
    void drawToolbar(Toolbar&, God&);
    void drawToolbarButton(ToolbarButton&, sf::Vector2f, God&);
    void drawCursor(God&);
    void drawNormalCursor(God&);
    void drawInterfaceCursor();
    void drawFPS();
    void endDrawing();
};

#endif
