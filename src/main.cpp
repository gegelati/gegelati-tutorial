/**
* \file main.c
* \author Karol Desnos
* \license CeCILL-C
*/

#include <iostream>
#include <iomanip>
#include "pendulum.h"

int main(int argc, char** argv) {
	std::cout << "GEGELATI Tutorial start" << std::endl;

	Pendulum p;
	bool forward = false;

	for (auto frame = 0; frame < 300; frame++) {
		if (frame % 300 == 0) {
			forward = !forward;
		}

		double torque = (forward) ? Pendulum::MAX_TORQUE : -Pendulum::MAX_TORQUE;
		p.applyTorque(torque);

		std::cout << std::fixed;
		std::cout.precision(4);
		std::cout << "Frame: " << std::setw(4) << frame
			<< " Tor:" << std::setw(8) << torque
			<< " Ang: " << std::setw(8) << p.getAngle()
			<< " Vel: " << std::setw(8) << p.getVelocity() << std::endl;
	}

	std::cout << "GEGELATI Tutorial end" << std::endl;
	return 0;
}