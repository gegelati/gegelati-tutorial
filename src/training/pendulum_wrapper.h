/**
* \file pendulum_wrapper.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef PENDULUM_WRAPPER_H
#define PENDULUM_WRAPPER_H

#define SOLUTION

#include <gegelati.h>

#ifdef SOLUTION
#include "pendulum.h"
#endif // SOLUTION

/**
* \brief Learning environment wrapper for interfacing the pendulum with GEGELATI.
*
* This class implements all the methods needed to train a TPG within a
* learning environment.
*/
class PendulumWrapper : public Learn::LearningEnvironment {
public:

	/// Array containing the actions available for the TPG.
	/// These actions are expressed as real numbers in [-1, 1], and will be multiplied
	/// by the MAX_TORQUE of the Pendulum to apply the corresponding action.
	static const std::vector<double> actions;

#ifdef SOLUTION
	/// Pendulum interfaced with the GEGELATI Lib  
	Pendulum pendulum;

	/// DataHandler wrappers
	std::vector<Data::PointerWrapper<double>> data;
#endif // SOLUTION

#ifdef SOLUTION
	double accumulatedReward;
#endif // SOLUTION

	/// Default constructor for the PendulumWrapper
	PendulumWrapper();

	/**
	 * \brief Get the data sources for this LearningEnvironment.
	 *
	 * This method returns a vector of reference to the DataHandler that
	 * will be given to the learningAgent, and to its Program to learn how
	 * to interact with the LearningEnvironment. Throughout the existence
	 * of the LearningEnvironment, data contained in the data will be
	 * modified, but never the number, nature or size of the dataHandlers.
	 * Since this methods return references to the DataHandler, the
	 * learningAgent will assume that the referenced dataHandler are
	 * automatically updated each time the doAction, or reset methods
	 * are called on the LearningEnvironment.
	 *
	 * \return a vector of references to the DataHandler.
	 */
	virtual std::vector<std::reference_wrapper<const Data::DataHandler>> getDataSources(void) override;

	/**
	 * \brief Reset the LearningEnvironment.
	 *
	 * Resetting a learning environment is needed to train an agent.
	 * Optionally seed can be given to this function to control the
	 * randomness of a LearningEnvironment (if any). When available, this
	 * feature will be used:
	 * - for comparing the performance of several agents with the same
	 * random starting conditions.
	 * - for training each agent with diverse starting conditions.
	 *
	 * \param[in] seed the integer value for controlling the randomness of
	 * the LearningEnvironment.
	 * \param[in] mode LearningMode in which the Environment should be
	 * reset for the next set of actions.
	 */
	virtual void reset(size_t, Learn::LearningMode) override;

	/**
	 * \brief Execute an action on the LearningEnvironment.
	 *
	 * The purpose of this method is to execute an action, represented by
	 * an actionId comprised between 0 and nbActions - 1.
	 * The LearningEnvironment implementation only checks that the given
	 * actionID is comprised between 0 and nbActions - 1.
	 * It is the responsibility of this method to call the updateHash
	 * method on dataSources whose content have been affected by the action.
	 *
	 * \param[in] actionID the integer number representing the action to
	 * execute.
	 * \throw std::runtime_error if the actionID exceeds nbActions - 1.
	 */
	virtual void doAction(uint64_t actionID) override;

	/**
	 * \brief Returns the current score of the Environment.
	 *
	 * The returned score will be used as a reward during the learning
	 * phase of a LearningAgent.
	 *
	 * \return the current score for the LearningEnvironment.
	 */
	virtual double getScore(void) const override;

	/**
	 * \brief Method for checking if the LearningEnvironment has reached a
	 * terminal state.
	 *
	 * The boolean value returned by this method, when equal to true,
	 * indicates that the LearningEnvironment has reached a terminal state.
	 * A terminal state is a state in which further calls to the doAction
	 * method will have no effects on the dataSources of the
	 * LearningEnvironment, or on its score. For example, this terminal
	 * state may be reached for a Game Over state within a game, or in case
	 * the objective of the learning agent has been successfuly reached.
	 *
	 * \return a boolean indicating termination.
	 */
	virtual bool isTerminal(void) const override;
};

#endif // !PENDULUM_WRAPPER_H
