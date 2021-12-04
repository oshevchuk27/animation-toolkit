#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class Unique : public atkui::Framework
{
public:
	Unique() : atkui::Framework(atkui::Perspective) {}
	virtual ~Unique() {}

	void setup()
	{
		BVHReader reader;
		reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);

		_motion.update(_skeleton, 0);
		_drawer.color = vec3(1, 0, 0);
		theta = 0.0;
		thetaRate = 2.0;
		rightFootPos = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
		leftFootPos = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();


		leftFootRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
		rightFootRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();

		globalleftHandPos = _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();





	}

	void update()
	{
		_motion.update(_skeleton, elapsedTime());

		IKController ik;
		// TODO: Your code here

		_skeleton.getRoot()->setLocalTranslation(_skeleton.getRoot()->getGlobalTranslation() + vec3(5.0f * sin(theta), abs(20.0f * cos(theta)) - 25.0, 0));

		_skeleton.fk();



		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftFoot")->getID(), leftFootPos, 0.001);

		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:RightFoot")->getID(), rightFootPos, 0.001);




		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:LeftHand")->getID(), globalleftHandPos + vec3(0, 80, 0), 0.001);

		ik.solveIKAnalytic(_skeleton, _skeleton.getByName("Beta:Head")->getID(), _headTarget, 0.001);




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
		setColor(vec3(0, 0, 1));

		theta += thetaRate * dt();
		_headTarget.x = 0;
		_headTarget.y = 180;
		_headTarget.z = abs(50 * sin(theta));


		setColor(vec3(1));

		for (int i = 0; i < _skeleton.getNumJoints(); i++) {
			atk::Joint* joint = _skeleton.getByID(i);
			if (joint->getParent() == 0) continue;
			if (joint->getParent()->getName().find("Hand") != std::string::npos) continue; // skip hands

			glm::vec3 p1 = joint->getGlobalTranslation();
			glm::vec3 p2 = joint->getParent()->getGlobalTranslation();
			drawEllipsoid(p1, p2, 20);
			drawSphere(p2, 10);
			drawSphere(p1, 10);

		}

		Joint* head = _skeleton.getByName("Beta:Head");
		assert(head);

		atk::Joint* pParent = head->getParent();
		atk::Transform xform = pParent->getLocal2Global();

		setColor(glm::vec3(1, 1, 1));
		glm::vec3 eyePos(5, 19, 10 * 0.5f);
		drawSphere(xform.transformPoint(eyePos), 7);

		glm::vec3 eyePos1(-5, 19, 10 * 0.5f);
		drawSphere(xform.transformPoint(eyePos1), 7);

		setColor(glm::vec3(0, 0, 0));
		glm::vec3 pupilPos(5, 19, 10 * 0.7);
		drawSphere(xform.transformPoint(pupilPos), 0.5f * 10);

		setColor(glm::vec3(0, 0, 0));
		glm::vec3 pupilPos1(-5, 19, 10 * 0.7);
		drawSphere(xform.transformPoint(pupilPos1), 0.5f * 10);





	}

protected:
	Cyclops _drawer;
	Skeleton _skeleton;
	Motion _motion;

	float theta;
	float thetaRate;


	// Hand target positions
	vec3 _headTarget;
	vec3 leftFootPos;
	vec3 rightFootPos;
	quat globalParentRotRightFoot;
	quat globalParentRotLeftFoot;
	quat invGlobalParentRotRightFoot;
	quat invGlobalParentRotLeftFoot;
	quat leftFootRot;
	quat rightFootRot;
	vec3 globalleftHandPos;

};

int main(int argc, char** argv)
{
	Unique viewer;
	viewer.run();
	return 0;
}

