#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class Unique : public atkui::Framework
{
public:
	Unique() : atkui::Framework(atkui::Perspective) {}
	virtual ~Unique() {}

	void setup()
	{
		_alpha = 0;

		BVHReader reader;
		reader.load("../motions/14/14_14.bvh", _skeleton, _lower);
		reader.load("../motions/14/14_37.bvh", _skeleton, _upper);
		for (int i = _upper.getNumKeys() - 1; i >= 0; i--) {
			_upper.appendKey(_upper.getKey(i));
		}

		_splice = spliceUpperBody(_lower, _upper, _alpha);


		for (int i = 0; i < 5; i++) {
			Skeleton _flower;
			Joint* root_3 = new Joint("root_3");
			_flower.addJoint(root_3);

			Joint* jf_1 = new Joint("jf_1");
			_flower.addJoint(jf_1, root_3);



			_flower.fk();
			_flowers.push_back(_flower);

		}
	}

	Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
	{
		Motion result;
		result.setFramerate(lower.getFramerate());
		// todo: your code here
		Joint* startUpper = _skeleton.getByName("abdomen");



		for (int i = 0; i < lower.getNumKeys(); i++) {
			Pose pose = lower.getKey(i);
			Pose newPose = pose;
			for (int j = 0; j < pose.jointRots.size(); j++) {
				if (isADescendant(startUpper, _skeleton.getByID(j))) {
					newPose.jointRots[j] = glm::slerp(upper.getKey(i % upper.getNumKeys()).jointRots[j], pose.jointRots[j], 0.0f);
				}

			}
			result.appendKey(newPose);

		}

		return result;
	}

	void scene()
	{
		_splice.update(_skeleton, elapsedTime() * 0.5);



		for (int i = 0; i < _skeleton.getNumJoints(); i++) {
			Joint* joint = _skeleton.getByID(i);
			if (joint->getParent() == 0) continue;

			vec3 p1 = joint->getGlobalTranslation();
			vec3 p2 = joint->getParent()->getGlobalTranslation();
			setColor(vec3(randValue(), randValue(), randValue()));
			drawEllipsoid(p1, p2, 9);
		}




		for (int i = 0; i < 5; i++) {

			setColor(vec3(randValue(), randValue(), randValue()));

			Skeleton _flower = _flowers[i];

			int randNumPetals = randPetalNum();

			_flower.fk();
			for (int j = 0; j < _flower.getNumJoints(); j++) {

				if (_flower.getByID(j) == _flower.getRoot()) {
					continue;
				}

				Joint* parent = _flower.getByID(j)->getParent();
				Joint* child = _flower.getByID(j);

				for (int k = 0; k <= 500; k++) {

					float r = 30 * cos((randNumPetals * k));
					float x = r * cos(k);
					float y = r * sin(k) + 100;

					drawSphere(vec3(x, y, -100) + getPosition(i), 10);
				}


			}
		}

	}

	bool isADescendant(Joint* parent, Joint* child) {
		Joint* j = child;

		while (j != NULL) {
			if (j->getName() == parent->getName()) {
				return true;
			}
			j = j->getParent();
		}
		return false;
	}



	virtual float randValue() {
		float randValue = 0 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (1 + 1)));
		return randValue;
	}

	virtual float randPetalNum() {
		float randValue = 4 + (float)((rand()) %
			(8 - 4 + 1));
		return randValue;
	}

	virtual vec3 getPosition(int i) {

		std::vector<Joint*> joints;
		joints.push_back(_skeleton.getByName("head"));
		joints.push_back(_skeleton.getByName("rHand"));
		joints.push_back(_skeleton.getByName("lHand"));
		joints.push_back(_skeleton.getByName("rButtock"));
		joints.push_back(_skeleton.getByName("lButtock"));



		vec3 v = joints[i]->getGlobalTranslation();

		return v;


	}

	Skeleton _skeleton;
	Motion _upper;
	Motion _lower;
	Motion _splice;
	float _alpha;
	std::vector<Skeleton> _flowers;
};

int main(int argc, char** argv)
{
	Unique viewer;
	viewer.run();
	return 0;
}


