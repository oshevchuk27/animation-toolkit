#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }

    virtual void setup() {
        thetaRate = 2.0;
    }

    virtual void scene() {
        // setting white color
        setColor(vec3(255, 255, 255));
        drawSphere(vec3(150, 250, 0), 150);
        drawSphere(vec3(350, 250, 0), 150);
        // setting black color
        setColor(vec3(0));
        theta = thetaRate * elapsedTime();
        float py1 = 250;
        float px1 = 50.0f * sin(theta) + 150.0;
        drawSphere(vec3(px1, py1, 100), 40);
        float px2 = 50.0f * sin(theta) + 350.0;
        drawSphere(vec3(px2, py1, 100), 40);
    }

   private:
       float theta;
       float thetaRate;
};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}
