#include "atkui/framework.h"
#include "spline.h"
#include "interpolator_catmullrom.h"
#include "interpolator_linear.h"
#include <iostream>
#include <algorithm>
#include "curve_editor.h"

using namespace glm;
using namespace std;


class Unique : public atkui::Framework
{
public:
	Unique() : atkui::Framework(atkui::Orthographic)
	{
	}

	virtual void setup() {

		baseColor = vec3(0, 0, 1);

		spline.appendKey(0, vec3(250, 130, 0));
		spline.appendKey(1, vec3(380, 60, 0));
		spline.appendKey(2, vec3(350, 200, 0));
		spline.appendKey(3, vec3(450, 300, 0));
		spline.appendKey(4, vec3(315, 300, 0));
		spline.appendKey(5, vec3(250, 450, 0));
		spline.appendKey(6, vec3(185, 300, 0));
		spline.appendKey(7, vec3(50, 300, 0));
		spline.appendKey(8, vec3(150, 200, 0));
		spline.appendKey(9, vec3(120, 60, 0));
		spline.appendKey(10, vec3(250, 130, 0));

		for (int i = 0; i < 100; i++)
		{
			keys.push_back(baseColor + vec3(randValue(), randValue(), randValue()));
		}

		catInterp2.computeControlPoints(keys);

	}

	virtual void scene() {



		if (linear) {

			spline.addInterpolationType("Linear", &linInterp);

			spline.setInterpolationType("Linear");

		}
		else if (catMullRom) {


			spline.addInterpolationType("Catmull-Rom", &catInterp1);

			spline.setInterpolationType("Catmull-Rom");

		}
		else if (hermite) {
			spline.addInterpolationType("Hermite", &hermInterp);

			spline.setInterpolationType("Hermite");
		}

		for (float i = 0.0f; i < spline.getDuration(); i += 0.001f) {
			setColor(catInterp2.interpolate(rand() % 10, dt()));
			drawSphere(spline.getValue(i), 5.0f);
		}


	}

	virtual float randValue() {
		float randValue = -1 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (1 + 1)));
		return randValue;
	}



	virtual void keyUp(int key, int mods) {
		if (key == GLFW_KEY_SPACE) {
			catMullRom = true;
			linear = false;
			hermite = false;
		}
		else if (key == '1') {
			catMullRom = false;
			linear = false;
			hermite = true;
		}
		else if (key == '2') {
			linear = true;
			catMullRom = false;
			hermite = false;
		}
	}

private:
	// interpolator for the shape
	InterpolatorCatmullRom catInterp1;
	// interpolator for color
	InterpolatorCatmullRom catInterp2;
	InterpolatorLinear linInterp;
	InterpolatorHermite hermInterp;
	Spline spline;
	// storing colors to interpolate between
	vector<vec3> keys;
	bool catMullRom = false;
	bool linear = true;
	bool hermite = false;
	vec3 baseColor;

};


int main(int argc, char** argv) {
	Unique unique;
	unique.run();
}

