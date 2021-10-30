#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;


class Unique : public atkui::Framework {
public:
	Unique() : atkui::Framework(atkui::Perspective) {}
	virtual ~Unique() {}

	virtual void setup() {
		lookAt(vec3(200), vec3(0));

		Joint* root = new Joint("root");
		root->setLocalTranslation(vec3(0, 0, 0));
		_character.addJoint(root);

		Joint* joint1 = new Joint("joint1");
		joint1->setLocalTranslation(vec3(0, 30, 0));
		_character.addJoint(joint1, root);

		// todo: add more joints
		Joint* joint2 = new Joint("joint2");
		joint2->setLocalTranslation(vec3(0, 50, -30));
		_character.addJoint(joint2, joint1);

		Joint* joint3 = new Joint("joint3");
		joint3->setLocalTranslation(vec3(0, 50, 0));
		_character.addJoint(joint3, joint2);

		Joint* joint4 = new Joint("joint4");
		joint4->setLocalTranslation(vec3(0, -50, -30));
		_character.addJoint(joint4, joint2);

		Joint* joint5 = new Joint("joint5");
		joint5->setLocalTranslation(vec3(0, -30, 0));
		_character.addJoint(joint5, joint4);

		Joint* joint6 = new Joint("joint6");
		joint6->setLocalTranslation(vec3(0, -15, 30));
		_character.addJoint(joint6, joint3);

		Joint* joint7 = new Joint("joint7");
		joint7->setLocalTranslation(vec3(0, -15, -30));
		_character.addJoint(joint7, joint3);

		Joint* joint8 = new Joint("joint8");
		joint8->setLocalTranslation(vec3(0, 15, 0));
		_character.addJoint(joint8, joint3);

		Joint* joint9 = new Joint("joint9");
		joint9->setLocalTranslation(vec3(0, 10, 0));
		_character.addJoint(joint9, joint8);

		Joint* joint10 = new Joint("joint10");
		joint10->setLocalTranslation(vec3(0, 25, -15));
		_character.addJoint(joint10, joint7);

		Joint* joint11 = new Joint("joint11");
		joint11->setLocalTranslation(vec3(0, -25, 0));
		_character.addJoint(joint11, joint6);

		_character.fk(); // compute local2global transforms

		Joint* root_2 = new Joint("root_2");
		root_2->setLocalTranslation(vec3(0, 0, 100));
		_stem.addJoint(root_2);

		Joint* j1_0 = new Joint("j1_0");
		j1_0->setLocalTranslation(vec3(0, 35, 18));
		_stem.addJoint(j1_0, root_2);

		Joint* j1_1 = new Joint("j1_1");
		j1_1->setLocalTranslation(vec3(0, 35, -18));
		_stem.addJoint(j1_1, root_2);


		Joint* j1 = new Joint("j1");
		j1->setLocalTranslation(vec3(0, 35, 0));
		_stem.addJoint(j1, root_2);

		Joint* j2 = new Joint("j2");
		j2->setLocalTranslation(vec3(0, 35, 0));
		_stem.addJoint(j2, j1);

		// petals

		Joint* j4 = new Joint("j4");
		j4->setLocalTranslation(vec3(0, 27, 0));
		_stem.addJoint(j4, j2);

		Joint* j5 = new Joint("j5");
		j5->setLocalTranslation(vec3(0, 27, 0));
		_stem.addJoint(j5, j4);

		Joint* j6 = new Joint("j6");
		j6->setLocalTranslation(vec3(0, 18, 25));
		_stem.addJoint(j6, j4);

		Joint* j7 = new Joint("j7");
		j7->setLocalTranslation(vec3(0, 18, -25));
		_stem.addJoint(j7, j4);

		Joint* j8 = new Joint("j8");
		j8->setLocalTranslation(vec3(0, -15, -25));
		_stem.addJoint(j8, j4);

		Joint* j9 = new Joint("j9");
		j9->setLocalTranslation(vec3(0, -15, 25));
		_stem.addJoint(j9, j4);

		_stem.fk();

		for (int i = 0; i < 3; i++) {
			Skeleton _flower;
			Joint* root_3 = new Joint("root_3");
			root_3->setLocalTranslation(vec3(getPosition(i)[0], getPosition(i)[1], getPosition(i)[2]));
			_flower.addJoint(root_3);

			Joint* jf_1 = new Joint("jf_1");
			jf_1->setLocalTranslation(vec3(0, 30, 30));
			jf_1->setLocalRotation(glm::angleAxis(10.f, vec3(0, 1, 0)));
			_flower.addJoint(jf_1, root_3);



			_flower.fk();
			_flowers.push_back(_flower);

		}

	}

