#include "atkui/framework.h"
#include <math.h>


using namespace glm;

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  struct Bubble {
      vec3 position;
      vec3 color;
      float velocity;
      vec3 direction;
  };

  virtual void setup() {
      theta = 0.0;
      thetaRate = 0.2;
      r = 150.0;
      color = vec3(0, 165, 227) / 255.0f;
      Bubble bubble;
      for (int i = 0; i < 100; i++) {
          bubble.position = vec3(randX(), randY(), 0);
          bubble.color = vec3(0, 255, 255);
          bubble.velocity = rand() % 100 + 40;
          bubble.direction = vec3(0, 10, 0);
          v1.push_back(bubble);
          
      }
   
  }

  virtual void scene() {

      for (int i = 0; i < v1.size(); i++) {
          v1[i].position = v1[i].position + v1[i].velocity * v1[i].direction * 0.1f * dt();
          setColor(v1[i].color);
          drawSphere(v1[i].position, 20);
      }

      for (int i = 0; i < 5; i++) {

          int randNum = rand() % (5 - 2 + 1) + 2;

          for (int j = 0; j <= 500; j++) {

              float r = 100 * cos((randNum * j));
              float x = r * cos(j) + getPosition(i)[0];
              float y = r * sin(j) + getPosition(i)[1];

              setColor(pallet[i]);

              drawSphere(vec3(x, y, 0), 10);

             
          }

      }

      theta += thetaRate * dt();
      color = vec3(randValue(), randValue(), randValue());
      
      float px = r * cos(theta) + 0.5 * width();
      float py = r * sin(theta) + 0.5 * height();
      for (int i = 0; i <= 50; i++) {
          float r = i*0.5;
          float x = r * cos(i) + px;
          float y = r * sin(i) + py;
          setColor(color);
          drawSphere(vec3(x, y, 0), 5);
          drawSphere(vec3(x, y, 0), 5);

      }

  }

  virtual std::vector<float> getPosition(int i) {
      std::vector<float> v;
      if (i == 0) { v.push_back(50); v.push_back(50); }
      else if (i == 1) { v.push_back(50); v.push_back(450); }
      else if (i == 2) { v.push_back(450); v.push_back(50); }
      else if (i == 3) { v.push_back(450); v.push_back(450); }
      else { v.push_back(250); v.push_back(250); }
      return v;

  }

  virtual float randValue() {
      float randValue = -0.5 + static_cast <float> (rand()) /
          (static_cast <float> (RAND_MAX / (0.5 + 0.5)));
      return randValue;
  }

  virtual float randX() {
      float randX = ((float)rand() / RAND_MAX) * width();
      return randX;
  }

  virtual float randY() {
      float randY = ((float)rand() / RAND_MAX) * height();
      return randY;

  }

private:

    float theta;
    float thetaRate;
    float r;
    vec3 color;
    std::vector<Bubble> v1;

    std::vector<vec3> pallet =
    {
       vec3(0,165,227) / 255.0f,
       vec3(141,215,191) / 255.0f,
       vec3(255,150,197) / 255.0f,
       vec3(255,87,104) / 255.0f,
       vec3(255,162,58) / 255.0f
    };

 
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}

