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
#include "../../vendor/britime.hpp"
#include "../../data_structures/message.hpp"
#include "../../vendor/input_event_stream.hpp"
#include "../../atomics/timer.hpp" //ADD FILENAME

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


  auto Timer1 = make_atomic_ptr<timer<Time,Message>>();

  shared_ptr<flattened_coupled<Time, Message>> TOP(new flattened_coupled<Time, Message>(
  {input_test_generator, Timer1}, 
  {}, 
  {{input_test_generator, Timer1}}, 
  {Timer1}
  ));  
  //CHANGE ACCORDING TO FILE
 
    cout << "Preparing runner" << endl;
    Time initial_time = Time(0); 
    ofstream out_data("timer_test_output.txt"); //CHANGE  
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



    

   
