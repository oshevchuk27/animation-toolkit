#include "ik_controller.h"
#include <cmath>
#include <iostream>
//#include "atkmath/constants.h"

const double PI = 3.1415926535897932384626433832795;

using namespace atk;
using namespace glm;
using namespace std;

bool IKController::solveIKAnalytic(Skeleton& skeleton,
	int jointid, const vec3& goalPos, float epsilon) {
	if (jointid == -1) return true;

	Joint* ankle = skeleton.getByID(jointid);
	if (!ankle->getParent()) {
		std::cout << "Warning: solveIKAnalytic() needs joint "
			"with parent and grandparent\n";
		return false;
	}

	Joint* knee = ankle->getParent();
	if (!knee->getParent()) {
		std::cout << "Warning: solveIKAnalytic() needs joint "
			"with parent and grandparent\n";
		return false;
	}

	Joint* hip = knee->getParent();

	// TODO: Your code here

	int l1 = length(skeleton.getByID(jointid)->getParent()->getParent()->getGlobalTranslation() - skeleton.getByID(jointid)->getParent()->getGlobalTranslation());
	int l2 = length(skeleton.getByID(jointid)->getParent()->getGlobalTranslation() - skeleton.getByID(jointid)->getGlobalTranslation());

	double r_1 = length(goalPos - skeleton.getByID(jointid)->getParent()->getParent()->getGlobalTranslation());

	float input1 = (pow(r_1, 2) - pow(l1, 2) - pow(l2, 2)) / (-2.0f * l1 * l2);
	input1 = glm::clamp(input1, -1.0f, 1.0f);
	float phi_1 = acos(input1);

	float theta_2_z = phi_1 - PI;

	// axis of rotation for parent joint
	vec3 limbDir = normalize(skeleton.getByID(jointid)->getParent()->getLocalTranslation());
	vec3 axis = cross(limbDir, vec3(0, 0, -1));
	if (limbDir[1] < 0) axis = cross(limbDir, vec3(0, 0, 1));


	skeleton.getByID(jointid)->getParent()->setLocalRotation(glm::angleAxis((float)theta_2_z, axis));

	vec3 r_2 = skeleton.getByID(jointid)->getGlobalTranslation() - skeleton.getByID(jointid)->getParent()->getParent()->getGlobalTranslation();
	vec3 e = goalPos - skeleton.getByID(jointid)->getGlobalTranslation();

	float phi_2 = atan2(length(cross(r_2, e)), dot(r_2, r_2) + dot(r_2, e));

	quat globalParentRot = skeleton.getByID(jointid)->getParent()->getParent()->getParent()->getGlobalRotation();
	quat inverseGlobalParentRot = glm::inverse(globalParentRot);


	vec3 rotAxis = normalize(cross(r_2, e));

	if (length(rotAxis) >= 0.001) {
		skeleton.getByID(jointid)->getParent()->getParent()->setLocalRotation(inverseGlobalParentRot * glm::angleAxis((float)phi_2, rotAxis) * skeleton.getByID(jointid)->getParent()->getParent()->getGlobalRotation());
	}



	skeleton.fk();


	return true;
}

bool IKController::solveIKCCD(Skeleton& skeleton, int jointid,
	const vec3& goalPos, const std::vector<Joint*>& chain,
	float threshold, int maxIters, float nudgeFactor) {
	// There are no joints in the IK chain for manipulation
	if (chain.size() == 0) return true;

	vec3 p = skeleton.getByID(jointid)->getGlobalTranslation();
	int i = 0;
	while (length(goalPos - p) > threshold && i < maxIters) {

		cout << length(goalPos - p) << endl;

		for (Joint* j : chain) {

			vec3 e = goalPos - p;
			vec3 r = p - j->getGlobalTranslation();

			if (length(e) < 0.0001) continue;

			if (length(r) < 0.0001) continue;

			float phi = atan2(length(cross(r, e)), dot(r, r) + dot(r, e));

			if (phi < 0.0001) continue;

			float deltaPhi = nudgeFactor * phi;

			if (deltaPhi < 0.0001) continue;
			quat globalParentRot = j->getParent()->getGlobalRotation();
			quat inverseGlobalParentRot = glm::inverse(globalParentRot);

			vec3 rotAxis = normalize(cross(r, e));

			if (length(rotAxis) < 0.0001) {
				continue;
			}
			else {

				j->setLocalRotation(inverseGlobalParentRot * glm::angleAxis(deltaPhi, rotAxis) * j->getGlobalRotation());


				j->fk();

				//calculate new global position of end effector
				p = skeleton.getByID(jointid)->getGlobalTranslation();


			}
		}



		i++;
	}

	return false;
}
