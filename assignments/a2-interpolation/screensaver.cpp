#include "atkui/framework.h"
using namespace glm;

using namespace std;

class Screensaver : public atkui::Framework {

	struct Curve {
		vec3 B0, B1, B2, B3;
		vec3 color;
	};


public:
	Screensaver() : atkui::Framework(atkui::Orthographic) {
	}

	void setup() {

		t = 0.0f;
		timer = 0.0f;
		baseColor = vec3(0.2, 0.8, 0.2);

		// control points for curve 1

		curve1.B0 = vec3(randX(), randY(), 0);
		curve1.B1 = vec3(randX(), randY(), 0);
		curve1.B2 = vec3(randX(), randY(), 0);
		curve1.B3 = vec3(randX(), randY(), 0);
		curve1.color = baseColor + vec3(randValue(), randValue(), randValue());

		// control points for curve 2

		curve2.B0 = vec3(randX(), randY(), 0);
		curve2.B1 = vec3(randX(), randY(), 0);
		curve2.B2 = vec3(randX(), randY(), 0);
		curve2.B3 = vec3(randX(), randY(), 0);
		curve2.color = baseColor + vec3(randValue(), randValue(), randValue());


	}

	void scene() {

		t += 0.04;

		// interpolated points

		current.B0 = curve1.B0 * (1 - t) + curve2.B0 * t;
		current.B1 = curve1.B1 * (1 - t) + curve2.B1 * t;
		current.B2 = curve1.B2 * (1 - t) + curve2.B2 * t;
		current.B3 = curve1.B3 * (1 - t) + curve2.B3 * t;
		current.color = curve1.color * (1 - t) + curve2.color * t;



		// draw curve1
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

			setColor(curve1.color);


			vec3 p = (float)pow((1 - t), 3) * curve1.B0 + 3 * t * (float)pow((1 - t), 2) * curve1.B1
				+ (3 * (float)pow(t, 2)) * (1 - t) * curve1.B2 + (float)pow(t, 3) * curve1.B3;

			drawSphere(p, 1.0);
		}

		// draw curve2
		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

			setColor(curve2.color);


			vec3 p = (float)pow((1 - t), 3) * curve2.B0 + 3 * t * (float)pow((1 - t), 2) * curve2.B1 +
				(3 * (float)pow(t, 2)) * (1 - t) * curve2.B2 + (float)pow(t, 3) * curve2.B3;

			drawSphere(p, 1.0);
		}




		if (t > 1) {

			t = 0;
			curve1.B0 = curve2.B0;
			curve1.B1 = curve2.B1;
			curve1.B2 = curve2.B2;
			curve1.B3 = curve2.B3;
			curve1.color = curve2.color;
			curve2 = randCurve();

		}


		timer += dt();

		if (timer > 0.3f) {
			timer = 0.0;

			curves.push_back(current);

		}

		if (curves.size() > 50.0) {
			curves.pop_front();
		}

		for (Curve& curve : curves) {



			// draw current curve
			for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

				setColor(curve.color);


				vec3 p = (float)pow((1 - t), 3) * curve.B0 + 3 * t * (float)pow((1 - t), 2) * curve.B1
					+ (3 * (float)pow(t, 2)) * (1 - t) * curve.B2 + (float)pow(t, 3) * curve.B3;

				drawSphere(p, 1.0);
			}



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
		float randValue = -10 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (10 + 10)));
		return randValue;
	}

	// generate random control points
	virtual Curve randCurve() {
		Curve randCurve;

		randCurve.B0 = vec3(randX(), randY(), 0);
		randCurve.B1 = vec3(randX(), randY(), 0);
		randCurve.B2 = vec3(randX(), randY(), 0);
		randCurve.B3 = vec3(randX(), randY(), 0);


		randCurve.color = baseColor + vec3(randValue(), randValue(), randValue());


		return randCurve;

	}

private:
	Curve curve1;
	Curve curve2;
	Curve current;
	float t;
	float timer;
	vec3 baseColor;
	std::list<Curve> curves;

};

int main(int argc, char** argv) {
	Screensaver viewer;
	viewer.run();
}