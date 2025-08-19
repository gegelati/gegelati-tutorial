/**
* \file main-inference.cpp
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

	std::cout << "Pendulum TPG inference." << std::endl;

	// Use the same instruction set as the one used for training the TPG
	Instructions::Set instructionSet;
	fillInstructionSet(instructionSet);

	// Use the same parameters as the one used for training the TPG
	Learn::LearningParameters params;
	File::ParametersParser::loadParametersFromJson(ROOT_DIR "/params.json", params);

	// Setup the Learning Environment (LE)
	PendulumWrapper pendulumLE;

	// Instantiate the Learning Agent (LA)
	// The learning agent is not strictly needed for inference purposes, 
	// nevertheless, as it provides a simple API to initialize the execution
	// environment, it is easier to use it in this example.
	// Also note that this code can be reused as a basis to restart the 
	// training of a previously saved TPG.
	Learn::LearningAgent la(pendulumLE, instructionSet, params);
	la.init();

	// Load the TPG from the file
	File::TPGGraphDotImporter importer(ROOT_DIR "/dat/best_tpg.dot", la.getEnvironment(), *la.getTPGGraph());
	importer.importGraph();

	// Prepare for inference
	// Create an execution engine for executing the TPG.
	TPG::TPGExecutionEngine tee(la.getEnvironment());
	// Get the root of the TPG (there should be a single one)
	const TPG::TPGVertex* root(la.getTPGGraph()->getRootVertices().back());

	// Init display
	Renderer::renderInit();

	// Simulation and display loop
	// Runs until the used exits the display
	bool exit = false;
	while (!exit) {
		// Reset pendulum state
		pendulumLE.reset(0, Learn::LearningMode::TESTING);

		for (uint64_t frame = 0; frame < params.maxNbActionsPerEval && !exit; frame++) {

			/// Do one inference with the TPG
			auto trace = tee.executeFromRoot(*root);
			uint64_t action = ((const TPG::TPGAction*)trace.back())->getActionID();

			// Apply the action on the learning environment.
			pendulumLE.doAction(action);

			// Display the result.
			auto uiAction = Renderer::renderEnv(pendulumLE.pendulum.getAngle(), pendulumLE.actions[action] * pendulumLE.pendulum.MAX_TORQUE, frame, 0, pendulumLE.pendulum.TIME_DELTA);
			exit = uiAction == INT_MIN;
		}
		// Display the score obtained
		std::cout << "Score: " << pendulumLE.getScore() << std::endl;
	}

	// Cleanup instruction set
	deleteInstructions(instructionSet);

	// Exit display
	Renderer::renderFinalize();
}