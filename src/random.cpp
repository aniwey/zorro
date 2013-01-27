#include "random.hpp"

void initRandom(){
  srand(time(NULL));
}

int randomBetween(int a, int b){
  return a + (rand() % (b-a));
}
