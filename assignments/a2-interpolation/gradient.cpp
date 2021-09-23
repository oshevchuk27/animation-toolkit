#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
	  northWest = vec3(1, 1, 0);
	  northEast = vec3(0, 1, 1);
	  southWest = vec3(1, 0, 0);
	  southEast = vec3(1, 0, 1);
	  cubeSize = vec3(50, 50, 0);
  }

  virtual void scene() {
	  // drawing the edge cubes
	  setColor(northWest);
	  drawCube(vec3(25, 475, 0), cubeSize);
	  setColor(northEast);
	  drawCube(vec3(475, 475, 0), cubeSize);
	  setColor(southWest);
	  drawCube(vec3(25, 25, 0), cubeSize);
	  setColor(southEast);
	  drawCube(vec3(475, 25, 0), cubeSize);
	  for (int i = 25; i <= 475; i+=50) {
		  for (int j = 25; j <=475; j += 50) {
			  t_x = i / width();
			  t_x = glm::clamp(t_x, 0.0f, 1.0f); // clamp to range [0,1]
			  vec3 c_north = northWest * (1 - t_x) + northEast * t_x;
			  vec3 c_south = southWest * (1 - t_x) + southEast * t_x;
			  t_y = j / height();
			  t_y = glm::clamp(t_y, 0.0f, 1.0f); // clamp to range [0,1]
			  vec3 color = c_south * (1 - t_y) + c_north * t_y;
			  setColor(color);
			  drawCube(vec3(i, j, 0), cubeSize);
		  }
		  
	  }

	  

  }

private:
	float t_x;
	float t_y;
	vec3 northWest;
	vec3 northEast;
	vec3 southWest;
	vec3 southEast;
	vec3 cubeSize;
};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
