/**
* \file main-training.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#include <iostream>
#include <iomanip>
#include <gegelati.h>

#include "pendulum.h"
#include "renderer.h"
#include "instructions.h"

#include "pendulum_wrapper.h"
#include "main-training.h"

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

		// Basic logger for the training process
		Log::LABasicLogger basicLogger(la);

		// Train for params.nbGenerations generations
		for (int i = 0; i < params.nbGenerations; i++) {
			la.trainOneGeneration(i);
		}

		// Cleanup instruction set
		deleteInstructions(instructionSet);
	}
	catch (const std::exception& ex) {
		std::cerr << ex.what() << std::endl;
	}
}