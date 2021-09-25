#include "atkui/framework.h"
using namespace glm;

class DrawMovingCurve : public atkui::Framework
{
public:
	DrawMovingCurve() : atkui::Framework(atkui::Orthographic, 450, 450) {
	}

	struct Curve {
		vec3 B0, B1, B2, B3;
		vec3 color;
	};

	virtual void setup() {

		thetaRate = 1.0f;

		baseColor = vec3(0.2, 0.8, 0.2);


		float B0_B3_x = 50.0;
		float B1_x = 100.0;
		float B2_x = 0.0;



		// initialize a vector of curves
		for (int i = 0; i < 8; i++) {

			curve.B0 = vec3(B0_B3_x, 0, 0);
			curve.B1 = vec3(B1_x, 100, 0);
			curve.B2 = vec3(B2_x, 350, 0);
			curve.B3 = vec3(B0_B3_x, 450, 0);

			curves.push_back(curve);

			B0_B3_x += 50;
			B1_x += 50;
			B2_x += 50;

		}

		curves[0].color = vec3(1, 0, 1);
		curves[7].color = vec3(0, 1, 1);


		// gradient
		for (int i = 1; i < 7; i++) {

			float time = curves[i].B0.x / width();
			time = glm::clamp(time, 0.0f, 1.0f); // clamp to range [0,1]
			curves[i].color = curves[0].color * (1 - time) + curves[7].color * time;


		}



	}

	virtual void scene()
	{

		theta = thetaRate * elapsedTime();

		for (int i = 0; i < curves.size(); i++) {

			curves[i].B1.x = 200.0f * sin(theta) + 0.5 * height();
			curves[i].B2.x = width() - curves[i].B1.x;


			for (float t = 0.0f; t <= 1.0f; t += 0.001f) {

				setColor(curves[i].color);

				vec3 p = (float)pow((1 - t), 3) * curves[i].B0 + 3 * t * (float)pow((1 - t), 2) * curves[i].B1 + 
					(3 * (float)pow(t, 2)) * (1 - t) * curves[i].B2 + (float)pow(t, 3) * curves[i].B3;

				drawSphere(p, 5);
			}
		}

	}

	virtual float randValue() {
		float randValue = -3 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (3 + 3)));
		return randValue;
	}


private:

	std::vector<Curve> curves;
	Curve curve;
	float theta;
	float thetaRate;
	vec3 baseColor;
};

int main(int argc, char** argv) {
	DrawMovingCurve viewer;
	viewer.run();
}

