#include "color.hpp"

sf::Color getMidColor(sf::Color col1, sf::Color col2, float gradient){
  return sf::Color(getMidInteger(col1.r, col2.r, gradient), getMidInteger(col1.g, col2.g, gradient), getMidInteger(col1.b, col2.b, gradient));
}
