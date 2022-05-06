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
	std::cout << "Pendulum TPG training." << std::endl;

	// Create the instruction set for programs
	Instructions::Set set;
	fillInstructionSet(set);

	// Set the parameters for the learning process.
	// Controls mutations probability, program lengths, and graph size among 
	// other things. Loads them from the file params.json
	Learn::LearningParameters params;
	File::ParametersParser::loadParametersFromJson(ROOT_DIR "/params.json", params);

	// Setup the learning environment
	PendulumWrapper pw;
}