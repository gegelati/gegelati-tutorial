#include "pendulum_wrapper.h"

const std::vector<double> PendulumWrapper::actions{ 0.0 };

PendulumWrapper::PendulumWrapper() : LearningEnvironment(actions.size() /* Number of actions of the learning environment*/)
{
}

std::vector<std::reference_wrapper<const Data::DataHandler>> PendulumWrapper::getDataSources(void)
{
	return std::vector<std::reference_wrapper<const Data::DataHandler>>();
}

void PendulumWrapper::reset(size_t, Learn::LearningMode)
{
}

void PendulumWrapper::doAction(uint64_t actionID)
{
}

double PendulumWrapper::getScore(void) const
{
	return 0.0;
}

bool PendulumWrapper::isTerminal(void) const
{
	return false;
}
