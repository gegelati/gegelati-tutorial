/**
* \file renderer.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <deque>

#define DISPLAY_W 500
#define DISPLAY_H 500

#define PATH_PENDULUM PROJECT_ROOT_PATH "/dat/pendulum.png"
#define PATH_ARROW PROJECT_ROOT_PATH "/dat/arrow.png"
#define PATH_TTF PROJECT_ROOT_PATH "/dat/DejaVuSans.ttf"

/*
* The code of the renderer is adapted from Florian Arrestier's code released
* under CECILL - C License.
* Link : https://github.com/preesm/preesm-apps/tree/master/org.ietr.preesm.reinforcement_learning
*/

/**
* Namespace for rendering pendulum with the SDL2 lib.
*/
namespace Renderer {

	/**
	* \brief Initializes SDL renderer and TTF font
	*/
	void renderInit();

	/**
	 * \brief Function for rendering text in the displayed window.
	 *
	 * \param[in] text The string of character to display.
	 * \param[in] posX Abscissa of the text top-left corner.
	 * \param[in] posY Ordinate of the text top-left corner.
	 */
	void displayText(const char* text, int posX, int posY);

	/**
	 * \brief Display the environment.
	 *
	 * \param[in] state Current angle of the pendulum to display.
	 * \param[in] torque the torque currently applied to the pendulum.
	 * \param[in] frame the frame number to display in the top left corner.
	 * \param[in] generation the generation number to display in the top left corner.
	 * \return An int value is returned to the controller loop depending
	 * on the action made by the user:
	 * - [-3, 3]: 6 actions available to apply a torque to the pendulum.
	 * - INT_MIN: Exit request.
	 * \param[in] timeDelta time in second between two frames.
	 */
	int renderEnv(double state, double torque, uint64_t frame, uint64_t generation, double timeDelta);

	/**
	* \brief Separate control loop for displaying replays in parallel to training.
	*
	* Function initialazing and controlling a display rendering replays of the
	* pendulum oscillations.
	* This function should be launched in its own thread.
	*
	* \param[in,out] exit Boolean value for controlling the end of the display
	* and the main program.
	* \param[in,out] doDisplay Boolean value used as a semaphore to control the
	* creation of a new replay. This boolean value is set to true to start the
	* creation of a new replay, and this function resets it to false to
	* indicate that the replay has been created.
	* \param[in] generation Integer value representing the current generation
	* of the training process.
	* \param[in] replay deque used to transmit the replay to the secondary thread.
	*/
	void replayThread(std::atomic<bool>& exit, std::atomic<bool>& doDisplay, std::atomic<uint64_t>& generation, double delta, std::deque<std::tuple<uint64_t, double, double>>& replay);

	/**
	 * \brief Close SDL and destroy opened textures.
	*/
	void renderFinalize();
};

#endif // !RENDERER_H