	virtual void scene()
	{
		// todo animate joints
		// hint: use glm::angleAxis(...) to compute quaternions for each joint

		_character.fk(); // computes local2global transforms
		setColor(vec3(0, 0, 1));

		// todo: loop over all joints and draw
		for (unsigned int i = 0; i < _character.getNumJoints(); i++) {
			if (_character.getByID(i) == _character.getRoot()) {
				continue;
			}
			Joint* parent = _character.getByID(i)->getParent();
			if (i == _character.getNumJoints() - 2) {
				parent->setLocalRotation(glm::angleAxis(sin(1.5f * elapsedTime() + i), vec3(1, 0, 0)));
			}


			Joint* child = _character.getByID(i);
			vec3 globalParentPos = parent->getGlobalTranslation();
			vec3 globalPos = child->getGlobalTranslation();
			if (i == 9) {
				drawEllipsoid(globalParentPos, globalPos, 15);
			}
			else if (i == 8) {
				drawEllipsoid(globalParentPos, globalPos, 5);
			}
			else {
				drawEllipsoid(globalParentPos, globalPos, 7);

			}

		}

		_stem.fk();


		setColor(vec3(0, 1, 0));


		for (unsigned int i = 0; i < _stem.getNumJoints(); i++) {

			if (_stem.getByID(i) == _stem.getRoot()) {
				continue;
			}
			Joint* parent = _stem.getByID(i)->getParent();
			parent->setLocalRotation(glm::angleAxis(sin(0.5f * elapsedTime() + i), vec3(1, 0, 0)));
			Joint* child = _stem.getByID(i);
			vec3 globalParentPos = parent->getGlobalTranslation();
			vec3 globalPos = child->getGlobalTranslation();
			drawEllipsoid(globalParentPos, globalPos, 7);

			if (i >= 5) {
				setColor(vec3(randValue(), randValue(), randValue()));
				drawEllipsoid(globalParentPos, globalPos, 12);
			}

		}


		for (int i = 0; i < 3; i++) {

			setColor(vec3(1, 0, 1));

			Skeleton _flower = _flowers[i];

			_flower.fk();
			for (int j = 0; j < _flower.getNumJoints(); j++) {

				if (_flower.getByID(j) == _flower.getRoot()) {
					continue;
				}
				// strange, but guarantees smooth flower with stem rotation
				Joint* parent = _stem.getByID(j)->getParent();
				parent->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 1, 0)));
				parent = _flower.getByID(j)->getParent();

				Joint* child = _flower.getByID(j);
				child->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 1, 0)));

				for (int k = 0; k <= 500; k++) {

					float r = 30 * cos((5 * k));
					float x = r * cos(k);
					float y = r * sin(k) + 100;

					drawSphere(vec3(x, y, -100), 10);
				}


			}
		}




	}

	virtual float randValue() {
		float randValue = 0 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (1 + 1)));
		return randValue;
	}

	virtual std::vector<float> getPosition(int i) {
		std::vector<float> v;
		if (i == 0) { v.push_back(100); v.push_back(100); v.push_back(100); }
		else if (i == 1) { v.push_back(0); v.push_back(30); v.push_back(0); }
		else { v.push_back(0); v.push_back(30); v.push_back(0); }
		return v;

	}

protected:
	Skeleton _character;
	Skeleton _stem;
	std::vector<Skeleton> _flowers;
};

int main(int argc, char** argv)
{
	Unique viewer;
	viewer.run();
}

