/**
* \file pendulum.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#include <algorithm>

#include "pendulum.h"

void Pendulum::setAngle(double newValue)
{
	this->angle = newValue;
}

void Pendulum::setVelocity(double newValue)
{
	this->velocity = newValue;
}

double Pendulum::getAngle() const
{
	return this->angle;
}

double Pendulum::getVelocity() const
{
	return this->velocity;
}

void Pendulum::applyTorque(double torque)
{
	// Constrain the torque value to [-MAX_TORQUE, MAX_TORQUE]
	torque = std::min(MAX_TORQUE, torque);
	torque = std::max(-MAX_TORQUE, torque);

	// Update angular velocity
	this->setVelocity(this->getVelocity() + ((-3.0) * G / (2.0 * LENGTH) * (sin(angle + M_PI)) +
		(3.f / (MASS * LENGTH * LENGTH)) * torque) * TIME_DELTA);
	this->setVelocity(std::fmin(std::fmax(this->getVelocity(), -MAX_SPEED), MAX_SPEED));

	// Apply friction
	this->setVelocity(this->getVelocity() * (1 - FRICTION));

	// Update angle
	this->setAngle(this->getAngle() + this->getVelocity() * TIME_DELTA);
}

Pendulum::Pendulum(double a, double v, double maxSpeed,
	double maxTorque, double timeDelta, double gravity,
	double mass, double length, double friction) :
	angle{ a }, velocity{ v }, MAX_SPEED{ maxSpeed }, MAX_TORQUE{ maxTorque },
	TIME_DELTA{ timeDelta }, G{ gravity }, MASS{ mass }, LENGTH{ length },
	FRICTION{ friction }
{
}
