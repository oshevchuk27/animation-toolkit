#include "atkui/framework.h"
using namespace std;

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
    click = false;
    
  }

  virtual void scene() {
    
    // setting white color
    setColor(vec3(255, 255, 255));
    drawSphere(vec3(150, 250, 0), 150);
    drawSphere(vec3(350, 250, 0), 150);


    vec3 v1 = target - vec3(150, 250, 0);
    float theta = atan2(v1.y, v1.x);
    vec3 pupil1 = vec3(185, 250, 100);
    float a = 50.0;
  
    vec3 v1_1 = a * vec3(cos(theta), sin(theta), 0);
    vec3 p1 = v1_1 + vec3(150, 250, 100);

    vec3 v2 = target - vec3(350, 250, 0);
    theta = atan2(v2.y, v2.x);
    vec3 pupil2 = vec3(385, 250, 100);
    vec3 v1_2 = a * vec3(cos(theta), sin(theta), 0);
    vec3 p2 = v1_2 + vec3(350, 250, 100);

    // setting black color
    setColor(vec3(0));
    
    drawSphere(p1, 40);
    drawSphere(p2, 40);

    // setting color for the cursor
    setColor(vec3(1, 0, 0));
    drawSphere(target, 5);

  }

  void mouseMotion(int x, int y, int dx, int dy) {
      if (click) {
          _mouseX = x;
          _mouseY = height() - y;
          target = vec3(_mouseX, _mouseY, 0);
      }
  
   
  }

  void mouseUp(int but, int mods) {
      click = false;

  }


  void mouseDown(int button, int mods) {
      click = true;
  }

 private:
  int _mouseX;
  int _mouseY;
  float theta;
  bool click;
  vec3 target;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}
