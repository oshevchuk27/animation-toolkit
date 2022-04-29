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

        Joint* root = new Joint("Bone");
        Joint* joint1 = new Joint("Bone.001");
        Joint* joint2 = new Joint("Bone.002");
        Joint* joint3 = new Joint("Bone.003");
        Joint* joint4 = new Joint("Bone.004");
        Joint* joint5 = new Joint("Bone.005");
        Joint* joint6 = new Joint("Bone.006");
        Joint* joint7 = new Joint("Bone.007");
        Joint* joint8 = new Joint("Bone.008");
        Joint* joint9 = new Joint("Bone.009");

        root->setLocalTranslation(vec3(0, 0, 0));
        joint1->setLocalTranslation(vec3(0, 50, 0));
        joint2->setLocalTranslation(vec3(0, 50, 0));
        joint3->setLocalTranslation(vec3(0, 50, 0));
        joint4->setLocalTranslation(vec3(0, 50, 0));
        joint5->setLocalTranslation(vec3(0, 50, 0));
        joint6->setLocalTranslation(vec3(0, 50, 0));
        joint7->setLocalTranslation(vec3(0, 50, 0));
        joint8->setLocalTranslation(vec3(0, 50, 0));
        joint9->setLocalTranslation(vec3(0, 50, 0));



        _skeleton.addJoint(root);
        _skeleton.addJoint(joint1, root);
        _skeleton.addJoint(joint2, joint1);
        _skeleton.addJoint(joint3, joint2);
        _skeleton.addJoint(joint4, joint3);
        _skeleton.addJoint(joint5, joint4);
        _skeleton.addJoint(joint6, joint5);
        _skeleton.addJoint(joint7, joint6);
        _skeleton.addJoint(joint8, joint7);
        _skeleton.addJoint(joint9, joint8);

        _skeleton.fk();


        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/thread.glb");
        _origGeometry.load("../models/thread.glb"); // need to keep original vertices
        //_geometry.print();


        mat4 invMatrix = _geometry.getInverseBindMatrix(0, 0);
        std::cout << "INV 0" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 1);
        std::cout << "INV 1" << invMatrix << std::endl;


        invMatrix = _geometry.getInverseBindMatrix(0, 2);
        std::cout << "INV 2" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 3);
        std::cout << "INV 3" << invMatrix << std::endl;


        invMatrix = _geometry.getInverseBindMatrix(0, 4);
        std::cout << "INV 4" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 5);
        std::cout << "INV 5" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 6);
        std::cout << "INV 6" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 7);
        std::cout << "INV 7" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 8);
        std::cout << "INV 8" << invMatrix << std::endl;

        invMatrix = _geometry.getInverseBindMatrix(0, 9);
        std::cout << "INV 9" << invMatrix << std::endl;


    }

    virtual void scene() {

        // try to edit vertices
       
        _skeleton.fk(); // computes local2global transforms
        setColor(vec3(0, 1, 0));

        // todo: loop over all joints and draw
        for (unsigned int i = 0; i < _skeleton.getNumJoints(); i++) {

            if (_skeleton.getByID(i) == _skeleton.getRoot()) {
                continue;
            }
            Joint* parent = _skeleton.getByID(i)->getParent();
            parent->setLocalRotation(glm::angleAxis<float>(sin(1.5f * elapsedTime() + i), vec3(0, 0, 1)));
            Joint* child = _skeleton.getByID(i);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 3);


        }


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


                    //std::cout << joints << std::endl;
                    //std::cout << "matrix" << _geometry.getInverseBindMatrix(0, joints[0]) << endl;
                    // Need to use original vertices for calculations!
                    vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
                    pos[3] = 1; // homogeneous coordinate

                    vec4 newpos = vec4(0);
                    for (int i = 0; i < 4; i++) {
                        mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
                        std::string name = _geometry.getJointName(0, (int)joints[i]);
                        atk::Joint* joint = _skeleton.getByName(name);
                        if (!joint) std::cout << "Error: cannot find name! " << name << std::endl;

                        mat4 local2global = joint->getLocal2Global().matrix();
                        newpos += weights[i] * local2global * invMatrix * pos; 
                    }
                     
                    //setColor(vec3(1));
                    //drawSphere(newpos, 10);


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
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));
        //renderer.rotate(-1.5708, vec3(0, 0, 1));
        //renderer.translate(vec3(0, 70, 0));
        //renderer.translate(vec3(70, 0, 0));
        //renderer.translate(vec3(-50, 0, 0));
         //renderer.scale(vec3(2, 1, 2));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        ASkeletonDrawer drawer;
       
        //drawer.setJointRadius(0.05);
        //drawer.setScale(100);
        drawer.draw(renderer,_skeleton);
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



