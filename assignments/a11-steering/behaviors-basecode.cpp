#include "behaviors.h"
#include "steerable.h"
#include <cmath>

using namespace glm;
using namespace atk;

//--------------------------------------------------------------
// Shared properties for all behaviors
ABehavior::ABehavior(const char* name) : _name(name)
{
	// TODO: set good values
	setParam("MaxSpeed", 200);
	setParam("AgentRadius", 200);
}

//--------------------------------------------------------------
// Seek behavior

ASeek::ASeek() : ABehavior("Seek")
{
}

//
// Steer the actor TOWARDS a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 ASeek::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	vec3 currentPos = actor.getPosition();
	vec3 desiredDir = target - currentPos;


	if (length(desiredDir) < 30) {
		return vec3(0);
	}

	vec3 normalizedDir = normalize(desiredDir);

	vec3 desiredVelocity = getParam("MaxSpeed") * normalizedDir;

	return desiredVelocity;
}

//--------------------------------------------------------------
// Flee behavior

AFlee::AFlee() : ABehavior("Flee")
{
}

//
// Steer the actor AWAY from a target at max speed
//
// @param actor: steerable character to move
// @return desired velocity
//
// @note: call actor.getPosition to get teh actor's position
// @note: call getParam("MaxSpeed") to get the max speed
vec3 AFlee::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& targetPos)

{

	vec3 currentPos = actor.getPosition();
	vec3 desiredDir = targetPos - currentPos;

	vec3 normalizedDir = normalize(desiredDir);

	vec3 desiredVelocity = -1.0f * getParam("MaxSpeed") * normalizedDir;

	return desiredVelocity;
}

//--------------------------------------------------------------
// Arrival behavior

AArrival::AArrival() : ABehavior("Arrival")
{
	// TODO: Set good parameters
	setParam("kArrival", 10);
	setParam("TargetRadius", 50);
}

//
// Returns a velocity whose speed is proportional to the actors distance
// from the target
//
// @param actor: steerable character to move
// @return desired velocity
vec3 AArrival::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& targetPos)

{

	vec3 p_d = targetPos;
	float r = getParam("TargetRadius");
	vec3 targetOffset = p_d - actor.getPosition();
	float distance = length(targetOffset);
	float speed;
	if (distance <= r) {
		speed = distance / r;
	}
	else {
		speed = getParam("MaxSpeed");
	}

	vec3 desiredVelocity = speed * normalize(targetOffset);


	return desiredVelocity;
}

//--------------------------------------------------------------
// Departure behavior

ADeparture::ADeparture() : ABehavior("Departure")
{
	setParam("InnerRadius", 200);
	setParam("OuterRadius", 500);
	setParam("kDeparture", 10);
}

//
// Calculate a repelent velocity based on the actor's 
// distance from the target
vec3 ADeparture::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& targetPos)
{
	vec3 p_d = targetPos;
	float r = getParam("InnerRadius");
	vec3 targetOffset = p_d - actor.getPosition();
	float distance = length(targetOffset);
	float speed;
	if (distance <= r) {
		speed = getParam("MaxSpeed") * distance / r;
	}
	else  if (distance <= getParam("outerRadius")) {
		speed = getParam("MaxSpeed");
	}
	else {
		speed = 0;
	}

	vec3 desiredVelocity = -1.0f * speed * normalize(targetOffset);


	return desiredVelocity;
}

//--------------------------------------------------------------
// Avoid behavior

AAvoid::AAvoid() : ABehavior("Avoid")
{
	setParam("kAvoid", 1);
}

// If an actor is near an obstacle, avoid adds either a tangential or
// normal response velocity
//  Obstacles are in getWorld()->getObstacle(i) and have class type AObstacle
vec3 AAvoid::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& targetPos)
{
	return vec3(0, 0, 0);
}
//--------------------------------------------------------------
// Wander behavior

AWander::AWander() : ABehavior("Wander")
{
	setParam("kWander", 1);
	setParam("wanderStrength", 10);
	setParam("wanderRate", 100);
}


float getRandomValue() {
	return  -1 + (float)(rand()) / ((float)(RAND_MAX / (1 + 1)));
}

// Wander returns a velocity whose direction changes randomly (and smoothly)
vec3 AWander::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{

	vec3 targetPoint = vec3(0, 0, 100);

	vec3 globalTargetPos = actor.getRotation() * targetPoint;

	vec3 currentPos = actor.getPosition();

	vec3 circleCenter = currentPos + globalTargetPos;

	vec3 jitterVelocity = vec3(getParam("wanderRate") * getRandomValue(), 0, getParam("wanderRate") * getRandomValue());


	vec3 q = jitterVelocity + getParam("wanderStrength") * vec3(agl::random(), 0, agl::random());


	vec3 desiredVelocity = normalize(q - actor.getPosition()) * getParam("MaxSpeed");


	return desiredVelocity;
}



//--------------------------------------------------------------
// Separation behavior

ASeparation::ASeparation() : ABehavior("Separation")
{
	setParam("Neighborhood", 1);
	setParam("kSeparation", 1);
}


// Separation tries to maintain a constant distance between all agents
// within the neighborhood of the agent
// getWorld()->getAgent(i) returns the ith agent in the environment
// you can check whether two agents are the same with ==
vec3 ASeparation::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	return vec3(0, 0, 0);
}


//--------------------------------------------------------------
// Cohesion behavior

ACohesion::ACohesion() : ABehavior("Cohesion")
{
	setParam("Neighborhood", 1);
	setParam("kCohesion", 1);
}

// Cohesion moves actors towards the center of a group of agents
vec3 ACohesion::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	return vec3(0, 0, 0);
}

//--------------------------------------------------------------
// Alignment behavior
AAlignment::AAlignment() : ABehavior("Alignment")
{
	setParam("Neighborhood", 1);
	setParam("kAlignment", 1);
}

// Alignment returns an average velocity of all active agents
vec3 AAlignment::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	return vec3(0);
}

//--------------------------------------------------------------
// Flocking behavior
AFlocking::AFlocking() : ABehavior("Flocking")
{
}

// Flocking combines separation, cohesion, and alignment
vec3 AFlocking::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	return vec3(0, 0, 0);
}

//--------------------------------------------------------------
// Leader
ALeader::ALeader() : ABehavior("Leader")
{
	setParam("CSeparation", 1);
	setParam("CCohesion", 1);
	setParam("CAlignment", 1);
	setParam("CArrival", 1);
}

// You need to find the leader, who is always the first agent in agents
// If the actor is the leader, move towards the target; otherwise, 
// follow the leader as a flock
vec3 ALeader::calculateDesiredVelocity(const ASteerable& actor,
	const AWorld& world, const vec3& target)
{
	return vec3(0, 0, 0);
}


