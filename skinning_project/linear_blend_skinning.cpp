#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using glm::vec3;

class LinearBlendSkinning : public atkui::Framework {
public:
	LinearBlendSkinning() : atkui::Framework(atkui::Perspective) {}
	virtual ~LinearBlendSkinning() {}

	virtual void setup() {
		lookAt(vec3(200), vec3(0));

		Joint* root = new Joint("root");
		root->setLocalTranslation(vec3(0, 0, 0));
		skeleton.addJoint(root);

		Joint* joint1 = new Joint("joint1");
		joint1->setLocalTranslation(vec3(0, 30, 0));
		skeleton.addJoint(joint1, root);

		Joint* joint2 = new Joint("joint2");
		joint2->setLocalTranslation(vec3(0, 30, 0));
		skeleton.addJoint(joint2, joint1);

		skeleton.fk();

	}

	virtual void scene() {
		skeleton.fk();
		setColor(vec3(0, 0, 1));
		for (unsigned int i = 0; i < skeleton.getNumJoints(); i++) {
			if (skeleton.getByID(i) == skeleton.getRoot()) {
				continue;
			}
			Joint* parent = skeleton.getByID(i)->getParent();
			if (i == skeleton.getNumJoints() - 1) {
				parent->setLocalRotation(glm::angleAxis(sin(1.5f * elapsedTime() + i), vec3(1, 0, 0)));
			}
			Joint* child = skeleton.getByID(i);
			vec3 globalParentPos = parent->getGlobalTranslation();
			vec3 globalPos = child->getGlobalTranslation();
			drawEllipsoid(globalParentPos, globalPos, 7);

		}
	}


protected:
	Skeleton skeleton;


};

int main(int argc, char** argv)
{
	LinearBlendSkinning viewer;
	viewer.run();
}