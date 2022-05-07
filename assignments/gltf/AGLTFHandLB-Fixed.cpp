#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"
#include "atkmath/constants.h"

using namespace atk;
using namespace std;
using namespace glm;
using glm::dualquat;
using glm::quat;

class AGLTFSimple : public atkui::Framework
{
  public:
    AGLTFSimple() : atkui::Framework(atkui::Perspective) {
    }

    virtual void setup()
    {
      atk::BVHReader reader;
      reader.load("../motions/hand.bvh", _skeleton, _motion);

      vec3 center = vec3(0, 0, 0);
      vec3 dim = vec3(50, 70, 50);
      setupPerspectiveScene(center, dim);
      lookAt(vec3(25), vec3(0));

      _geometry.load("../models/hand2.glb");
      _origGeometry.load("../models/hand2.glb"); // need to keep original vertices
      _geometry.print(false);

      for (int i = 0; i < _geometry.getNumSkinJoints(0); i++) {
        std::string name = _geometry.getJointName(0, i);
        Joint* joint = _skeleton.getByName(name);
        if (! joint) continue;

        mat4 M = inverse(_geometry.getInverseBindMatrix(0, i));
        if (i > 0) {
          mat4 PM = joint->getParent()->getLocal2Global().matrix();
          M = inverse(PM) * M; 
        }
        vec3 t = vec3(M[3]);
        quat r = quat(M);
        
        joint->setLocalTranslation(t);
        joint->setLocalRotation(r);
        _skeleton.fk();
      }
    }

    virtual void draw() {

      // try to edit vertices
      float factor = sin(elapsedTime());
      float theta = elapsedTime();
     _skeleton.getByName("Bone.002")->setLocalRotation(glm::angleAxis(factor, vec3(0, 0, 1)));
      _skeleton.getRoot()->setLocalTranslation(10.0f * vec3(cos(theta), 0, sin(theta)));
      _skeleton.getRoot()->setLocalRotation(glm::angleAxis(factor, vec3(0, 0, 1)));
      _skeleton.fk();

      int nummesh = _geometry.getNumMeshes();

      for (int meshid = 0; meshid < nummesh; meshid++) {
        int numprims = _geometry.getNumPrimitives(meshid);

        for (int primid = 0; primid < numprims; primid++) {
          int numverts = _geometry.getNumVertices(meshid, primid, "POSITION");

          for (int vid = 0; vid < numverts; vid++) {

            vec4 weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
            vec4 joints = _geometry.getVertexData(meshid, primid, "JOINTS_0", vid);

            //std::cout << joints << std::endl;
            // Need to use original vertices for calculations!
            vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
            pos[3] = 1; // homogeneous coordinate

            vec4 newpos = vec4(0);
            for (int i = 0; i < 4; i++) {
              std::string name = _geometry.getJointName(0, (int)joints[i]);
              atk::Joint* joint = _skeleton.getByName(name);
              if (!joint) std::cout << "Error: cannot find name! " << name << std::endl;

              mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
              mat4 local2global = joint->getLocal2Global().matrix();
              newpos += weights[i] * local2global * invMatrix * pos;
            } 

            _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);
          }
        }

      } 

      _geometry.update();

      setColor(vec3(0, 1, 0));
      _geometry.draw(renderer);

      ASkeletonDrawer drawer;
      drawer.setJointRadius(1.0);
      //drawer.setScale(100);
      drawer.draw(renderer, _skeleton);
      drawFloor(200, 4, 1);
    }

  private:

    Skeleton _skeleton;
    Motion _motion;
    AGLTFGeometry _geometry;
    AGLTFGeometry _origGeometry;
};

int main(int argc, char** argv) {
  AGLTFSimple viewer;
  viewer.run();
}




