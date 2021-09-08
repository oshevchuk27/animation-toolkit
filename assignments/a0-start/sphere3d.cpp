#include "atkui/framework.h"

using glm::vec3;
using agl::randomUnitVector;

class Sphere3D : public atkui::Framework {
 public:
     Sphere3D() : atkui::Framework(atkui::Perspective) {
     };
     // the initial position of the sphere
     vec3 currentPos;
     vec3 velocity;
     vec3 direction;
     bool spaceKey;

   virtual void setup() {

       currentPos = vec3(0);
       velocity = vec3(100);
       direction = randomUnitVector();
       spaceKey = false;
   }


  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of the world
    float radius = 50.0;

    if (spaceKey) {
        currentPos = currentPos + velocity * dt() * direction;
    }

    drawSphere(currentPos, radius);



  }

 

  virtual void keyUp(int key, int mods) {
      if (key == GLFW_KEY_SPACE) {
          spaceKey = true;
      }
      else if (key == GLFW_KEY_R) {
          spaceKey = false;
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
