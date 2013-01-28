#ifndef HPP_SFML_SERIALIZATION_FUNCTIONS
#define HPP_SFML_SERIALIZATION_FUNCTIONS

#include <SFML/Graphics/Color.hpp>

namespace boost {
namespace serialization {

template<class Archive>
void serialize(Archive & ar, sf::Color & color, const unsigned int){
    ar & color.r;
    ar & color.g;
    ar & color.b;
    ar & color.a;
}

}
}

#endif
