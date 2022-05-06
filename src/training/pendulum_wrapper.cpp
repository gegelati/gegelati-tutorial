#include "pendulum_wrapper.h"

#ifdef SOLUTION
const std::vector<double> PendulumWrapper::actions{ -1.0, -0.66, -0.33, 0.0, 0.33, 0.66, 1.0 };
#else
const std::vector<double> PendulumWrapper::actions{ 0.0 };
#endif // !SOLUTION

#ifdef SOLUTION
PendulumWrapper::PendulumWrapper() : LearningEnvironment(actions.size()), pendulum(), data(2)
{
}
#else
PendulumWrapper::PendulumWrapper() : LearningEnvironment(actions.size())
{
}
#endif // SOLUTION



std::vector<std::reference_wrapper<const Data::DataHandler>> PendulumWrapper::getDataSources()
{
#ifdef SOLUTION
	std::vector<std::reference_wrapper<const Data::DataHandler>> result;
	result.push_back(this->data);
	return result;
#else
	return std::vector<std::reference_wrapper<const Data::DataHandler>>();
#endif
}

void PendulumWrapper::reset(size_t seed, Learn::LearningMode mode)
{
#ifdef SOLUTION
	this->pendulum.setAngle(M_PI);
	this->pendulum.setVelocity(0.0);

	// Copy the resulting angle and velocity in the data handler
	this->data.setDataAt(typeid(double), 0, this->pendulum.getAngle());
	this->data.setDataAt(typeid(double), 1, this->pendulum.getVelocity());
#endif // SOLUTION
#ifdef SOLUTION
	this->accumulatedReward = 0.0;
#endif // SOLUTION

}

void PendulumWrapper::doAction(uint64_t actionID)
{
#ifdef SOLUTION
	// Retrieve the torque corresponding to the ID
	double torque = this->actions[actionID] * pendulum.MAX_TORQUE;

	// Apply it to the pendulum
	this->pendulum.applyTorque(torque);

	// Copy the resulting angle and velocity in the data handler
	this->data.setDataAt(typeid(double), 0, this->pendulum.getAngle());
	this->data.setDataAt(typeid(double), 1, this->pendulum.getVelocity());
#endif
#ifdef SOLUTION
	// Get the angle value between -M_PI and M_PI (0 being the upward position)
	double angle = fmod(this->pendulum.getAngle() + M_PI, 2.0 * M_PI) - M_PI;

	// Compute a reward based on the angle to the upward position, the velocity and the torque.
	// All three values should be minimized.
	double reward = -((angle * angle) + 0.1f * (this->pendulum.getVelocity() * this->pendulum.getVelocity()) + 0.001f * (torque * torque));

	// Accumulate the reward
	accumulatedReward += reward;
#endif // SOLUTION
}

double PendulumWrapper::getScore(void) const
{
#ifdef SOLUTION
	// Return the accumulated reward, divided by 10 for better display.
	return accumulatedReward / 10;
#else
	return 0.0;
#endif // SOLUTION

	
}

bool PendulumWrapper::isTerminal(void) const
{
	return false;
}
