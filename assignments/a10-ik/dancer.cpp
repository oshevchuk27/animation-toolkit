#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
	ADancer() : atkui::Framework(atkui::Perspective) {}
	virtual ~ADancer() {}

	void setup()
	{
		BVHReader reader;
		reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);

		_motion.update(_skeleton, 0);
		_drawer.color = vec3(1, 0, 0);
		theta = 0.0;
		thetaRate = 3.0;
		rightFootPos = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
		leftFootPos = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();


		leftFootRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
		rightFootRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();

	}

	void update()
	{
		_motion.update(_skeleton, elapsedTime());

		IKController ik;
		// TODO: Your code here

		_skeleton.getRoot()->setLocalTranslation(_skeleton.getRoot()->getGlobalTranslation() + vec3(15.0f * sin(theta), abs(20.0f * cos(theta)) - 25.0, 0));

		_skeleton.fk();

		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftFoot")->getID(), leftFootPos, 0.001);

		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightFoot")->getID(), rightFootPos, 0.001);


		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftHand")->getID(), _lhandTarget, 0.001);

		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightHand")->getID(), _rhandTarget, 0.001);

		globalParentRotRightFoot = _skeleton.getByName("Beta:RightFoot")->getParent()->getGlobalRotation();
		globalParentRotLeftFoot = _skeleton.getByName("Beta:LeftFoot")->getParent()->getGlobalRotation();

		invGlobalParentRotRightFoot = glm::inverse(globalParentRotRightFoot);
		invGlobalParentRotLeftFoot = glm::inverse(globalParentRotLeftFoot);

		_skeleton.getByName("Beta:LeftFoot")->setLocalRotation(invGlobalParentRotLeftFoot * leftFootRot);
		_skeleton.getByName("Beta:RightFoot")->setLocalRotation(invGlobalParentRotRightFoot * rightFootRot);

		_skeleton.fk();


	}

	void scene()
	{
		update();
		_drawer.draw(_skeleton, *this);
		setColor(vec3(0, 0, 1));

		theta += thetaRate * dt();
		_lhandTarget.x = 25;
		_lhandTarget.y = 15 - 25.0f * sin(theta) + 90.0;
		_lhandTarget.z = 25;

		drawSphere(_lhandTarget, 10);

		_rhandTarget.x = -25;
		_rhandTarget.y = 15 + 25.0f * sin(theta) + 90.0;
		_rhandTarget.z = 25;

		drawSphere(_rhandTarget, 10);
	}

protected:
	Cyclops _drawer;
	Skeleton _skeleton;
	Motion _motion;

	float theta;
	float thetaRate;


	// Hand target positions
	vec3 _lhandTarget;
	vec3 _rhandTarget;
	vec3 leftFootPos;
	vec3 rightFootPos;
	quat globalParentRotRightFoot;
	quat globalParentRotLeftFoot;
	quat invGlobalParentRotRightFoot;
	quat invGlobalParentRotLeftFoot;
	quat leftFootRot;
	quat rightFootRot;
};

int main(int argc, char** argv)
{
	ADancer viewer;
	viewer.run();
	return 0;
}

