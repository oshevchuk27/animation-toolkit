#include "atkui/framework.h"
#include "atk/toolkit.h"
#include <chrono>

#include "agl/window.h"
#include "agl/mesh/plane.h"
#include "atkui/framework.h"

using namespace atk;
using namespace std::chrono;
using glm::vec3;
using glm::vec2;
using glm::vec4;

class SkinTestMesh : public agl::TriangleMesh {
public:
    SkinTestMesh() {

        setIsDynamic(true);
        
    }
    int count = 0;
    std::vector<float> times;


    void update(float elapsedTime, Skeleton skeleton) {


        auto start = high_resolution_clock::now();
   
        
        for (int i = 0; i < numVertices(); i++) {
            
            vec3 p = vec3(points[3 * i], points[3 * i + 1], points[3 * i + 2]);

            Transform transJoint1 = skeleton.getByName("root")->getLocal2Global() *
                restBone1.inverse();


            Transform transJoint2 = skeleton.getByName("joint1")->getLocal2Global() *
                restBone2.inverse();



            vec3 newp = weights[i * 2] * transJoint1.transformPoint(p) + 
                weights[i * 2 + 1] * transJoint2.transformPoint(p);
            
            setVertexData(POSITION, i, vec4(newp, 0));

        }


        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);
        //std::cout << "Time taken by frame "<< count << "is " << duration.count()/1000.0f<< "milliseconds." << std::endl;

        if (count < 1000) {
            sum += duration.count()/1000.0f;
            std::cout << "the sum is " << sum << std::endl;
            std::cout << "the average is " << sum / 1000 << std::endl;

        }


        
        count++;
        

    }

    void init() {


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

        weights.push_back(1);
        weights.push_back(0);
        weights.push_back(1);
        weights.push_back(0);
        weights.push_back(0.75);
        weights.push_back(0.25);
        weights.push_back(0.75);
        weights.push_back(0.25);
        weights.push_back(0.5);
        weights.push_back(0.5);
        weights.push_back(0.5);
        weights.push_back(0.5);
        weights.push_back(0.25);
        weights.push_back(0.75);
        weights.push_back(0.25);
        weights.push_back(0.75);
        weights.push_back(0);
        weights.push_back(1);
        weights.push_back(0);
        weights.push_back(1);




        initBuffers(&indices, &points, &normals);

    }

    void init(const Skeleton& skeleton) {


        restBone1 = skeleton.getByName("root")->getLocal2Global();

        restBone2 = skeleton.getByName("joint1")->getLocal2Global();

        std::cout << restBone2 << std::endl;

        init();

       

        
        

    }

    public:
        std::vector<GLuint> indices;
        std::vector<GLfloat> points;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> weights;
        Transform restBone1;
        Transform restBone2;
        float sum = 0.0f;
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



        _mesh.init(skeleton);

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
                //parent->setLocalRotation(glm::angleAxis<float>(sin(1.5f * elapsedTime() + i), vec3(0, 0, 1)));
                parent->setLocalRotation(glm::angleAxis<float>(-3.14f/3, vec3(0, 0, 1)));
                //parent->setLocalRotation(glm::angleAxis<float>(0.1, vec3(0, 0, 1)));
                //parent->setLocalTranslation(vec3(1, 2, 0));
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

        _mesh.update(elapsedTime(), skeleton);

        drawFloor(200, 4, 1);
	} 

   

public:
	Skeleton skeleton;
    SkinTestMesh _mesh = SkinTestMesh();



};

int main(int argc, char** argv)
{
	LinearBlendSkinning viewer;
	viewer.run();

   
}
