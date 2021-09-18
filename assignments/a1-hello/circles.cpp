#include "atkui/framework.h"
#include <corecrt_math_defines.h>


using namespace glm;
class Circles : public atkui::Framework
{
public:
   Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
	   theta = 0.0;
	   thetaRate = 0.01;
   }

   virtual void scene() {
	   for (int i = 0; i < 16; i ++) {
		   bigRadius = 20 * i + 20;
		   if (i % 2 == 0) {
			   direction = 1;
		   } else {
			   direction = -1;
		   }
		   color = pallet[i % (pallet.size())];
		   theta += thetaRate * dt();
		   for (int j = 0; j < 36; j++) {
			  
			   float spacingAngle = 2 * M_PI * (10.0 / 360.0);
			   float px = bigRadius * cos(direction * (theta + spacingAngle * j)) + 0.5 * width();
			   float py = bigRadius * sin(direction * (theta + spacingAngle * j)) + 0.5 * height();
			   setColor(color);
			   drawSphere(vec3(px, py, 0), 15);

		   }
		  
	   }
   }
private:
	float theta;
	float thetaRate;
	float bigRadius;
	float direction;
	vec3 color;

	std::vector<vec3> pallet =
	{
	   vec3(0,165,227) / 255.0f,
	   vec3(141,215,191) / 255.0f,
	   vec3(255,150,197) / 255.0f,
	   vec3(255,87,104) / 255.0f,
	   vec3(255,162,58) / 255.0f
	};
};

int main(int argc, char** argv)
{
   Circles viewer;
   viewer.run();
   return 0;
}
