#include "atkui/framework.h"
#include "devil.h"

using namespace glm;

using namespace atk;



class Unique : public atkui::Framework
{
public:
    Unique() : atkui::Framework(atkui::Perspective) {}
    virtual ~Unique() {}

    virtual void setup() {
        BVHReader reader;
        reader.load("../motions/14/14_01.bvh", _skeleton, _motion);


    }

    virtual void scene() {
        if (!_paused) _motion.update(_skeleton, elapsedTime());
        
        for (int i = 0; i < _skeleton.getNumJoints(); i++) {
            Joint* joint = _skeleton.getByID(i);
            if (joint->getParent() == 0) continue;

            vec3 p1 = joint->getGlobalTranslation();
            vec3 p2 = joint->getParent()->getGlobalTranslation();
            setColor(vec3(randColorValue(), randColorValue(), randColorValue()));
            drawEllipsoid(p1, p2, 10);
        }

      
    }

    virtual void keyUp(int key, int mods) {
        if (key == 'P') _paused = !_paused;
    }


   

    virtual float randColorValue() {
        float randColorValue = 0 + static_cast <float> (rand()) /
            (static_cast <float> (RAND_MAX / (1 + 1)));
        return randColorValue;
    }

protected:
    Motion _motion;
    Skeleton _skeleton;
    bool _paused = false;
    
};

int main(int argc, char** argv) {
    Unique viewer;
    viewer.run();
}




