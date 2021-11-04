#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup() {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 position = vec3(-225, 0, -225);
     
      /*for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 4; j++) {
              _devil = Devil(position, color, size);
              devils.push_back(_devil);
              position.z += 150;
          }
          position.z = -225;
          position.x += 225;
     }*/

      // jittering positions
      for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 4; j++) {
              _devil = Devil(position, vec3(randColorValue(), randColorValue(), randColorValue()), randSizeValue());
              devils.push_back(_devil);
              position.z += 150 + randValue();
              position.x += randValue();
          }
          position.z = -225 + randValue();
          position.x += 225 + randValue();
      }
      
   }

   virtual void scene() {
      if (!_paused) _motion.update(_skeleton, elapsedTime());
      for (int i = 0; i < devils.size(); i++) {
          devils[i].draw(_skeleton, *this);
      }
      
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

   virtual float randValue() {
       float randValue = -30 + (float) ((rand()) %
           (30 + 30 + 1));
       return randValue;
   }

   virtual float randSizeValue() {
       float randSizeValue = 0.5 + static_cast <float> (rand()) /
           (static_cast <float> (RAND_MAX / (1 - 0.5)));
       return randSizeValue;
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
   Devil _devil;
   std::vector<Devil> devils;
};

int main(int argc, char** argv) {
   Thriller viewer;
   viewer.run();
}

