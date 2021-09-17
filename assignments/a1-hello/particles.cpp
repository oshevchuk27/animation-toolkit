#include "atkui/framework.h"
using namespace std;

using namespace glm;

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  struct Particle {
      vec3 position;
      vec3 color;
      float velocity;
  };

  virtual void setup() {
      Particle particle;
      baseColor = vec3(0.2, 0.8, 0.2);
     
      for (int i = 0; i < 400; i++) {
          particle.position = vec3(randX(), randY(), 0);
          particle.color = baseColor + vec3(randValue(), randValue(), randValue());
          particle.velocity = rand() % 100 + 40;
          v1.push_back(particle);

      }  
  }

  virtual void scene() {
      
      for (int i = 0; i < v1.size(); i++) {
          v1[i].position = v1[i].position + v1[i].velocity * dt();
          if (v1[i].position.x > width()){
              v1[i].position.x = 0;
          }
          if (v1[i].position.y > height()) {
              v1[i].position.y = 0;
          }
          setColor(v1[i].color);
          drawSphere(v1[i].position, 10);
      }
      
  }

  virtual float randValue() {
      float randValue = -0.4 + static_cast <float> (rand()) /
          (static_cast <float> (RAND_MAX / (0.4 + 0.4)));
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
    vector<Particle> v1;
    vec3 baseColor;
};



int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
