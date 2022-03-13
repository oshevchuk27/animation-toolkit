#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"

using namespace atk;
using namespace std;
using namespace glm;

class AGLTFSimple : public atkui::Framework {
public:
   AGLTFSimple() : atkui::Framework(atkui::Perspective) {
   }

   virtual void setup() {
      loadMotion("../motions/Mia/gesture.bvh");
      for (int i = 0; i < _skeleton.getNumJoints(); i++)
      {
         atk::Joint* joint = _skeleton.getByID(i);
         joint->setLocalTranslation(joint->getLocalTranslation());
      }

      _geometry.load("../models/Mia/Mia-v2.0.glb"); 
      _geometry.print();
   }

   virtual void scene() {

      _motion.update(_skeleton, elapsedTime());

      // modify mesh
      int nummesh = _geometry.getNumMeshes();
      for (int meshid = 0; meshid < nummesh; meshid++) {
         int numprims = _geometry.getNumPrimitives(meshid);
         for (int primid = 0; primid < numprims; primid++) {

            int numverts = _geometry.getNumVertices(meshid, primid, "POSITION");
            for (int vid = 0; vid < numverts; vid++) {

               vec4 pos = _geometry.getVertexData(meshid, primid, "POSITION", vid);
               //std::cout << elapsedTime() << " " << pos << std::endl;

               pos[1] = sin(elapsedTime())*100;
               _geometry.setVertexData(meshid, primid, "POSITION", vid, pos);
            }
         }
      }

      setColor(vec4(1, 1, 1, 1));
      renderer.push();
      renderer.rotate(-3.14/2.0, vec3(1,0,0));
      renderer.scale(vec3(100));
      _geometry.draw(renderer, _skeleton);
      renderer.pop();
   }

   virtual void keyPress(unsigned char key, int specialKey, int x, int y) {
   }

   virtual void loadDir(const std::string& dir) {
   }

   virtual void loadMotion(const std::string& filename) {
      atk::BVHReader reader;
      reader.load(filename, _skeleton, _motion);
   }

protected:
   Skeleton _skeleton;
   Motion _motion;
   AGLTFGeometry _geometry;
};

int main(int argc, char** argv)
{
   AGLTFSimple viewer;
   //viewer.loadMotion("../motions/Mia/idle.bvh");
   viewer.run(); 
}

