#include "atkui/framework.h"
#include "atk/toolkit.h"

#include "agl/window.h"
#include "agl/mesh/plane.h"
#include "atkui/framework.h"

using namespace atk;
using glm::vec3;
using glm::vec2;
using glm::vec4;

class SkinTestMesh : public agl::TriangleMesh {
public:
    SkinTestMesh() {
        // compute verts,normals,indices here

        setIsDynamic(true);
    }

    // try to animate one vertex

    void update(float elapsedTime) {
        vec3 p = vec3(vertexData(POSITION, 0));
        setVertexData(POSITION, 0, vec4(0, 0, 0, 0));
        //setVertexData(POSITION, 0, vec4(-0.5, 0, sin(elapsedTime), 0));

    }

    void init() {


        std::vector<GLuint> indices;
        std::vector<GLfloat> points;
        std::vector<GLfloat> normals;

        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(0);
        indices.push_back(3);
        indices.push_back(2);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(4);
        indices.push_back(5);
        indices.push_back(7);
        indices.push_back(4);
        indices.push_back(7);
        indices.push_back(6);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(9);
        indices.push_back(6);
        indices.push_back(9);
        indices.push_back(8);


        points.push_back(-0.5);
        points.push_back(0);
        points.push_back(0);
        points.push_back(0.5);
        points.push_back(0);
        points.push_back(0);
        points.push_back(-0.5);
        points.push_back(0.5);
        points.push_back(0);
        points.push_back(0.5);
        points.push_back(0.5);
        points.push_back(0);
        points.push_back(-0.5);
        points.push_back(1);
        points.push_back(0);
        points.push_back(0.5);
        points.push_back(1);
        points.push_back(0);
        points.push_back(-0.5);
        points.push_back(1.5);
        points.push_back(0);
        points.push_back(0.5);
        points.push_back(1.5);
        points.push_back(0);
        points.push_back(-0.5);
        points.push_back(2);
        points.push_back(0);
        points.push_back(0.5);
        points.push_back(2);
        points.push_back(0);


        for (int i = 0; i < 10; i++) {
            normals.push_back(0);
            normals.push_back(0);
            normals.push_back(1);
        }



        initBuffers(&indices, &points, &normals);

    }


    vec3 position(const vec3& p, float t) {
        float angle = t;
        float frequency = 7.0;
        float amplitude = 0.05;

        float heightFn = (angle + frequency * p[0] * frequency * p[2]);
        float y = amplitude * sin(heightFn);
        return vec3(p.x, y, p.z);
    }

};


class LinearBlendSkinning : public atkui::Framework {
public:
	LinearBlendSkinning() : atkui::Framework(atkui::Perspective) {}
	virtual ~LinearBlendSkinning() {}

	virtual void setup() {
		lookAt(vec3(2), vec3(0));

        vec3 center = vec3(0, 0, 0);
        vec3 dim = vec3(5, 10, 5);
        setupPerspectiveScene(center, dim);

		Joint* root = new Joint("root");
		root->setLocalTranslation(vec3(0, 0, 0));
		skeleton.addJoint(root);

		Joint* joint1 = new Joint("joint1");
		joint1->setLocalTranslation(vec3(0, 1, 0));
		skeleton.addJoint(joint1, root);

		Joint* joint2 = new Joint("joint2");
		joint2->setLocalTranslation(vec3(0, 1, 0));
		skeleton.addJoint(joint2, joint1);

		skeleton.fk();

        _mesh.init();

	}

	virtual void draw() {


        skeleton.fk();
        setColor(vec3(0, 0, 1));
        for (unsigned int i = 0; i < skeleton.getNumJoints(); i++) {
            if (skeleton.getByID(i) == skeleton.getRoot()) {
                continue;
            }
            Joint* parent = skeleton.getByID(i)->getParent();
            if (i == skeleton.getNumJoints() - 1) {
                parent->setLocalRotation(glm::angleAxis<float>(sin(1.5f * elapsedTime() + i), vec3(1, 0, 0)));
            }
            Joint* child = skeleton.getByID(i);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 0.1);

        }
		

        renderer.push();
        setColor(vec3(0, 1, 0));
        

       
        renderer.mesh(_mesh);
        renderer.pop();

        _mesh.update(elapsedTime());

        drawFloor(200, 4, 1);
	} 

    /*void drawFloor() {


        renderer.beginShader("floor");
        renderer.setUniform("uFog.color", vec3(0.8));
        renderer.setUniform("uLargeBlockSize", 20);
        renderer.setUniform("uSmallBlockSize", 5);
        renderer.setUniform("uFog.minDist", 0.75f * 100);
        renderer.setUniform("uFog.maxDist", 100);
        renderer.push();
        renderer.scale(vec3(100, 1, 100));
        renderer.plane();
        renderer.pop();
        renderer.endShader();
    }*/




protected:
	Skeleton skeleton;
    SkinTestMesh _mesh = SkinTestMesh();

};

int main(int argc, char** argv)
{
	LinearBlendSkinning viewer;
	viewer.run();
}
