/**
* \file pendulum.h
* \author Karol Desnos
* \license CeCILL-C
*/

#ifndef PENDULUM_H
#define PENDULUM_H

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

	/// Setter for angle state
	void setAngle(double newValue);

	/// Setter for velocity
	void setVelocity(double newValue);

public:
	// Constants for the pendulum behavior

	/// Maximum angular velocity of the pendulum.
	static const double MAX_SPEED;

	/// Maximum torque applicable to the pendulum.
	static const double MAX_TORQUE;

	/// Time step used for simulating the pendulum behavior.
	static const double TIME_DELTA;

	/// Gravity constant.
	static const double G;

	/// Theoretical mass of the pendulum.
	static const double MASS;

	/// Theoretical length of the pendulum.
	static const double LENGTH;

	/// Getter for angle state
	double getAngle() const;

	/// Getter for velocity
	double getVelocity() const;

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
	*/
	Pendulum(double a = 0.0, double v = 0.0);

};

#endif