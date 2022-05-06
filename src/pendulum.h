/**
* \file pendulum.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef PENDULUM_H
#define PENDULUM_H

#define _USE_MATH_DEFINES // To get M_PI
#include <math.h>
#include <cmath>

/**
* \brief Inverted pendulum class
*
* The code of the class is adapted from Florian Arrestier's code released
* under CECILL-C License.
* Link: https://github.com/preesm/preesm-apps/tree/master/org.ietr.preesm.reinforcement_learning
*/
class Pendulum {
private:
	/// Current angle of the pendulum in radians.
	double angle;

	/// Current velocity of the pendulum in [-MAX_SPEED; MAX_SPEED]
	double velocity;

public:
	// Constants for the pendulum behavior

	/// Maximum angular velocity of the pendulum.
	const double MAX_SPEED;

	/// Maximum torque applicable to the pendulum.
	const double MAX_TORQUE;

	/// Time step used for simulating the pendulum behavior.
	const double TIME_DELTA;

	/// Gravity constant.
	const double G;

	/// Theoretical mass of the pendulum.
	const double MASS;

	/// Theoretical length of the pendulum.
	const double LENGTH;

	/// Theoretical length of the pendulum.
	const double FRICTION;

	/// Getter for angle state
	double getAngle() const;

	/// Getter for velocity
	double getVelocity() const;

	/// Setter for angle state
	void setAngle(double newValue);

	/// Setter for velocity
	void setVelocity(double newValue);

	/**
	* \brief Apply the given torque to the pendulum and advance the simulation.
	*
	* \param[in] torque the torque applied to the pendulum. If the given value
	* is not within the [-MAX_TORQUE, MAX_TORQUE] interval, it will be
	* saturated to the closest bound.
	*/
	void applyTorque(double torque);

	/**
	* \brief Default constructor for a pendulum.
	*
	* \param[in] a the initial angular position of the pendulum. Default value
	* is 0.0, that is downard position.
	* \param[in] c the initial angular velocity of the pendulum. Default value
	* is 0.0.
	* \param[in] maxSpeed Maximum speed of the pendulum
	* \param[in] maxTorque maximum torque applicable to the pendulum.
	* \param[in] timeDelta Simulation step duration.
	* \param[in] gravity Universal constant
	* \param[in] mass Weight of the pendulum.
	* \param[in] length Length of the pendulum.
	* \param[in] friction Friction slowing down the pendulum proportionally to 
	* its velovity.
	*/
	Pendulum(double a = M_PI, double v = 0.0, double maxSpeed = 16.0,
		double maxTorque = 2.0, double timeDelta = 0.01, double gravity = 9.81,
		double mass = 0.8, double length = 1.0, double friction = 0.005);

};

#endif