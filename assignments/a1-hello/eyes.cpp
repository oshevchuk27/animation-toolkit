#include "atkui/framework.h"

using namespace glm;
class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }

    virtual void setup() {
        theta = 0.0;
        thetaRate = 0.02;
    }

    virtual void scene() {
        // setting white color
        setColor(vec3(255, 255, 255));
        drawSphere(vec3(150, 250, 0), 150);
        drawSphere(vec3(350, 250, 0), 150);
        // setting black color
        setColor(vec3(0));
        theta += thetaRate * elapsedTime();
        float py = 250;
        //float px = 250.0f;
        float px = 50.0f * sin(elapsedTime()) + 150.0;
        drawSphere(vec3(px, py, 50), 50);


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
