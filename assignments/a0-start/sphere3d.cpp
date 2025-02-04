#include "atkui/framework.h"

using glm::vec3;
using agl::randomUnitVector;

class Sphere3D : public atkui::Framework {
 public:
     Sphere3D() : atkui::Framework(atkui::Perspective) {
     };
     // the initial position of the sphere
     vec3 currentPos;
     // the direction of the sphere movement
     vec3 velocity;
     // indicates whether the SPACEBAR key is pressed
     bool spaceKey;

   virtual void setup() {
       // initial position of the sphere
       currentPos = vec3(0);
       // initially, the SPACEBAR is not pressed
       spaceKey = false;
   }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of the world
    float radius = 50.0;

   
    if (spaceKey) {
        currentPos = currentPos + 100.0f * velocity * dt();
    }

    drawSphere(currentPos, radius);
  }

  virtual void keyUp(int key, int mods) {
      if (key == GLFW_KEY_SPACE) {
          spaceKey = true;
          velocity = randomUnitVector();
      }
      else if (key == GLFW_KEY_R) {
          spaceKey = false;
          // going back to the original position
          currentPos = vec3(0);
      }
  }
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}
