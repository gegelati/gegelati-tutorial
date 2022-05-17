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

int main(int argc, char** argv) {
	try { // Global exception catching.

		std::cout << "Pendulum TPG training." << std::endl;

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

		// Instantiate and initialize the Learning Agent (LA)
		Learn::LearningAgent la(pendulumLE, instructionSet, params);
		la.init();

		// Start display thread
		std::atomic<bool> exitProgram = true; // (set to false by other thread after init) 
		std::atomic<bool> doDisplay = false;
		std::atomic<uint64_t> generation = 0;
		std::deque< std::tuple<uint64_t, double, double>> replay;
		std::thread threadDisplay(Renderer::replayThread, std::ref(exitProgram), std::ref(doDisplay), std::ref(generation), pendulumLE.pendulum.TIME_DELTA, std::ref(replay));
		while (exitProgram); // Wait for other thread to print key info.

		// Basic logger for the training process
		Log::LABasicLogger basicLogger(la);

		// Train for params.nbGenerations generations
		for (int i = 0; i < params.nbGenerations && !exitProgram; i++) {
			la.trainOneGeneration(i);

			// Get replay of best root actions on the pendulum
			replay = createReplay(pendulumLE, la.getBestRoot().first, instructionSet, params);
			generation = i;

			// trigger display
			doDisplay = true;
			while (doDisplay && !exitProgram);
		}

		// Cleanup instruction set
		deleteInstructions(instructionSet);

		// Exit the display thread
		std::cout << "Exiting program, press a key then [enter] to exit if nothing happens.";
		threadDisplay.join();
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
}