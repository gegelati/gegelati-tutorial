/**
* \file renderer.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef RENDERER_H
#define RENDERER_H

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
namespace Render {

	/**
	* \brief Initializes SDL renderer and TTF font
	*/
	void renderInit();

	/**
	 * \brief Close SDL and destroy opened textures.
	*/
	void renderFinalize();
}

#endif // !RENDERER_H
