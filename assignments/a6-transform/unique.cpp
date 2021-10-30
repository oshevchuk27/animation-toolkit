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
        root->setLocalTranslation(vec3(0, 0, 30));
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
        joint10->setLocalTranslation(vec3(0, -15, 0));
        _character.addJoint(joint10, joint6);

        Joint* joint11 = new Joint("joint11");
        joint11->setLocalTranslation(vec3(0, -15, 0));
        _character.addJoint(joint11, joint7);


        _character.fk(); // compute local2global transforms

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
            //parent->setLocalRotation(glm::angleAxis(sin(1.5f * elapsedTime() + i), vec3(0, 0, 1)));
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

    }

protected:
    Skeleton _character;
};

int main(int argc, char** argv)
{
    Unique viewer;
    viewer.run();
}

