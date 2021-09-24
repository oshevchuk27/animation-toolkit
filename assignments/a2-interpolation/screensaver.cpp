#include "atkui/framework.h"
using namespace glm;

using namespace std;

class Screensaver : public atkui::Framework {
public:
	Screensaver() : atkui::Framework(atkui::Orthographic) {
	}

	void setup() {

		t = 0.0f;
		baseColor = vec3(0.2, 0.8, 0.2);

		// control points for curve 1
		vec3 B0 = randCtrlPoints()[0];
		vec3 B1 = randCtrlPoints()[1];
		vec3 B2 = randCtrlPoints()[2];
		vec3 B3 = randCtrlPoints()[3];
		vec3 color = baseColor + vec3(randValue(), randValue(), randValue());

		curve1.push_back(B0);
		curve1.push_back(B1);
		curve1.push_back(B2);
		curve1.push_back(B3);
		curve1.push_back(color);

		// control points for curve 2
		B0 = randCtrlPoints()[0];
		B1 = randCtrlPoints()[1];
		B2 = randCtrlPoints()[2];
		B3 = randCtrlPoints()[3];
		color = baseColor + vec3(randValue(), randValue(), randValue());


		curve2.push_back(B0);
		curve2.push_back(B1);
		curve2.push_back(B2);
		curve2.push_back(B3);
		curve2.push_back(color);


	}

	void scene() {

		vec3 B0_1 = curve1[0] * (1 - t) + curve2[0] * t;
		vec3 B1_1 = curve1[1] * (1 - t) + curve2[1] * t;
		vec3 B2_1 = curve1[2] * (1 - t) + curve2[2] * t;
		vec3 B3_1 = curve1[3] * (1 - t) + curve2[3] * t;
		vec3 color_1 = curve1[4] * (1 - t) + curve2[4] * t;

		current.push_back(B0_1);
		current.push_back(B1_1);
		current.push_back(B2_1);
		current.push_back(B3_1);
		current.push_back(color_1);



		// draw curve1
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

			setColor(curve1[4]);


			vec3 p = (float)pow((1 - t), 3) * curve1[0] + 3 * t * (float)pow((1 - t), 2) * curve1[1] + (3 * (float)pow(t, 2)) * (1 - t) * curve1[2] + (float)pow(t, 3) * curve1[3];

			drawSphere(p, 5);
		}

		// draw curve2
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

			setColor(curve2[4]);


			vec3 p = (float)pow((1 - t), 3) * curve2[0] + 3 * t * (float)pow((1 - t), 2) * curve2[1] + (3 * (float)pow(t, 2)) * (1 - t) * curve2[2] + (float)pow(t, 3) * curve2[3];

			drawSphere(p, 5);
		}



		t += dt();


		if (t > 1) {
			t = 0;
			curve1[0] = curve2[0];
			curve1[1] = curve2[1];
			curve1[2] = curve2[2];
			curve1[3] = curve2[3];
			curve1[4] = curve2[4];
			curve2 = randCtrlPoints();


		}


		// draw current curve
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

			setColor(color_1);


			vec3 p = (float)pow((1 - t), 3) * B0_1 + 3 * t * (float)pow((1 - t), 2) * B1_1 + (3 * (float)pow(t, 2)) * (1 - t) * B2_1 + (float)pow(t, 3) * B3_1;

			drawSphere(p, 5);
		}


	}


	virtual float randX() {
		float randX = ((float)rand() / RAND_MAX) * width();
		return randX;
	}

	virtual float randY() {
		float randY = ((float)rand() / RAND_MAX) * height();
		return randY;

	}

	virtual float randValue() {
		float randValue = -2 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (2 + 2)));
		return randValue;
	}

	// generate random control points
	virtual std::vector<vec3> randCtrlPoints() {
		vector<vec3> v;
		for (int i = 0; i < 4; i++) {
			v.push_back(vec3(randX(), randY(), 0));
		}

		vec3 color = baseColor + vec3(randValue(), randValue(), randValue());
		v.push_back(color);

		return v;

	}

private:
	std::vector<vec3> curve1;
	std::vector<vec3> curve2;
	std::vector<vec3> current;
	float t;
	vec3 baseColor;

};

int main(int argc, char** argv) {
	Screensaver viewer;
	viewer.run();
}

