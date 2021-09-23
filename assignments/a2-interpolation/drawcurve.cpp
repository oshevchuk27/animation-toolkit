#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class DrawCubic : public atkui::Framework
{
 public:
  DrawCubic() : atkui::Framework(atkui::Orthographic, 400, 400) {
  }

  virtual void scene()
  {
    setColor(vec3(0,0,1));
    drawSphere(B0, 10);
    drawSphere(B3, 10);

    setColor(vec3(1,1,0));
    drawSphere(B1, 10);
    drawSphere(B2, 10);

    if (drawBernstein) {

        for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

            setColor(vec3(0, 0, 1));

            vec3 p = (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow((1 - t), 2) * B1 + (3 * (float)pow(t, 2)) * (1 - t) * B2 + (float)pow(t, 3) * B3;

            drawSphere(p, 5);
        }
    }

    if (drawCasteljau) {
        for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
           
            // level 1
            vec3 B0_1 = B0 * (1 - t) + B1 * t;
            vec3 B1_1 = B1 * (1 - t) + B2 * t;
            vec3 B2_1 = B2 * (1 - t) + B3 * t;
            // level 2
            vec3 B0_2 = B0_1 * (1 - t) + B1_1 * t;
            vec3 B1_2 = B1_1 * (1 - t) + B2_1 * t;
            // level 3
            vec3 B0_3 = B0_2 * (1 - t) + B1_2 * t;

            setColor(vec3(0, 0, 1));

            drawSphere(B0_3, 5);

        }
    }
  }

  void keyUp(int key, int mod) {
    if (key == GLFW_KEY_1) {
      // draw Bernstein
        drawBernstein = true;
     
    }
    else if (key == GLFW_KEY_2) {
      // draw Casteljau
        drawCasteljau = true;
    }
  }

 private:
  vec3 B0 = vec3(100,  50, 0);
  vec3 B1 = vec3(150, 200, 0);
  vec3 B2 = vec3(250, 100, 0);
  vec3 B3 = vec3(300, 300, 0);
  bool drawBernstein = false;
  bool drawCasteljau = false;
};

int main(int argc, char **argv) {
  DrawCubic viewer;
  viewer.run();
}
