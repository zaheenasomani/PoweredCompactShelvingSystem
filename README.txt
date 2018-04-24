This folder contains the templates to implement DEVS models in CDBoost

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt

This file contains the information about the folders and the steps to run them.

atomics [This folder implements the atomic models in CDBoost]
	entrySensor.hpp
	exitSensor.hpp
	counter.hpp
	aisleUnlocked.hpp
	calcAisles.hpp
	timer.hpp
	filterPort.hpp
	portConversor.hpp

data_structures [This folder defines the message structure of the atomic model in CDBoost]
	message.hpp
	message.cpp

test [This folder contains the unit test]
	entrySensorCDBoost [This folder defines and compiles entrySensor test]
		main.cpp
		makefile
		entrySensor_input_test.txt

	exitSensorCDBoost [This folder defines and compiles exitSensor test]
		main.cpp
		makefile
		exitSensor_input_test.txt

	counterCDBoost [This folder defines and compiles counter test]
		main.cpp
		makefile
		counter_input_test.txt

	aisleUnlockedCDBoost [This folder defines and compiles aisleUnlocked test]
		main.cpp
		makefile
		aisleUnlocked_input_test.txt


	calcAislesCDBoost [This folder defines and compiles calcAisles test]
		main.cpp
		makefile
		calcAisles_input_test.txt

	timerCDBoost [This folder defines and compiles timer test]
		main.cpp
		makefile
		timer_input_test.txt

	filterPortCDBoost [This folder defines and compiles filterPort test]
		main.cpp
		makefile
		filterPort_input_test.txt

	PortConversorCDBoost [This folder defines and compiles portConversor test]
		main.cpp
		makefile
		portConversor_input_test.txt

vendor [This folder contains a time class to use in your model and a model to generate the inputs to the DEVS models]
	input_event_stream.hpp
	britime.hpp

top_model [This folder defines and compiles the PCSS top model]	
	main.cpp
	makefile
	PCSS_input_test.txt

/*************/
/****STEPS****/
/*************/

1 - Update include paths in all the makefiles in this folder and subfolders. You need to update the following lines:
	#INCLUDEBOOST=-I ../../../../boost_1_61_0
	INCLUDEBCDPP=-I ../../../../cdboost/include
    Update the relative path to cdboost/include from the folder where the makefile is. You need to take into account where you copied the folder during the installation process
	Example: INCLUDEBCDPP=-I ../../../../cdboost/include
    If you installed Boost using option 2, uncomment INCLUDEBOOST and update the relative path to Boost from the folder where the makefile is
	Example: INCLUDEBOOST=-I ../../../../boost_1_58_0
* ../ means going up a level in the directory tree.

2 - Run the unit tests
	2.1. Run filterPort test
		1 - Open the terminal. Press in your keyboard Ctrl+Alt+t
		2 - Set the command prompt in the filterPortCDBoost folder. To do so, type in the terminal the path to this folder.
			Example: cd Documents/PCSS/test/filterPortCDBoost
		3 - To compile the test, type in the terminal:
			make clean; make all
		4 - To run the test, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this specific test you need to type:
			./FILTERPORT filterPort_input_test.txt
		5 - To check the output of the test, open  "filterPort_test_output.txt"
	2.2. Run portConversor test
		1 - Open the terminal. Press in your keyboard Ctrl+Alt+t
		2 - Set the command prompt in the portConversorCDBoost folder. To do so, type in the terminal the path to this folder.
			Example: cd ../portConversor
		3 - To compile the test, type in the terminal:
			make clean; make all
		4 - To run the test, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this specific test you need to type:
			./PORTCONVERSOR portConversor_input_test.txt
		5 - To check the output of the test, open  "portConversor_test_output.txt"
	2.3. To run entrySensorCDBoost, exitSensorCDBoost, counterCDBoost, aisleUnlockedCDBoost, calcAislesCDBoost, timerCDBoost tests, the steps are analogous to 2.1 and 2.2
			
3 - Run the top model
	1 - Open the terminal. Press in your keyboard Ctrl+Alt+t
	2 - Set the command prompt in the top_model folder. To do so, type in the terminal the path to this folder.
		Example: cd ../../top_model
	3 - To compile the model, type in the terminal:
		make clean; make all
	4 - To run the model, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this test you need to type:
		./PCSS PCSS_input_test.txt
	5 - To check the output of the model, open  "pcss_output.txt"
	6 - To run the model with different inputs
		6.1. Create new .txt files with the same structure
		6.2. Run the model using the instructions in step 4
		6.3. If you want to keep the output, rename pcss_output.txt. To do so, type in the terminal: "mv pcss_output.txt NEW_NAME"
			Example: mv pcss_output.txt pcss_output_0.txt
