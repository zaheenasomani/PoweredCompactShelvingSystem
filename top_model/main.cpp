
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
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/simulation.hpp>
#include "../vendor/britime.hpp"
#include "../data_structures/message.hpp"
#include "../vendor/input_event_stream.hpp"
#include "../atomics/entrySensor.hpp"
#include "../atomics/exitSensor.hpp"
#include "../atomics/counter.hpp"
#include "../atomics/aisleUnlocked.hpp"
#include "../atomics/calcAisles.hpp"
#include "../atomics/timer.hpp"
#include "../atomics/filterPort.hpp"
#include "../atomics/portConversor.hpp"

using namespace std;
using namespace boost::simulation;
using namespace boost::simulation::pdevs;
using namespace boost::simulation::pdevs::basic_models;


using hclock = chrono::high_resolution_clock;
using Time =BRITime;
using Message = Message_t;


int main(int argc, char ** argv) {

  if (argc < 2) {
   cout << "you are using this program with wrong parameters. Te program should be invoked as follow:";
   cout << argv[0] << " path to the input file " << endl;
   return 1; 
  }
  
  string test_file = argv[1];
  ifstream file(test_file);
  string str;
  string file_contents;
  while (getline(file, str)){
    file_contents += str;
    file_contents.push_back('\n');
  }  
    
  string m_input;
  m_input = file_contents;

  cout << "model input:" << endl;
  cout << m_input << endl;

  shared_ptr<istringstream> pointer_iss{ new istringstream{} };
  pointer_iss->str(m_input);

  auto input_test_generator = make_atomic_ptr<input_event_stream<Time, Message, Time, Message>, shared_ptr<istringstream>, Time>(pointer_iss, Time(0),
                [](const string& s, Time& t_next, Message& m_next)->void{ //parsing function
            
        string aux;
        m_next.clear();          
        istringstream ss;        
        ss.str(s);
        ss >> t_next;
        ss >> m_next.port;
        ss >> m_next.value;
        
        string thrash;
        ss >> thrash;
        if ( 0 != thrash.size()) throw exception();         
  });

  auto entry1 = make_atomic_ptr<entrySensor<Time,Message>>();
  auto exit1 = make_atomic_ptr<exitSensor<Time,Message>>();
  auto counter1 = make_atomic_ptr<counter<Time,Message>>();
  auto aisleUnlocked1 = make_atomic_ptr<aisleUnlocked<Time,Message>>();
  auto calcAisles1 = make_atomic_ptr<calcAisles<Time,Message>>();
  auto timer1 = make_atomic_ptr<timer<Time,Message>>();
  auto FilterIncrement = make_atomic_ptr<filter<Time,Message>, string>(string("incr"));
  auto FilterDecrement = make_atomic_ptr<filter<Time,Message>, string>(string("decr"));
  auto FilterAisleLocked = make_atomic_ptr<filter<Time,Message>, string>(string("aisleLocked"));
  auto FilterStop = make_atomic_ptr<filter<Time,Message>, string>(string("stop"));
  auto FilterOnReset = make_atomic_ptr<filter<Time,Message>, string>(string("onReset"));
  auto FilterOnExit = make_atomic_ptr<filter<Time,Message>, string>(string("onExit"));
  auto PortConversorMoveLeft = make_atomic_ptr<portConversor<Time,Message>, string>(string("moveLeft"));
  auto PortConversorMoveRight = make_atomic_ptr<portConversor<Time,Message>, string>(string("moveRight"));
  auto PortConversorReset = make_atomic_ptr<portConversor<Time,Message>, string>(string("reset"));
  

  shared_ptr<flattened_coupled<Time, Message>> TOP(new flattened_coupled<Time, Message>(
  {input_test_generator, entry1, exit1, counter1, aisleUnlocked1, calcAisles1, timer1,  FilterIncrement, FilterDecrement, FilterAisleLocked, FilterStop, FilterOnReset, PortConversorMoveLeft, PortConversorMoveRight, FilterOnExit, PortConversorReset },
  {}, 
  {{input_test_generator, counter1}, {input_test_generator, aisleUnlocked1}, {input_test_generator, entry1}, {input_test_generator, exit1}, {entry1, FilterIncrement}, {exit1, FilterDecrement}, {FilterIncrement, counter1}, {FilterDecrement, counter1}, {counter1, FilterOnExit}, {FilterOnExit, aisleUnlocked1}, {aisleUnlocked1, FilterStop}, {aisleUnlocked1, FilterOnReset}, {FilterOnReset, calcAisles1}, {calcAisles1, timer1}, {counter1, FilterAisleLocked}}, 
  {entry1, exit1, counter1, aisleUnlocked1, calcAisles1, timer1,  FilterIncrement, FilterDecrement, FilterAisleLocked, FilterStop, FilterOnReset, PortConversorMoveLeft, PortConversorMoveRight, FilterOnExit, PortConversorReset}
  ));  
  
 
  cout << "Preparing runner" << endl;
  Time initial_time = Time(0); 
  ofstream out_data("pcss_output.txt");   
  runner<Time, Message> r(TOP, initial_time, out_data, [](ostream& os, Message m){ os << m;});
  Time end_time = Time(1000000);

  cout << "Starting simulation until time: " << end_time << "seconds" << endl;

  auto start = hclock::now(); 

  end_time = r.runUntil(end_time); 
  auto elapsed = chrono::duration_cast<std::chrono::duration<double, std::ratio<1>>> (hclock::now() - start).count(); 
    
  cout << "Finished simulation with time: " << end_time << "sec" << endl;
  cout << "Simulation took: " << elapsed << "sec" << endl;
  return 0;
}



    

   
