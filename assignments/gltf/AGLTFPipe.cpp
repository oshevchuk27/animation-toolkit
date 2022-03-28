#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"

using namespace atk;
using namespace std;
using namespace glm;
using glm::dualquat;
using glm::quat;

class AGLTFSimple : public atkui::Framework
{
public:
    AGLTFSimple() : atkui::Framework(atkui::Perspective) {
    }

    virtual void setup()
    {
        Joint* root = new Joint("joint0");
        Joint* joint1 = new Joint("joint1");
        Joint* joint2 = new Joint("joint2");
        root->setLocalTranslation(vec3(0, 0, 0));
        joint1->setLocalTranslation(vec3(0, 2.20, 0));
        joint2->setLocalTranslation(vec3(0, 2.20, 0));
        _skeleton.addJoint(root);
        _skeleton.addJoint(joint1, root);
        _skeleton.addJoint(joint2, joint1);

        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/pipe.glb");
       // _geometry.print(true);

        RestBone1Transform = _skeleton.getByName("joint0")->getLocal2Global();
        RestBone1Rot = RestBone1Transform.r();
        RestBone1Trans = RestBone1Transform.t();


        RestBone2Transform = _skeleton.getByName("joint1")->getLocal2Global();

        RestBone2Rot = RestBone2Transform.r();
        RestBone2Trans = RestBone2Transform.t();

        //renderer.loadShader("skin", "../shaders/skin.vs", "../shaders/skin.fs");
    }

    virtual void scene() {

        // try to edit vertices
        _factor = 0.7 * sin(elapsedTime());
        _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 0, 1)));
        _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0, 0, 1)));
        _skeleton.fk();


        int nummesh = _geometry.getNumMeshes();

        for (int meshid = 0; meshid < nummesh; meshid++) {
            int numprims = _geometry.getNumPrimitives(meshid);

           
            for (int primid = 0; primid < numprims; primid++) {
                int numverts = _geometry.getNumVertices(meshid, primid, "POSITION");

                for (int vid = 0; vid < numverts; vid++) {

                   

                    //vec4 weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);

                    //vec4 joints = _geometry.getVertexData(meshid, primid, "JOINTS_0", vid);

                    

                    /*mat4 skinMatrix = weights[0] * _geometry.getInverseBindMatrix(0, joints[0]) +
                        weights[1] * _geometry.getInverseBindMatrix(1, joints[1]) +
                        weights[2] * _geometry.getInverseBindMatrix(2, joints[2]) +
                        weights[3] * _geometry.getInverseBindMatrix(3, joints[3]);

                    vec4 newpos = skinMatrix * pos;*/

                   /*quat Bone1Rot = _skeleton.getByName("joint0")->getLocal2Global().r();
                    vec3 Bone1Trans = _skeleton.getByName("joint0")->getLocal2Global().t();

                    dualquat Bone1 = dualquat(Bone1Rot, Bone1Trans);


                    quat Bone2Rot = _skeleton.getByName("joint1")->getLocal2Global().r();
                    vec3 Bone2Trans = _skeleton.getByName("joint1")->getLocal2Global().t();

                    dualquat Bone2 = dualquat(Bone2Rot, Bone2Trans);




                    dualquat RestBone1 = dualquat(RestBone1Rot, RestBone1Trans);

                    dualquat RestBone2 = dualquat(RestBone2Rot, RestBone2Trans);


                    dualquat newquat = normalize(weights[0] * Bone1 * inverse(RestBone1) +
                        weights[1] * Bone2 * inverse(RestBone2));

                    vec3 newpos = newquat * pos


                    _geometry.setVertexData(meshid, primid, "POSITION", vid, vec4(newpos, 0));*/


                }


            }


        }




        //renderer.beginShader("skin");
        //setColor(vec3(0, 1, 0));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));
        renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer);
        renderer.pop();
        //renderer.endShader();

        ASkeletonDrawer drawer;
        drawer.setJointRadius(0.05);
        drawer.setScale(100);
        drawer.draw(renderer, _skeleton);
    }

private:

    float _factor = 1;
    Skeleton _skeleton;
    AGLTFGeometry _geometry;

    Transform  RestBone1Transform;
    Transform  RestBone2Transform;

    quat RestBone1Rot;
    vec3 RestBone1Trans;
    quat RestBone2Rot;
    vec3 RestBone2Trans;
};

int main(int argc, char** argv) {
    AGLTFSimple viewer;
    viewer.run();
}


