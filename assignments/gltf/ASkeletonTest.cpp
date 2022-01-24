#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"

using namespace atk;
using namespace std;
using namespace glm;

class ASkeletonTest : public atkui::Framework
{
public:
   ASkeletonTest() : atkui::Framework(atkui::Perspective) {
   }

   virtual void setup()
   {
      Joint* root = new Joint("joint0");
      Joint* joint1 = new Joint("joint1");
      Joint* joint2 = new Joint("joint2");
      root->setLocalTranslation(vec3(0, 0, 0));
      joint1->setLocalTranslation(vec3(0, 100, 0));
      joint2->setLocalTranslation(vec3(0, 100, 0));
      _skeleton.addJoint(root);
      _skeleton.addJoint(joint1, root);
      _skeleton.addJoint(joint2, joint1);
   }

   virtual void scene() {

      // try to edit vertices
      _factor = 0.7*sin(elapsedTime());
      _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0,0,1)));
      _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0,0,1)));
      _skeleton.fk();

      ASkeletonDrawer drawer;
      drawer.setJointRadius(10);
      drawer.draw(renderer, _skeleton);
   }

   float _factor = 1;
   Skeleton _skeleton;
};

int main(int argc, char** argv) {
   ASkeletonTest viewer;
   viewer.run(); 
}

