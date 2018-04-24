#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** Message_t ****************/
/*******************************************/
struct Message_t{

  Message_t(){}
  //DEFINE THE CONSTRUCTOR OF YOUR MESSAGE
  /*Example*/
  Message_t(string i_port, float i_value)
   :port(i_port), value(i_value){}


  //DEFINE YOUR MESSAGE STRUCTURE FILDS
   /*Example*/
  string  port;
  float   value;
	
  
  void clear() {
   Message_t(); 
  }
};

istream& operator>> (istream& is, Message_t& msg);

ostream& operator<<(ostream& os, const Message_t& msg);


#endif // BOOST_SIMULATION_MESSAGE_HPP
