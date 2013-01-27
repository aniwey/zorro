#include "algo.hpp"

#include <iostream>

int getMidInteger(int a, int b, float gradient){
  return std::min(a, b) + abs(a-b)*gradient;
}
