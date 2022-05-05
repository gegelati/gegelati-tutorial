/**
* \file pendulum.cpp
* \author Karol Desnos
* \license CeCILL-C
*/

#include <algorithm>

#include "pendulum.h"

const double Pendulum::MAX_SPEED = 8.0;
const double Pendulum::MAX_TORQUE = 2.0;
const double Pendulum::TIME_DELTA = 0.05;
const double Pendulum::G = 9.81;
const double Pendulum::MASS = 1.0;
const double Pendulum::LENGTH = 1.0;

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

	// Update angle
	this->setAngle(this->getAngle() + this->getVelocity() * TIME_DELTA);
}

Pendulum::Pendulum(double a, double v): angle{a}, velocity{v}
{
}
