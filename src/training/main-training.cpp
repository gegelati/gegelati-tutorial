/**
* \file main-training.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#include <iostream>
#include <iomanip>
#include <thread>
#include <gegelati.h>

#include "pendulum.h"
#include "renderer.h"
#include "instructions.h"
#include "replay.h"

#include "pendulum_wrapper.h"

#define DEACTIVATE_DISPLAY 0


/**
* Thread managing the training of the TPG.
*
* Code in this thread can generally be used as a template for the main function
* of a program training a TPG with GEGELATI. In this tutorial, it is separated
* to avoid mixing it with display functions, which are only portable when executed
* in the main thread.
*/
void train_main(std::atomic<bool>& exitProgram, std::atomic<bool>& doDisplay, std::atomic<uint64_t>& generation, double& time_delta, std::deque<std::tuple<uint64_t, double, double>>& replay) {

	// Create the instruction instructionSet for programs
	Instructions::Set instructionSet;
	fillInstructionSet(instructionSet);

	// Set the parameters for the learning process.
	// Controls mutations probability, program lengths, and graph size among 
	// other things. Loads them from the file params.json
	Learn::LearningParameters params;
	File::ParametersParser::loadParametersFromJson(ROOT_DIR "/params.json", params);

	// Setup the Learning Environment (LE)
	PendulumWrapper pendulumLE;

	#if ( DEACTIVATE_DISPLAY == 0 )
		// Forward the time_delta for display (DISPLAY SPECIFIC CODE)
		time_delta = pendulumLE.pendulum.TIME_DELTA;
	#endif

	// Instantiate and initialize the Learning Agent (LA)
	Learn::LearningAgent la(pendulumLE, instructionSet, params);
	la.init();

	// Basic logger for the training process
	Log::LABasicLogger basicLogger(la);


	// Train for params.nbGenerations generations
	for (uint64_t i = 0; i < params.nbGenerations && !exitProgram; i++) {
		la.trainOneGeneration(i);

		#if ( DEACTIVATE_DISPLAY == 0 )
			// Get replay of best root actions on the pendulum for the display
			replay = createReplay(pendulumLE, la.getBestRoot().first, instructionSet, params);
			generation = i;

			// trigger display
			doDisplay = true;
			while (doDisplay && !exitProgram);
		#endif
	}

	// Cleanup instruction set
	deleteInstructions(instructionSet);
}

int main(int argc, char** argv) {
	try { // Global exception catching.
		std::cout << "Pendulum TPG training." << std::endl;

		double time_delta;

		std::atomic<bool> exitProgram = false;
		std::atomic<bool> doDisplay = false;
		std::atomic<uint64_t> generation = 0;
		std::deque< std::tuple<uint64_t, double, double>> replay;
		#if ( DEACTIVATE_DISPLAY == 0 )
			// Start training in secondary thread
			std::thread threadTraining(train_main, std::ref(exitProgram), std::ref(doDisplay), std::ref(generation), std::ref(time_delta), std::ref(replay));
		#else 
			std::cout << "No display version, send interrupt signal to process to exit." << std::endl;
			// Start training in main thread
			train_main(exitProgram, doDisplay, generation, time_delta, replay);
		#endif

		// Replay code
		Renderer::replayThread(exitProgram, doDisplay, generation, time_delta, replay);

		#if ( DEACTIVATE_DISPLAY == 0 )
			// Exit the display thread
			threadTraining.join();
			std::cout << "Exiting program, press a key then [enter] to exit if nothing happens.";
		#endif
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
}
