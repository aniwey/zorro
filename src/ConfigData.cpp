#include "ConfigData.hpp"

ConfigData::ConfigData(){
  data["not_found"] = "NOT_FOUND";
}

ConfigData::~ConfigData(){
  
}

void ConfigData::readFromFile(std::string filename){
  std::ifstream file(filename.c_str());
  
  if(file){ // If opening was successful
    std::string line;
    std::size_t equalsSignPosition;

    while(std::getline(file, line)){
      equalsSignPosition = line.find_first_of("=");
      if(equalsSignPosition != std::string::npos && equalsSignPosition != 0 && equalsSignPosition < line.length()-1){ // If the = sign was found and isn't the first or the last character of the line
        data[line.substr(0, equalsSignPosition)] = line.substr(equalsSignPosition+1);
      }
    }
  }
  else{
    std::cerr << "Error while opening the file " << filename << "." << std::endl;
  }
}

std::string ConfigData::getString(std::string key){
  if(data.find(key) != data.end())
    return data[key];
  else
    return "NOT_FOUND";
}

MapStringStringIterator ConfigData::getIterator(std::string key){
  if(data.find(key) != data.end())
    return data.find(key);
  else{
    return data.find("not_found");
  }
}
