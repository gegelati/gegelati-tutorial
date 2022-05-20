/**
* \file main-manual.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#include <climits>
#include <iostream>
#include <iomanip>

#include "pendulum.h"
#include "renderer.h"

int main(int argc, char** argv) {
	std::cout << "Pendulum manual control." << std::endl;
	std::cout << "Press the following keys to control the pendulum:" << std::endl <<
		"\t [C], [V], [B]: Apply a clockwise torque to the pendulum." << std::endl <<
		"\t [1], [2], [3]: Apply a counter clockwise torque to the pendulum." << std::endl <<
		"\t [Q]: Exit the simulator." << std::endl;
	std::cout << std::endl << "Press [Enter] to start the simulation.";
	getc(stdin);

	// Instanciate the pendulum
	Pendulum p;

	// Initialize the display
	Renderer::renderInit();

	// Available actions
	double torques[] = { -p.MAX_TORQUE, -0.66 * p.MAX_TORQUE,  -0.33 * p.MAX_TORQUE,
	0.0 , 0.33 * p.MAX_TORQUE, 0.66 * p.MAX_TORQUE,  p.MAX_TORQUE };

	// Simulation and display loop.
	int frame = 0;
	bool exit = false;
	bool forward = false;
	double torque = 0.0;
	while(!exit) {
		frame++;		

		int action = Renderer::renderEnv(p.getAngle(), torque, frame, 0, p.TIME_DELTA);

		if (exit = (action == INT_MIN)) {
			std::cout << "Exit simulation loop." << std::endl;
			continue;
		}

		torque = torques[action + 3];

		p.applyTorque(torque);
	}

	// Finalize the display
	Renderer::renderFinalize();

	std::cout << "GEGELATI Tutorial end" << std::endl;
	return 0;
}
