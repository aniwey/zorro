#include <fstream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "Land.hpp"

void Land::save(std::string filename){
  std::ofstream ofs(filename);
  boost::archive::text_oarchive oa(ofs);
  oa << pixelGroupsArePositive;
  oa << p;
}

void Land::load(std::string filename){
  std::ifstream ifs(filename);
  boost::archive::text_iarchive ia(ifs);
  ia >> pixelGroupsArePositive;
  ia >> p;
}
