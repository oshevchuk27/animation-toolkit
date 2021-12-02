#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include <algorithm>
#include <string>
#include "atkmath/matrix3.h"
#include <glm/mat3x3.hpp>   
#include "atkmath/quaternion.h"

using namespace atk;
using namespace glm;
using namespace std;
using atkmath::Matrix3;
using atkmath::Quaternion;
using atkmath::Vector3;

class ALooker : public atkui::Framework {
public:
	ALooker() : atkui::Framework(atkui::Perspective) {}
	virtual ~ALooker() {}

	void setup() {
		Motion motion;
		BVHReader reader;
		reader.load("../motions/Beta/idle.bvh", _skeleton, motion);
		motion.update(_skeleton, 0);

		_drawer.color = vec3(1, 0, 0);
		_head = _skeleton.getByName("Beta:Head");
	}

	void reset(Joint* head) {
		head->setLocalRotation(IdentityQ);
		head->fk();
	}

	void lookAtTarget(Joint* head, const vec3& target) {
		// TODO: Your code here
		vec3 globalPos = head->getGlobalTranslation();
		vec3 desiredPos = target - globalPos;
		vec3 normDesiredPos = vec3(desiredPos.x / length(desiredPos), desiredPos.y / length(desiredPos),
			desiredPos.z / length(desiredPos));
		vec3 x = normalize(glm::cross(vec3(0, 1, 0), normDesiredPos));
		vec3 y = normalize(glm::cross(normDesiredPos, x));

		Matrix3 rotMatrix = Matrix3(x[0], y[0], normDesiredPos[0], x[1], y[1], normDesiredPos[1],
			x[2], y[2], normDesiredPos[2]);

		Quaternion quat = Quaternion();
		quat.fromMatrix(rotMatrix);
		Vector3 rotAxis = Vector3();
		double angle;
		quat.toAxisAngle(rotAxis, angle);
		glm::quat globalParentRot = head->getParent()->getGlobalRotation();
		glm::quat inverseGlobalParentRot = glm::inverse(globalParentRot);

		head->setLocalRotation(inverseGlobalParentRot * glm::angleAxis((float)angle, vec3(rotAxis[0], rotAxis[1], rotAxis[2])));

		head->fk();
	}

	void scene() {
		float r = 100;
		float angle = elapsedTime();
		_target = vec3(r * cos(angle), r * sin(angle) + r, 150);

		reset(_head);
		lookAtTarget(_head, _target);
		_drawer.draw(_skeleton, *this);

		vec3 globalHeadPos = _head->getGlobalTranslation();
		vec3 globalForward = _head->getGlobalRotation() * vec3(0, 0, 1);

		setColor(vec3(0, 0, 1));
		drawLine(globalHeadPos, globalHeadPos + 250.0f * globalForward);
		drawSphere(_target, 5);
	}

	Cyclops _drawer;
	Skeleton _skeleton;
	Joint* _head;
	vec3 _target;
};

int main(int argc, char** argv) {
	ALooker viewer;
	viewer.run();
	return 0;
}

