#include "atkui/framework.h"
#include <math.h>
#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"


using namespace glm;
using atkmath::Matrix3;
using atkmath::Vector3;
using atkmath::Deg2Rad;
using atkmath::Rad2Deg;


class Unique : public atkui::Framework {
public:
	Unique() : atkui::Framework(atkui::Orthographic) {
	}



	virtual void setup() {

		theta1 = 0.0;
		theta2 = 0.0;
		thetaRate = 0.2;
		rotRate = 0.8;
		r = 150.0;

	}

	virtual void scene() {

		theta1 += thetaRate * dt();
		theta2 += rotRate * dt();
		frameCount += 1;


		color = pallet[frameCount % (pallet.size())];;

		Matrix3 rot;
		Vector3 euler = Vector3(360, 360, 360);

		rot.fromEulerAnglesXYZ(euler * Deg2Rad);
		mat4 rotm = rot.writeToMat4();
		Vector3 testXYZ = rot.toEulerAnglesXYZ();
		testXYZ = testXYZ * Rad2Deg;

		Vector3 anglesinRad = euler * Deg2Rad;

		float px = r * cos(theta1) + 0.5 * width();
		float py = r * sin(theta1) + 0.5 * height();

		push();

		translate(vec3(px, py, 0));



		if (xRotation) {
			rotate(anglesinRad[0] + theta2, vec3(1, 0, 0));
		}
		else if (yRotation) {
			rotate(anglesinRad[1] + theta2, vec3(0, 1, 0));
		}
		else if (zRotation) {
			rotate(anglesinRad[2] + theta2, vec3(0, 0, 1));
		}


		// draw spiral
		for (int i = 0; i <= 75; i++) {
			float r = i;
			float x = r * cos(i);
			float y = r * sin(i);
			setColor(color);
			drawSphere(vec3(x, y, 0), 15);
			drawSphere(vec3(x, y, 0), 15);

		}

		pop();
	}


	virtual void keyUp(int key, int mods) {
		if (key == '1') {
			xRotation = true;
			yRotation = false;
			zRotation = false;
		}
		else if (key == '2') {
			xRotation = false;
			yRotation = true;
			zRotation = false;
		}
		else if (key == '3') {
			xRotation = false;
			yRotation = false;
			zRotation = true;
		}
	}


private:

	float theta1;
	float theta2;
	float thetaRate;
	float r;
	float rotRate;
	vec3 color;
	bool xRotation = false;
	bool yRotation = false;
	bool zRotation = false;
	int frameCount = 0;

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
