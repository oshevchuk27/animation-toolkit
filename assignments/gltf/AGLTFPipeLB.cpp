#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"
#include <chrono>

using namespace atk;
using namespace std;
using namespace glm;
using namespace std::chrono;

using glm::dualquat;
using glm::quat;

class AGLTFSimple : public atkui::Framework
{
public:
    AGLTFSimple() : atkui::Framework(atkui::Perspective) {
    }

    virtual void setup()
    {
        //lookAt(vec3(250), vec3(0));
		  //lookAt(vec3(0), vec3(0), vec3(0, -50, 0));
        //lookAt(vec3(5), vec3(0));
        vec3 center = vec3(0, 0, 0);
        vec3 dim = vec3(25, 35, 25);
        setupPerspectiveScene(center, dim);

        Joint* root = new Joint("Bone");
        Joint* joint1 = new Joint("Bone.001");
        Joint* joint2 = new Joint("Bone_End");
        root->setLocalTranslation(vec3(0, 0, 0));
        joint1->setLocalTranslation(vec3(0, 7.9813, 0));
        joint2->setLocalTranslation(vec3(0, (12.975 - 7.9813), 0));
        _skeleton.addJoint(root);
        _skeleton.addJoint(joint1, root);
        _skeleton.addJoint(joint2, joint1);

        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/pipe3.glb");
        _origGeometry.load("../models/pipe3.glb"); // need to keep original vertices
        _geometry.print(false);

        mat4 invMatrix = _geometry.getInverseBindMatrix(0, 0);
        std::cout << "INV 0" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 1);
        std::cout << "INV 1" << invMatrix << std::endl;
      
    }

    virtual void draw() {

        // try to edit vertices
        _factor = 3 * sin(elapsedTime());
        _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 1, 0)));
        _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0, 1, 0)));
        _skeleton.fk();



        auto start = high_resolution_clock::now();


        int nummesh = _geometry.getNumMeshes();

        for (int meshid = 0; meshid < nummesh; meshid++) {
            int numprims = _geometry.getNumPrimitives(meshid);

            for (int primid = 0; primid < numprims; primid++) {
                int numverts = _geometry.getNumVertices(meshid, primid, "POSITION");

                for (int vid = 0; vid < numverts; vid++) {

                    vec4 weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    vec4 joints = _geometry.getVertexData(meshid, primid, "JOINTS_0", vid);

                    //std::cout << joints << std::endl;
                    //std::cout << "matrix" << _geometry.getInverseBindMatrix(0, joints[0]) << endl;
                    // Need to use original vertices for calculations!
                    vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
                    pos[3] = 1; // homogeneous coordinate

                    vec4 newpos = vec4(0);
                    for (int i = 0; i < 4; i++) {
                      mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
                      std::string name = _geometry.getJointName(0, (int) joints[i]);
                      atk::Joint* joint = _skeleton.getByName(name); 
                      if (!joint) std::cout << "Error: cannot find name! " << name << std::endl;
                      
                      mat4 local2global = joint->getLocal2Global().matrix();
                      newpos += weights[i] * local2global * invMatrix * pos;
                    }


                   // vec4 newpos = skinMatrix * pos;
                    _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);

                    //weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    //std::cout << weights << std::endl;
                }
            }
        }

        _geometry.update();

        auto stop = high_resolution_clock::now();


        auto duration = duration_cast<microseconds>(stop - start);
        //std::cout << "Time taken by frame "<< count << "is " << duration.count()/1000.0f<< "milliseconds." << std::endl;

        if (count < 1000) {
            std::cout << "the count is " << count << endl;
            sum += duration.count() / 1000.0f;
            std::cout << "the sum is " << sum << std::endl;
            std::cout << "the average is " << sum / 1000 << std::endl;

        }



        count++;


        //setColor(vec3(0, 1, 0));
        renderer.blendMode(agl::BLEND);
        setColor(vec4(0, 1, 0, 0.5));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));
  
        renderer.translate(vec3(2, 0, 0));
        //renderer.scale(vec3(20));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        ASkeletonDrawer drawer;
        //drawer.setJointRadius(0.05);
        //drawer.setScale(100);
        drawer.setColor(vec3(1,1,0));
        drawer.setJointRadius(0.05);
        drawer.setScale(1);
        drawer.draw(renderer, _skeleton);

        /*for (int i = 0; i < _skeleton.getNumJoints(); i++) {
            Joint* joint = _skeleton.getByID(i);
            if (joint->getParent() == 0) continue;

            vec3 p1 = joint->getGlobalTranslation();
            vec3 p2 = joint->getParent()->getGlobalTranslation();
            drawEllipsoid(p1, p2, 5);
        }*/
        drawFloor(200, 4, 1);
    }

private:

    float _factor = 1;
    Skeleton _skeleton;
    AGLTFGeometry _geometry;
    AGLTFGeometry _origGeometry;

    Transform  RestBone1Transform;
    Transform  RestBone2Transform;

    quat RestBone1Rot;
    vec3 RestBone1Trans;
    quat RestBone2Rot;
    vec3 RestBone2Trans;
    int count = 0;
    float sum = 0.0f;

};

int main(int argc, char** argv) {
    AGLTFSimple viewer;
    viewer.run();
}



                  
