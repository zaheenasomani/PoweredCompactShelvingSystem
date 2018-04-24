/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* CalculateAisles:
* CalculateAisles model CDBoost implementation
*/

#ifndef BOOST_SIMULATION_PDEVS_CALCAISLES_HPP //You need to change ATOMIC by your model's name
#define BOOST_SIMULATION_PDEVS_CALCAISLES_HPP //You need to change ATOMIC by your model's name

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
 * @class calcAisles
*/

template<class TIME, class MSG>
class calcAisles : public pdevs::atomic<TIME, MSG>{ //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC MODEL.
private:

  //DECLARE YOUR MODEL'S PARAMETRES
  
  /* example */
   string id;
   string onReset;
   string update;
   TIME preparationTime;

  
  //DECLARE YOUR MODEL'S STATE VARIABLES
  
  /* example */
  //bool state;
  //int packet;
  //int index;
  int count;
  int numAisles;
  int currentAisle;
  int aisleToMove; 
  bool ifupdate;
  //bool model_active;
  TIME next_internal;

public:

  /**
   * @constructor 
   */

  explicit calcAisles(/*PARAMETERS TO INITIALIZE THE MODEL AND THE STATE VARIABLES IF ANY*/ /*example: string ID*/) noexcept {   //CHANGE AtomicModelName TO THE NAME OF YOUR ATOMIC
    
    //INITIALIZE THE MODEL PARAMETERS AND VARIABLES. YOU CAN USE THE PARAMETERS IN THE CONSTRUCTOR, IF ANY
    /* example*/
    id                = string("CalculateAisles");
    //state           = 0;
    //packet          = -8;
    //index           = 0;
    onReset	     = string("onReset");
    update	     = string("Update");
    numAisles 	     = 10;
    preparationTime  = 15;
    next_internal    = pdevs::atomic<TIME, MSG>::infinity;

    //model_active     = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {  
    
    //DEFINE YOU INTERNAL FUNCTION HERE 
    ifupdate = false;   
  }

  /**
   * @advance
   */

  TIME advance() const noexcept { 
    TIME next_internal;
    
    //DEFINE YOUR TIME ADVANCE FUNCTION HERE
    //WARNING: IT CANNOT MODIFY ANY OF THE STATE VARIABLES OF THE MODEL. IT IS AGAINTS DEVS FORMALISM. THE MODEL WILL NOT COMPILE
    if (ifupdate) {
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

    if(ifupdate)
	{
         aux.port = update;
         aux.value = aisleToMove;
         out_put.push_back(aux);
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
      if( mb[i].port == onReset){
	currentAisle = static_cast < int > (mb[i].value);
        aisleToMove = numAisles - currentAisle;        
	ifupdate = true;
        next_internal = TIME(0);    
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
    external (mb, 0);
  }
};

#endif // BOOST_SIMULATION_PDEVS_CALCAISLES_HPP //You need to change ATOMIC by your model's name
