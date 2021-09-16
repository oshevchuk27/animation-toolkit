#include "atkui/framework.h"
using namespace std;

using namespace glm;

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
      struct Particle particle;
      for (int i = 0; i < 10; i++) {
          particle.position = vec3(0, i, 0);
          particle.color = vec3(i, 0, 0);
          particle.velocity = vec3(3, i + 5, 0);
          v1.push_back(particle);
      }
         
  }

  virtual void scene() {
      vec3 vel;
      vec3 currentPos;
      for (int i = 0; i < v1.size(); i++) {
          currentPos = v1[0].position + v1[0].velocity * dt();
          setColor(vec3(0, 0, 1));
          drawSphere(currentPos, 10);

      }
      
  }

private:
    vec3 currentPos;
    vec3 vel;
    vector<Particle> v1;
};

struct Particle {
    vec3 position;
    vec3 color;
    vec3 velocity;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
