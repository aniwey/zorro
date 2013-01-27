#ifndef HPP_CONFIGDATA
#define HPP_CONFIGDATA

#include <fstream>
#include <iostream>
#include <map>
#include <string>

typedef std::map<std::string, std::string>::iterator MapStringStringIterator;

class ConfigData{
  public:
    ConfigData();
    ~ConfigData();
    
    void readFromFile(std::string);
    
    MapStringStringIterator getIterator(std::string key);
    std::string getString(std::string key);
    
  private:
    std::map<std::string, std::string> data;
};

#endif
