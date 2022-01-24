#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"

using namespace atk;
using namespace std;
using namespace glm;

class AGLTFSimple : public atkui::Framework
{
public:
   AGLTFSimple() : atkui::Framework(atkui::Perspective) {
   }

   virtual void setup()
   {
      Joint* root = new Joint("joint0");
      Joint* joint1 = new Joint("joint1");
      Joint* joint2 = new Joint("joint2");
      root->setLocalTranslation(vec3(0, 0, 0));
      joint1->setLocalTranslation(vec3(0, 1, 0));
      joint2->setLocalTranslation(vec3(0, 1, 0));
      _skeleton.addJoint(root);
      _skeleton.addJoint(joint1, root);
      _skeleton.addJoint(joint2, joint1);

      //_geometry.load("../models/triangle.gltf");
      //_geometry.load("../models/cube.glb");
      //_geometry.load("../models/Borb.glb");
      //_geometry.load("../models/warrok.glb");
      //_geometry.load("../models/two-shapes.gltf");
      _geometry.load("../models/simpleSkin.gltf");
      _geometry.print(true);

      renderer.loadShader("skin", "../shaders/skin.vs", "../shaders/skin.fs");
   }

   virtual void scene() {

      // try to edit vertices
      _factor = 0.7*sin(elapsedTime());
      _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0,0,1)));
      _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0,0,1)));
      _skeleton.fk();

      renderer.beginShader("skin");
      setColor(vec3(0,1,0));
      push();
      scale(vec3(100,100,100));
      _geometry.draw(renderer, _skeleton);
      pop();
      renderer.endShader();

      ASkeletonDrawer drawer;
      drawer.setJointRadius(0.1);
      drawer.setScale(100);
      drawer.draw(renderer, _skeleton);
   }

   float _factor = 1;
   Skeleton _skeleton;
   AGLTFGeometry _geometry;
};

int main(int argc, char** argv) {
   AGLTFSimple viewer;
   viewer.run(); 
}

