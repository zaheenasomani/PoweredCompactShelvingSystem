/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* Timer:
* Timer model CDBoost implementation
*/

#ifndef BOOST_SIMULATION_PDEVS_TIMER_HPP //You need to change ATOMIC by your model's name
#define BOOST_SIMULATION_PDEVS_TIMER_HPP //You need to change ATOMIC by your model's name

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
#include <random>
#include <boost/simulation/pdevs/atomic.hpp>

//#include any other library you need

#include "../data_structures/message.hpp" //Path to your message structure file. If you follow the folder template and the template name, you do not need to change it.

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;


/**
 * @class ExitSensor
*/

template<class TIME, class MSG>
class timer : public pdevs::atomic<TIME, MSG>{ //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC MODEL.
private:

  //DECLARE YOUR MODEL'S PARAMETRES
  
  /* example */
   string id;
   //string walk_out;
   //string decr;
   string lightOn;
   TIME preparationTime;
   TIME doorTimer;

  
  //DECLARE YOUR MODEL'S STATE VARIABLES
  
  /* example */
  //bool state;
  //int packet;
  //int index;
  int count;
  bool ifaisleOpen;
  //bool model_active;
  TIME next_internal;

public:

  /**
   * @constructor 
   */

  explicit timer(/*PARAMETERS TO INITIALIZE THE MODEL AND THE STATE VARIABLES IF ANY*/ /*example: string ID*/) noexcept {   //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC
    
    //INITIALIZE THE MODEL PARAMETERS AND VARIABLES. YOU CAN USE THE PARAMETERS IN THE CONSTRUCTOR, IF ANY
    /* example*/
    id                = string("AisleOpen");
    //state           = 0;
    //packet          = -8;
    //index           = 0;
    //walk_out	     = string("WalkOut");
    //decr	     = string("Decrement");
    lightOn	     = string("lightOn");
    preparationTime  = 15;
    doorTimer	     = 10;
    //count	     = 0;
    ifaisleOpen	     = false;
    next_internal    = pdevs::atomic<TIME, MSG>::infinity;

    //model_active     = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {  
    
    //DEFINE YOU INTERNAL FUNCTION HERE 
    ifaisleOpen = false;
  }

  /**
   * @advance
   */

  TIME advance() const noexcept { 
    TIME next_internal;
    
    //DEFINE YOUR TIME ADVANCE FUNCTION HERE
    //WARNING: IT CANNOT MODIFY ANY OF THE STATE VARIABLES OF THE MODEL. IT IS AGAINTS DEVS FORMALISM. THE MODEL WILL NOT COMPILE
    if (ifaisleOpen) {
      next_internal = preparationTime + doorTimer;
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

    aux.port = id;
    aux.value = true;
    out_put.push_back(aux);
    aux.port = lightOn;
    aux.value = true;
    out_put.push_back(aux);

    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
     //DEFINE YOU EXTERNAL FUNCTION HERE
     //WARINING: YOU RECEIVE A VECTOR OF MESSAGES. THERE CAN BE MORE THAN ONE AT THE SAME TIME.
     if(mb.size()>1){
    assert(false && "Only one message at a time");
   }else{ 
          ifaisleOpen = true;
          next_internal = TIME(0);
        }
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    //DEFINE YOUR CONFLUENCE FUNCTION HERE
    //THIS FUNCTION DEFINES WHAT HAPPENS WHEN THE INTERNAL AND THE EXTERNAL ARE SCHEDULLED FOR THE SAME TIME
    //WARINING: YOU RECEIVE A VECTOR OF MESSAGES. THERE CAN BE MORE THAN ONE AT THE SAME TIME.
    internal();
    external (mb, 0);
  }
};

#endif // BOOST_SIMULATION_PDEVS_EXITSENSOR_HPP //You need to change ATOMIC by your model's name
