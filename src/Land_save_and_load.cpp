#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

#include "Land.hpp"

void Land::saveGame(std::string filename){
  std::ofstream ofs(filename);
  boost::archive::binary_oarchive oa(ofs);
  oa.register_type<Seed>();
  oa.register_type<Fruit>();
  oa << width;
  oa << height;
  oa << entities;
  oa << g;
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < height; ++j){
      oa << p[i][j];
    }
  }
}

void Land::loadGame(std::string filename){
  // Load serialized data
  std::ifstream ifs(filename);
  boost::archive::binary_iarchive ia(ifs);
  ia.register_type<Seed>();
  ia.register_type<Fruit>();
  ia >> width;
  ia >> height;
  ia >> entities;
  ia >> g;
  init(width, height); // Init land (resize the vector)
  for(int i = 0; i < width; ++i){
    for(int j = 0; j < height; ++j){
      ia >> p[i][j];
    }
  }
  
  // Other stuff
  frame_id = 0;
}
