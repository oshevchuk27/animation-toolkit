#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <stdio.h>
#include <string.h>

using namespace atk;
using namespace atkui;
using namespace glm;

class Unique : public atkui::Framework
{
public:
	Unique() : atkui::Framework(atkui::Perspective) {}
	virtual ~Unique() {}

	void loadMotion1(const std::string& filename)
	{
		BVHReader reader;
		reader.load(filename, skeleton_, motion1_);
	}

	void loadMotion2(const std::string& filename)
	{
		BVHReader reader;
		reader.load(filename, skeleton_, motion2_);
	}


	void setup() {



		Joint* root = new Joint("root");
		spiral_.addJoint(root);

		Joint* jf_1 = new Joint("jf_1");
		spiral_.addJoint(jf_1, root);

		spiral_.fk();

	}

	void crossfade(int numBlendFrames)
	{
		assert(motion1_.getNumKeys() > 0);
		assert(motion2_.getNumKeys() > 0);
		assert(numBlendFrames > 0);
		assert(numBlendFrames <= motion1_.getNumKeys());
		assert(numBlendFrames <= motion2_.getNumKeys());

		blend_.setFramerate(motion1_.getFramerate());
		int start1 = motion1_.getNumKeys() - numBlendFrames - 1;
		int start2 = 0;

		reorient();

		for (int i = 0; i < start1; i++) {
			blend_.appendKey(motion1_.getKey(i));
		}

		motionsBlend(numBlendFrames, start1, start2);

		for (int j = numBlendFrames; j < motion2_.getNumKeys(); j++) {
			blend_.appendKey(motion2_.getKey(j));
		}

	}

	void motionsBlend(int numBlendFrames, int start1, int start2) {

		for (float k = 0; k <= numBlendFrames; k++) {
			float alpha = (float)k / numBlendFrames;
			Pose pose1 = motion1_.getKey(start1 + k);
			Pose pose2 = motion2_.getKey(start2 + k);
			Pose newPose = Pose::Lerp(pose1, pose2, alpha);
			blend_.appendKey(newPose);


		}

	}

	void reorient() {


		Transform T1 = Transform(motion2_.getKey(0).jointRots[skeleton_.getRoot()->getID()], motion2_.getKey(0).rootPos);
		Transform T_desired = Transform(motion1_.getKey(motion1_.getNumKeys() - 1).jointRots[skeleton_.getRoot()->getID()], motion1_.getKey(motion1_.getNumKeys() - 1).rootPos);
		Transform T_offset = T_desired * T1.inverse();
		for (int i = 0; i < motion2_.getNumKeys(); i++) {
			Pose pose = motion2_.getKey(i);
			Transform T_orig = Transform(pose.jointRots[skeleton_.getRoot()->getID()], pose.rootPos);
			Transform T_new = T_offset * T_orig;
			pose.rootPos = T_new.t();
			pose.jointRots[skeleton_.getRoot()->getID()] = T_new.r();
			motion2_.editKey(i, pose);

		}
	}

	void save(const std::string& filename)
	{
		BVHWriter writer;
		writer.save(filename, skeleton_, blend_);
	}

	void scene()
	{
		blend_.update(skeleton_, elapsedTime());
		drawer_.color = vec3(randValue(), randValue(), randValue());
		drawer_.draw(skeleton_, *this);



		for (int j = 0; j < spiral_.getNumJoints(); j++) {

			if (spiral_.getByID(j) == spiral_.getRoot()) {
				continue;
			}

			Joint* parent = spiral_.getByID(j)->getParent();
			Joint* child = spiral_.getByID(j);



			for (int i = 0; i < 5; i++) {
				drawSpiral(i);
			}


		}
	}

	void drawSpiral(int i) {

		float value = randSpiralValue();
		for (int k = 0; k <= 55; k++) {
			float r = k;
			float x = r * cos(value * k);
			float y = r * sin(value * k);
			setColor(pallet[i]);
			drawSphere(vec3(x, y, 0) + getPosition(i), 15);


		}
	}

	vec3 getPosition(int i) {

		std::vector<vec3> coordinates;
		coordinates.push_back(vec3(0, 300, -100)); // blue
		coordinates.push_back(vec3(150, 250, 0)); // light blue
		coordinates.push_back(vec3(-150, 250, 0)); // pink
		coordinates.push_back(vec3(-250, 200, 100)); // red
		coordinates.push_back(vec3(250, 200, 100)); // yellow


		return coordinates[i];
	}




	float randSpiralValue() {
		float randValue = 0.5 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (2 - 0.5)));
		return randValue;

	}

	float randValue() {
		float randValue = 0 + static_cast <float> (rand()) /
			(static_cast <float> (RAND_MAX / (1 + 1)));
		return randValue;
	}

private:
	Skeleton skeleton_;
	SkeletonDrawer drawer_;
	Motion motion1_;
	Motion motion2_;
	Motion blend_;
	Skeleton spiral_;
	std::vector<vec3> pallet =
	{
	   vec3(0,165,227) / 255.0f,
	   vec3(141,215,191) / 255.0f,
	   vec3(255,150,197) / 255.0f,
	   vec3(255,87,104) / 255.0f,
	   vec3(255,162,58) / 255.0f
	};
};

std::string PruneName(const std::string& name)
{
	int idx1 = name.find_last_of("/");
	int idx2 = name.find_last_of(".");
	return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char** argv)
{
	std::string motion1 = "../motions/88/88_07.bvh";
	std::string motion2 = "../motions/88/88_01.bvh";
	int numFrames = 10;

	try
	{
		for (int i = 0; i < argc; i++)
		{
			if (strncmp(argv[i], "-m1", 3) == 0)
				motion1 = argv[i + 1];
			else if (strncmp(argv[i], "-m2", 3) == 0)
				motion2 = argv[i + 1];
			else if (strncmp(argv[i], "-nframes", 7) == 0)
				numFrames = std::stoi(argv[i + 1]);
		}
	}
	catch (const std::exception& e)
	{
		std::cout << "usage: crossfade -m1 <path> -m2 <path> -nframes <num frames>\n";
	}

	Unique viewer;
	viewer.loadMotion1(motion1);
	viewer.loadMotion2(motion2);
	viewer.crossfade(numFrames);

	std::string saveName = "blend-" + PruneName(motion1) + PruneName(motion2);
	saveName += "-" + std::to_string(numFrames) + ".bvh";
	viewer.save(saveName);

	viewer.run();
	return 0;
}


