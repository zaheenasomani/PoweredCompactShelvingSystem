/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* Counter:
* Counter model CDBoost implementation
*/

#ifndef BOOST_SIMULATION_PDEVS_COUNTER_HPP //You need to change ATOMIC by your model's name
#define BOOST_SIMULATION_PDEVS_COUNTER_HPP //You need to change ATOMIC by your model's name

#include <math.h> 
#include <assert.h>
#include <memory>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <limits>
#include <boost/simulation/pdevs/atomic.hpp>

//#include any other library you need

#include "../data_structures/message.hpp" //Path to your message structure file. If you follow the folder template and the template name, you do not need to change it.

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;


/**
 * @class Counter
*/

template<class TIME, class MSG>
class counter : public pdevs::atomic<TIME, MSG>{ //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC MODEL.
private:

  //DECLARE YOUR MODEL'S PARAMETRES
  
  /* example */
  // string id;
   string id;
   string moveLeft;
   string moveRight;
   string incr;
   string decr;
   string aisleLocked;
   string onExit;
   TIME preparationTime;
   TIME initialTime;
   TIME timeout;

  
  //DECLARE YOUR MODEL'S STATE VARIABLES
  
  /* example */
  //bool state;
  //int packet;
  //int index;
  int count;
  bool ifLocked;
  bool ifExit;
  bool model_active;
  TIME next_internal;

public:

  /**
   * @constructor 
   */

  explicit counter(/*PARAMETERS TO INITIALIZE THE MODEL AND THE STATE VARIABLES IF ANY*/ /*example: string ID*/) noexcept {   //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC
    
    //INITIALIZE THE MODEL PARAMETERS AND VARIABLES. YOU CAN USE THE PARAMETERS IN THE CONSTRUCTOR, IF ANY
    /* example*/
    id                = string("Counter");
    //state           = 0;
    //packet          = -8;
    //index           = 0;
    moveLeft	     = string("moveLeft");
    moveRight	     = string("moveRight");
    incr	     = string("incr");
    decr	     = string("decr");
    aisleLocked	     = string("AisleLocked");
    onExit	     = string("OnExit");
    preparationTime  = 2;
    initialTime	     = 10;
    timeout	     = 20;
    //count	     = 0;
    ifLocked	     = false;
    ifExit	     = false;
    next_internal    = pdevs::atomic<TIME, MSG>::infinity;

    model_active     = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {  
    
    //DEFINE YOU INTERNAL FUNCTION HERE 
    ifExit = false;
    ifLocked = false;
  }

  /**
   * @advance
   */

  TIME advance() const noexcept { 
    TIME next_internal;
    
    //DEFINE YOUR TIME ADVANCE FUNCTION HERE
    //WARNING: IT CANNOT MODIFY ANY OF THE STATE VARIABLES OF THE MODEL. IT IS AGAINTS DEVS FORMALISM. THE MODEL WILL NOT COMPILE
    if (ifExit || ifLocked) {
      next_internal = preparationTime;
    }else {
      next_internal = pdevs::atomic<TIME, MSG>::infinity;
    }    
    return next_internal;
  }

  /**
   *@output
   */

  vector<MSG> out() const noexcept {   
    MSG aux; 
    vector<MSG> out_put;
    //DEFINE YOUR OUTPUT FUNCTION HERE
    //WARNING: IT CANNOT MODIFY ANY OF THE STATE VARIABLES OF THE MODEL. IT IS AGAINTS DEVS FORMALISM. THE MODEL WILL NOT COMPILE

    if (ifLocked){
	aux.port = aisleLocked;
        aux.value = true;
        out_put.push_back(aux);
	}else{
      if (ifExit){
        aux.port = onExit;
        aux.value = true;
        out_put.push_back(aux);
      }
    }   	

    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
     //DEFINE YOU EXTERNAL FUNCTION HERE
     //WARINING: YOU RECEIVE A VECTOR OF MESSAGES. THERE CAN BE MORE THAN ONE AT THE SAME TIME.
     if(mb.size() > 1){
      assert(false && "one message per time uniti");
    }
     for(int i = 0; i < mb.size(); i++){
      if( mb[i].port == incr){
        count = 1;
        }
       else if( mb[i].port == decr) { 
        count = 0;
        }
       else if( mb[i].port == moveLeft || mb[i].port == moveRight)
	{
		if(count==1){
			ifLocked = true;
			next_internal = TIME(0);
		}
		else if(count==0){
			ifExit = true;
			next_internal = TIME(0);
		}
	}
          
      }   
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    //DEFINE YOUR CONFLUENCE FUNCTION HERE
    //THIS FUNCTION DEFINES WHAT HAPPENS WHEN THE INTERNAL AND THE EXTERNAL ARE SCHEDULLED FOR THE SAME TIME
    //WARINING: YOU RECEIVE A VECTOR OF MESSAGES. THERE CAN BE MORE THAN ONE AT THE SAME TIME.
    external (mb, t);
  }
};

#endif // BOOST_SIMULATION_PDEVS_COUNTER_HPP //You need to change ATOMIC by your model's name
