#include "atkui/framework.h"
#include <map>

using namespace agl;
using namespace atk;
using namespace glm;

struct Sprite {
	std::string name;
	float rotation;
	vec3 pivot;
	Image image;
};

class Unique : public atkui::Framework {

public:
	Unique() : atkui::Framework(atkui::Orthographic) {
	}

	virtual void setup() {
		loadSprite("body", vec3(0.5, 0.5f, 0.0f), 0, "../models/character/body.png");
		loadSprite("head", vec3(0.5, 0.1f, 0.0f), 0, "../models/character/head.png");
		loadSprite("head1", vec3(0.5, 0.1f, 0.0f), 0, "../models/character/head1.png");
		loadSprite("head2", vec3(0.5, 0.1f, 0.0f), 0, "../models/character/head2.png");
		loadSprite("arm1", vec3(0.1, 0.9f, 0.0f), 0.3, "../models/character/rarm.png");
		loadSprite("arm2", vec3(0.8, 0.9f, 0.0f), -0.3, "../models/character/larm.png");
		loadSprite("leg1", vec3(0.2, 1.0f, 0.0f), -0.2, "../models/character/lleg.png");
		loadSprite("leg2", vec3(0.2, 0.9f, 0.0f), 0, "../models/character/rleg.png");


		renderer.blendMode(agl::BLEND);
		background(vec3(0.8));
	}

	virtual void scene() {
		drawShadow();

		const Sprite& body = _sprites["body"];
		const Sprite& head = _sprites["head"];
		const Sprite& head1 = _sprites["head1"];
		const Sprite& head2 = _sprites["head2"];
		const Sprite& arm1 = _sprites["arm1"];
		const Sprite& arm2 = _sprites["arm2"];
		const Sprite& leg1 = _sprites["leg1"];
		const Sprite& leg2 = _sprites["leg2"];

		push();
		translate(vec3(width() * 0.5, height() * 0.5, 0));
		rotate(body.rotation, vec3(0, 0, 1));


		// arm2
		push();
		translate(vec3(-20, 55, -40));
		rotate(arm2.rotation, vec3(0, 0, 1));
		translate(-arm2.pivot);
		drawSprite(arm2);
		pop();


		// arm1
		push();
		translate(vec3(20, 47, -30));
		rotate(arm1.rotation, vec3(0, 0, 1));
		translate(-arm1.pivot);
		drawSprite(arm1);
		pop();




		// leg2
		push();
		translate(vec3(23, -70, -10));
		rotate(leg2.rotation, vec3(0, 0, 1));
		translate(-leg2.pivot);
		drawSprite(leg2);
		pop();


		// body
		push();
		translate(-body.pivot);
		drawSprite(body);
		pop();



		if (drawHead && !drawHead1 && !drawHead2) {
			//head
			push();
			translate(vec3(-15, 101, 20));
			scale(vec3(0.7, 0.7, 0.7));
			rotate(head.rotation, vec3(0, 0, 1));
			translate(-head.pivot);
			drawSprite(head);
			pop();


		}
		else if (drawHead1 && !drawHead && !drawHead2) {

			//head1
			push();
			translate(vec3(-15, 101, 20));
			scale(vec3(0.7, 0.7, 0.7));
			rotate(head1.rotation, vec3(0, 0, 1));
			translate(-head1.pivot);
			drawSprite(head1);
			pop();
		}
		else if (drawHead2 && !drawHead1 && !drawHead) {


			//head2
			push();
			translate(vec3(-15, 101, 20));
			scale(vec3(0.7, 0.7, 0.7));
			rotate(head2.rotation, vec3(0, 0, 1));
			translate(-head2.pivot);
			drawSprite(head2);
			pop();

		}


		//leg1
		push();
		translate(vec3(-18, -58, 30));
		rotate(leg1.rotation, vec3(0, 0, 1));
		translate(-leg1.pivot);
		drawSprite(leg1);
		pop();


		pop();

		renderer.fontColor(vec4(0, 0, 0, 1));
		drawText("Selected: " + _selected, 5, 25);
	}

	void drawSprite(const Sprite& sprite) {
		renderer.beginShader("sprite2d");
		renderer.texture("image", sprite.name);
		scale(vec3(sprite.image.width(), sprite.image.height(), 1));
		renderer.sprite(vec3(0, 0, 0), vec4(1.0), 1.0f);
		renderer.endShader();
	}

	void loadSprite(const std::string& name, const vec3& pivotOffset, float startRot, const std::string& filename) {
		Image image;
		bool success = image.load(filename, true);

		if (success) {
			renderer.loadTexture(name, image, 1);
			vec3 pivot = vec3(image.width(), image.height(), 0) * pivotOffset;
			_sprites[name] = Sprite{ name,startRot,pivot,image };

			std::cout << "load " << name << ": (" << image.width() << ", " << image.height()
				<< ") pivot: " << glm::to_string(pivot) << std::endl;

		}
		else {
			std::cout << "Error: cannot load " << filename << std::endl;
		}
	}

	void drawShadow() {
		setColor(vec3(0.75));
		push();
		translate(vec3(width() * 0.5, height() * 0.05, -300));
		scale(vec3(1, 0.1, 0.1));
		drawSphere(vec3(0), 300);
		pop();
	}

	void keyUp(int keyChar, int mods) {
		int key = keyChar - '1'; // convert char to integer
		if (key >= 0 && key < _sprites.size()) {
			auto it = _sprites.begin();
			for (int i = 0; i < key; i++) it++;
			_selected = it->first;
		}

		if (_selected == "head") {
			drawHead = true;
			drawHead1 = false;
			drawHead2 = false;

		}
		else if (_selected == "head1") {
			drawHead = false;
			drawHead1 = true;
			drawHead2 = false;
		}
		else if (_selected == "head2") {
			drawHead = false;
			drawHead1 = false;
			drawHead2 = true;
		}



		//std::cout << "selected: " << keyChar << " " << key << " " << _selected << std::endl;

		if (keyChar == GLFW_KEY_UP) {
			_sprites[_selected].rotation += 0.1;
		}
		else if (keyChar == GLFW_KEY_DOWN) {
			_sprites[_selected].rotation -= 0.1;

		}


	}



private:
	std::string _selected = "arm1";
	std::map<std::string, Sprite> _sprites;
	Skeleton _skeleton;
	bool drawHead = true;
	bool drawHead1 = false;
	bool drawHead2 = false;

};

int main(int argc, char** argv) {
	Unique viewer;
	viewer.run();
}
