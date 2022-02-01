#include "atkui/framework.h"
using namespace glm;

class ParticleCubic : public atkui::Framework {
public:
	ParticleCubic() : atkui::Framework(atkui::Orthographic) {
	}

	void setup() {
		t = 0.0f;
	}

	void scene() {

		float duration = 5.0f;

		float t = fmod(elapsedTime(), 5);
		t = t / duration;


		vec3 p = (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow((1 - t), 2) * B1
			+ (3 * (float)pow(t, 2)) * (1 - t) * B2 + (float)pow(t, 3) * B3;
		setColor(vec3(1));
		drawSphere(p, 10);

		for (float t = 0.0f; t <= 1.0f; t += 0.001f) {
			setColor(vec3(0, 0, 1));
			vec3 p = (float)pow((1 - t), 3) * B0 + 3 * t * (float)pow((1 - t), 2) * B1
				+ (3 * (float)pow(t, 2)) * (1 - t) * B2 + (float)pow(t, 3) * B3;
			drawSphere(p, 2);
		}

	}

private:
	float t;
	vec3 B0 = vec3(100, 50, 0);
	vec3 B1 = vec3(150, 200, 0);
	vec3 B2 = vec3(250, 100, 0);
	vec3 B3 = vec3(300, 300, 0);

};

int main(int argc, char** argv) {
	ParticleCubic viewer;
	viewer.run();
}

