#include "replay.h"

std::deque<std::tuple<uint64_t, double, double>> createReplay(PendulumWrapper& pendulumLE, const TPG::TPGVertex* root, const Instructions::Set& set, const Learn::LearningParameters& params)
{
	// Setup simulation environment
	std::deque<std::tuple<uint64_t, double, double>> replay;
	Environment env(set, pendulumLE.getDataSources(), params.nbRegisters, params.nbProgramConstant);
	TPG::TPGExecutionEngine tee(env);

	// Compute all moves
	pendulumLE.reset(0, Learn::LearningMode::VALIDATION);
	for (auto action = 0; action < params.maxNbActionsPerEval; action++) {
		auto vertexList = tee.executeFromRoot(*root);
		const auto actionID = ((const TPG::TPGAction*)vertexList.back())->getActionID();
		const double angle = pendulumLE.pendulum.getAngle();
		const double torque = pendulumLE.actions[actionID];
		replay.push_back(std::make_tuple(action, angle, torque));
		pendulumLE.doAction(actionID);
	}


	return replay;
}
