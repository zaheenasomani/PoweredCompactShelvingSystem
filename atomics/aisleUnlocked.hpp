/**
* Cristina Ruiz Martin
* ARSLab - Carleton University
*
* SenderCDBoost:
* CDBoost implementation of CD++ Sender atomic model
*/

#ifndef BOOST_SIMULATION_PDEVS_AISLEUNLOCKED_HPP
#define BOOST_SIMULATION_PDEVS_AISLEUNLOCKED_HPP

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

#include "../data_structures/message.hpp"

using namespace boost::simulation::pdevs;
using namespace boost::simulation;
using namespace std;

/**
 * @class Sender
*/

template<class TIME, class MSG>
class aisleUnlocked : public pdevs::atomic<TIME, MSG>{
private:

  //PARAMETRES
  string id;
  string reset;
  string onExit;
  string stop;
  string onReset;
  string aisleLocked;
  string aisleLock;
  //string dataOut;  
  TIME   preparationTime;
  TIME   timeout;

  //STATE VARIABLES
  
  bool ifExit;
  //int packetNum;
  //int totalPacketNum;
  //int alt_bit;
  bool ifReset;
  bool ifLocked;
  bool model_active;
  TIME next_internal;

public:

  /**
   * @constructor 
   */

  explicit aisleUnlocked() noexcept {   
    id               = string("AisleUnlocked");
    reset            = string("reset");
    onExit           = string("onExit");
    stop             = string("Stop");
    onReset          = string("onReset");
    aisleLocked      = string("aisleLocked");
    aisleLock        = string("aisleLock");
    //dataOut          = string("dataOut"); 
    preparationTime  = 10;
    timeout          = 20;
    //alt_bit          = 0;
    next_internal    = pdevs::atomic<TIME, MSG>::infinity;

    model_active     = false;
  }

  /**
   * @Internal
   */

  void internal() noexcept {      
    if (ifReset || ifExit || ifLocked){
     
        model_active = false;
        next_internal = pdevs::atomic<TIME, MSG>::infinity;
      
    }  
  }
  /**
   * @advance
   */

  TIME advance() const noexcept {  
    return next_internal;
  }

  /**
   *@output
   */

  vector<MSG> out() const noexcept {    
    MSG aux;
    vector<MSG> out_put;
    if (ifReset){
      aux.port = onReset;
      aux.value = true;
      out_put.push_back(aux);
      //aux.port = packetSentOut;
      //aux.value = packetNum;
      //out_put.push_back(aux);
    }
     else if (ifExit){
        aux.port = stop;
        aux.value = true;
        out_put.push_back(aux);
      }
      else if (ifLocked){
        aux.port = aisleLock;
        aux.value = true;
        out_put.push_back(aux);
      }
       
    return out_put;
  }

  /**
   *@external
   */

  void external(const std::vector<MSG>& mb, const TIME& t) noexcept {    
    
    if(mb.size() > 1){
      assert(false && "one message per time uniti");
    }
    for(int i = 0; i < mb.size(); i++){
      if( mb[i].port == aisleLocked  && model_active == false)
	{
          ifLocked = true;
        if (ifLocked){
          model_active = true;
          next_internal = preparationTime;
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
      }
       else if( mb[i].port == reset  && model_active == false){
        if(ifLocked)
	{
	  if(ifExit){
          ifReset = true;
	  if (ifReset){
          model_active = true;
          next_internal = TIME(0);
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
        }else {
	    next_internal = TIME(0);
	  }
	}
	else{
            ifReset = true;
	  if (ifReset){
          model_active = true;
          next_internal = TIME(0);
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
	}
	}
        else if( mb[i].port == onExit  && model_active == false) { 
         ifExit = true;
        if (ifExit){
          model_active = true;
          next_internal = preparationTime;
        }else{
          if(next_internal != pdevs::atomic<TIME, MSG>::infinity){
            next_internal = next_internal - t;
          }
        }
      }
    }  
  }

  /**
   * @confluence 
  */

  virtual void confluence(const std::vector<MSG>& mb, const TIME& t) noexcept {
    external (mb, t);

  }

};

#endif // BOOST_SIMULATION_PDEVS_AISLEUNLOCKED_HPP
