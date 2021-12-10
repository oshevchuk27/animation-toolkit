#include "steerable.h"
#include "behaviors.h"

using namespace glm;
using namespace atk;

float ASteerable::kVelKv = 5.0;
float ASteerable::kOriKv = 16.0;
float ASteerable::kOriKp = 64.0;

const double PI = 3.1415926535897932384626433832795;

// Given a desired velocity, veld, and dt, compute a transform holding 
// the new orientation and change in position
//
// rotation control: tau = I * (-Kv1 * thetaDot + Kp1 * (thetaD - theta))
// translation control: f = m * Kv0 * (vd - v)
void ASteerable::senseControlAct(const vec3& veld, float dt)
{
	// Compute _vd and _thetad

	_vd = length(veld);



	std::cout << _vd << std::endl;

	_thetad = atan2(veld.x, veld.z);



	// compute _force and _torque
	_force = _mass * kVelKv * (_vd - _state[VEL]);

	std::cout << "force" << _force << std::endl;

	float angle = _thetad - _state[ORI];
	angle = fmod(angle + PI, PI * 2) - PI;

	_torque = _inertia * (kOriKp * (angle)-kOriKv * _state[AVEL]);

	std::cout << "torque" << _torque << std::endl;


	// find derivative

	_derivative[POS] = _state[VEL];
	_derivative[ORI] = _state[AVEL];
	_derivative[VEL] = _force / _mass;
	_derivative[AVEL] = _torque / _inertia;


	// update state

	_state[POS] = _state[POS] + dt * _derivative[POS];
	_state[ORI] = _state[ORI] + dt * _derivative[ORI];
	_state[VEL] = _state[VEL] + dt * _derivative[VEL];
	_state[AVEL] = _state[AVEL] + dt * _derivative[AVEL];



	// compute global position and orientation and update _characterRoot
	quat rot = glm::angleAxis(_state[ORI], vec3(0, 1, 0));
	vec3 localPos(0, 0, _state[POS]);

	_characterRoot.setT(rot * localPos + _characterRoot.t());
	_characterRoot.setR(rot);


}

float randColorValue() {
	float randColorValue = 0 + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (1 + 1)));
	return randColorValue;
}



float randSizeValue() {
	float randSizeValue = 1 + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (2 - 1)));
	return randSizeValue;
}


float randRadiusValue() {
	float randRadValue = 10 + static_cast <float> (rand()) /
		(static_cast <float> (RAND_MAX / (20 - 10)));
	return randRadValue;
}



// randomize the colors, characters, and animation times here
void ASteerable::randomizeAppearance()
{
	// to randomize the walking animations, compute different initial values 
	// for _time

	// to randomize color, call _drawer.setColor

	_drawer.color = vec3(randColorValue(), randColorValue(), randColorValue());

	// to randomize shape, compute random values for _drawer.setJointRadius
	
	_drawer.size = vec3(randSizeValue());

	_drawer.jointRadius = randRadiusValue();

	// or randomly assign different drawers to have a mix of characters


}

