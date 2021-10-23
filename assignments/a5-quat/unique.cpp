#include <iostream>
#include "atkui/framework.h"
#include "atkmath/matrix3.h"
#include <Eigen/Dense>
using namespace glm;

class Unique : public atkui::Framework
{
public:
	Unique() : atkui::Framework(atkui::Orthographic)
	{
	}

	virtual void scene()
	{
		// drawing cursor
		setColor(vec3(1, 0, 0));
		drawSphere(target, 5);

		for (int i = 0; i < width(); i += 15)
		{
			for (int j = 0; j < height(); j += 15)
			{

				if (j == 0) {
					setColor(vec3(1, 0, 1));
				}
				else if (j == height() - 5) {
					setColor(vec3(0, 1, 1));
				}
				else {
					float t_y = j / height();
					t_y = glm::clamp(t_y, 0.0f, 1.0f);
					vec3 color = vec3(1, 0, 1) * (1 - t_y) + vec3(0, 1, 1) * t_y;
					setColor(color);

				}

				vec3 position = vec3(i, j, 0);
				float distance = sqrt(pow(position.x - target.x, 2) + pow(position.y - target.y, 2));


				if (distance <= 100) {


					vec3 v1 = target - position;
					theta = atan2(v1.y, v1.x);

					push();
					translate(position);
					rotate(theta, vec3(0, 0, 1));
					drawCube(vec3(0), vec3(15, 1, 0));
					pop();

				}
				else {
					drawCube(position, vec3(15, 1, 0));
				}
			}
		}

	}

	void mouseMotion(int pX, int pY, int dx, int dy)
	{
		if (click)
		{
			_mouseX = pX;
			_mouseY = height() - pY;
			target = vec3(_mouseX, _mouseY, 0);
		}
	}

	void mouseDown(int button, int mods)
	{

		click = true;

	}

	void mouseUp(int button, int mods)
	{
		click = false;
	}

private:
	int _mouseX;
	int _mouseY;
	float theta;
	bool click;
	vec3 target;
};

int main(int argc, char** argv)
{
	Unique viewer;
	viewer.run();
}
