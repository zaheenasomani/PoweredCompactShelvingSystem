#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "message.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const Message_t& msg) {
  //DEFINE THE os OPERATOR FOR YOUR MESSAGE
  /*Example*/
  os << msg.port << " " << msg.value;
  
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, Message_t& msg) {
  //DEFINE THE is OPERATOR FOR YOUR MESSAGE
  /*Example*/
  is >> msg.port;
  is >> msg.value;
  
  return is;
}
