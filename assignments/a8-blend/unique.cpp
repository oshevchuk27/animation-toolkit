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
        reader.load("../motions/14/14_01.bvh", _skeleton, _lower);
        reader.load("../motions/14/14_37.bvh", _skeleton, _upper);
        for (int i = _upper.getNumKeys() - 1; i >= 0; i--) {
            _upper.appendKey(_upper.getKey(i));
        }

        _splice = spliceUpperBody(_lower, _upper, _alpha);


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

    Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
    {
        Motion result;
        result.setFramerate(lower.getFramerate());
        // todo: your code here
        Joint* startUpper = _skeleton.getByName("chest");



        for (int i = 0; i < lower.getNumKeys(); i++) {
            Pose pose = lower.getKey(i);
            Pose newPose = pose;
            for (int j = 0; j < pose.jointRots.size(); j++) {
                if (isADescendant(startUpper, _skeleton.getByID(j))) {
                    newPose.jointRots[j] = glm::slerp(upper.getKey(i%upper.getNumKeys()).jointRots[j], pose.jointRots[j], 0.0f);
                }

            }
            result.appendKey(newPose);

        }

        return result;
    }

    void scene()
    {
        _splice.update(_skeleton, elapsedTime() * 0.5);
        setColor(vec3(randValue(), randValue(), randValue()));
        SkeletonDrawer drawer;
        drawer.draw(_skeleton, *this);
        //drawText("alpha: " + std::to_string(_alpha), 10, 15);
       


        for (int i = 0; i < 3; i++) {

            setColor(vec3(1, 0, 1));

            Skeleton _flower = _flowers[i];

            _flower.fk();
            for (int j = 0; j < _flower.getNumJoints(); j++) {

                if (_flower.getByID(j) == _flower.getRoot()) {
                    continue;
                }
              
                Joint* parent = _flower.getByID(j)->getParent();
                parent->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 1, 0)));
               

                Joint* child = _flower.getByID(j);
                child->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 1, 0)));

                for (int k = 0; k <= 500; k++) {

                    float r = 30 * cos((5 * k));
                    float x = r * cos(k);
                    float y = r * sin(k) + 100;

                    drawSphere(vec3(x, y, -100) + getPosition(i), 10);
                }


            }
        }

    }

    bool isADescendant(Joint* parent, Joint* child) {
        //int id = child.getID();
        Joint* j = child;

        while (j != NULL) {
            if (j->getName() == parent->getName()) {
                return true;
            }
            j = j->getParent();
        }
        return false;
    }

    void keyUp(int key, int mods)
    {
        if (key == GLFW_KEY_UP)
        {
            _alpha = std::min(_alpha + 0.05, 1.0);
            _splice = spliceUpperBody(_lower, _upper, _alpha);
            
        }
        else if (key == GLFW_KEY_DOWN)
        {
            _alpha = std::max(_alpha - 0.05, 0.0);
            _splice = spliceUpperBody(_lower, _upper, _alpha);
        }
    }

    virtual float randValue() {
        float randValue = 0 + static_cast <float> (rand()) /
            (static_cast <float> (RAND_MAX / (1 + 1)));
        return randValue;
    }

    virtual vec3 getPosition(int i) {
        Joint* head = _skeleton.getByName("head");
        Joint* rHand = _skeleton.getByName("rHand");
        Joint* lHand = _skeleton.getByName("lHand");


        vec3 v;
        if (i == 0) { v = head->getGlobalTranslation(); }
        else if (i == 1) { v = rHand->getGlobalTranslation(); }
        else { v = lHand->getGlobalTranslation(); }
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


