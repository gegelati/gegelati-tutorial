/**
* \file replay.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef REPLAY_H
#define REPLAY_H

#include <deque>
#include <gegelati.h>

#include "training/pendulum_wrapper.h"

/**
* \brief Create a replay of actions, angles and velocity from a TPG root.
*
* \param[in] root the TPGVertex from which the replay is created.
* \param[in] set the Instruction set of the TPG.
* \param[in] params the learning parameters used in the TPG.
*/
std::deque<std::tuple<uint64_t, double, double>> createReplay(PendulumWrapper& pendulumLE, const TPG::TPGVertex* root, const Instructions::Set& set, const Learn::LearningParameters& params);

#endif