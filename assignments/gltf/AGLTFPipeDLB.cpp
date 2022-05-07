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

        // lookAt(vec3(300), vec3(0));



        lookAt(vec3(2), vec3(0));
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
        _origGeometry.load("../models/pipe3.glb");
        // _geometry.print(true);



    }

    virtual void scene() {


        _factor = 3.0 * sin(elapsedTime());
        _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 1, 0)));
        _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(3.14f / 1.2f, vec3(1, 0, 0)));
        _skeleton.fk();


        auto start = high_resolution_clock::now();


        // try to edit vertices
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

                    vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
                    pos[3] = 1;


                    dualquat newquat = dualquat(glm::angleAxis(0.0f, vec3(0, 0, 1)), vec3(0));

                    for (int i = 0; i < 4; i++) {

                        mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
                        mat3 rotInvMatrix = mat3(invMatrix[0][0], invMatrix[0][1], invMatrix[0][2],
                            invMatrix[1][0], invMatrix[1][1], invMatrix[1][2], invMatrix[2][0],
                            invMatrix[2][1], invMatrix[2][2]);
                        quat rotInvQuat = quat(rotInvMatrix);
                        vec3 rotInvTrans = vec3(invMatrix[3][0], invMatrix[3][1], invMatrix[3][2]);
                        dualquat invDQuat = dualquat(rotInvQuat, rotInvTrans);
                        std::string name = _geometry.getJointName(0, (int)joints[i]);
                        quat BoneRot = _skeleton.getByName(name)->getLocal2Global().r();
                        vec3 BoneTrans = _skeleton.getByName(name)->getLocal2Global().t();
                        dualquat Bone = dualquat(BoneRot, BoneTrans);

                        newquat = newquat + weights[i] * Bone * invDQuat;

                    }



                    dualquat newquatnorm = normalize(newquat);

                    vec4 newpos = newquatnorm * pos * inverse(newquatnorm);



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


        renderer.blendMode(agl::BLEND);
        setColor(vec4(0, 1, 0, 0.5));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0))

       // renderer.translate(vec3(2, 0, 0));
        //renderer.scale(vec3(20));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        ASkeletonDrawer drawer;
        drawer.setColor(vec3(1, 1, 0));
        drawer.setJointRadius(0.05);
        drawer.setScale(1);
        drawer.draw(renderer, _skeleton);

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





































/*#include <cmath>
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

        // lookAt(vec3(300), vec3(0));
        lookAt(vec3(2), vec3(0));
        vec3 center = vec3(0, 0, 0);
        vec3 dim = vec3(5, 10, 5);
        setupPerspectiveScene(center, dim);

        Joint* root = new Joint("Bone");
        Joint* joint1 = new Joint("Bone.001");
        Joint* joint2 = new Joint("Bone_End");
        root->setLocalTranslation(vec3(0, 0, 0));
        joint1->setLocalTranslation(vec3(0, 1, 0));
        joint2->setLocalTranslation(vec3(0, 1, 0));
        _skeleton.addJoint(root);
        _skeleton.addJoint(joint1, root);
        _skeleton.addJoint(joint2, joint1);

        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/pipe3.glb");
        _origGeometry.load("../models/pipe3.glb");
        _geometry.print(false);


        mat4 invMatrix1 = _geometry.getInverseBindMatrix(0, 0);
        mat3 rotInvMatrix1 = mat3(vec3(invMatrix1[0][0], invMatrix1[0][1], invMatrix1[0][2]),
                vec3(invMatrix1[1][0], invMatrix1[1][1], invMatrix1[1][2]), vec3(invMatrix1[2][0],
                   invMatrix1[2][1], invMatrix1[2][2]));


        quat rotInvQuat1 = quat(rotInvMatrix1);

        std::cout << rotInvQuat1 << std::endl;
        vec3 rotInvTrans1 = vec3(invMatrix1[0][3], invMatrix1[1][3], invMatrix1[2][3]);

        std::cout << rotInvTrans1 << std::endl;
        dualquat invBone0 = dualquat(rotInvQuat1, rotInvTrans1);
     


        mat4 invMatrix2 = _geometry.getInverseBindMatrix(0, 1);
        mat3 rotInvMatrix2 = mat3(vec3(invMatrix2[0][0], invMatrix2[1][0], invMatrix2[2][0]),
            vec3(invMatrix1[0][1], invMatrix1[1][1], invMatrix1[2][1]), vec3(invMatrix1[0][2],
                invMatrix1[1][2], invMatrix1[2][2]));

        std::cout << invMatrix2[1][3] << std::endl;

        quat rotInvQuat2 = quat(rotInvMatrix2);
        std::cout << rotInvQuat2 << std::endl;
        vec3 rotInvTrans2 = vec3(invMatrix2[3][0], invMatrix2[3][1], invMatrix2[3][2]);
        std::cout << rotInvTrans2 << std::endl;
        dualquat invBone1 = dualquat(rotInvQuat2, rotInvTrans2);


        invDuals.push_back(invBone0);
        invDuals.push_back(invBone1);

    }

    virtual void draw() {

        _factor = 2.0 * sin(elapsedTime());
        _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 0, 1)));
        _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0, 0, 1)));
        _skeleton.fk();


        // try to edit vertices
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

                    vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
                    pos[3] = 1;

                    //std::cout << glm::to_string(newquat) << weights[0] << " " << weights[1] << std::endl;

                    dualquat newquat = dualquat(glm::angleAxis(0.0f, vec3(0, 0, 0)), vec3(0));
                    for (int i = 0; i < 4; i++) {

                        //mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
                        //mat3 rotInvMatrix = mat3(vec3(invMatrix[0][0], invMatrix[1][0], invMatrix[2][0]),
                        //    vec3(invMatrix[0][1], invMatrix[1][1], invMatrix[2][1]), vec3(invMatrix[0][2],
                        //        invMatrix[1][2], invMatrix[2][2]));


                        //quat rotInvQuat = quat(rotInvMatrix);
                        //vec3 rotInvTrans = vec3(invMatrix[0][3], invMatrix[1][3], invMatrix[2][3]);
                        //dualquat invDQuat = dualquat(rotInvQuat, rotInvTrans);
                        dualquat invDQuat = invDuals[(int)joints[i]];

                        std::string name = _geometry.getJointName(0, (int)joints[i]);
                        //std::cout << glm::to_string(invDQuat) << std::endl;
                        //std::cout << rotInvTrans << std::endl;

                        quat BoneRot = _skeleton.getByName(name)->getLocal2Global().r();
                        vec3 BoneTrans = _skeleton.getByName(name)->getLocal2Global().t();
                        dualquat Bone = dualquat(BoneRot, BoneTrans);

                        newquat = newquat + weights[i] * (Bone * invDQuat);

                    }

                    dualquat newquatnorm = normalize(newquat);

                    vec4 newpos = newquatnorm * pos * inverse(newquatnorm);



                    // vec4 newpos = skinMatrix * pos;

                    _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);


                    //weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    //std::cout << weights << std::endl;

                }
            }
        }
        _geometry.update();

        renderer.blendMode(agl::BLEND);
        setColor(vec4(0, 1, 0, 0.5));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0))

        renderer.translate(vec3(2, 0, 0));
        //renderer.scale(vec3(20));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        ASkeletonDrawer drawer;
        drawer.setColor(vec3(1, 1, 0));
        drawer.setJointRadius(0.05);
        drawer.setScale(1);
        drawer.draw(renderer, _skeleton);

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
    std::vector<dualquat> invDuals;


};

int main(int argc, char** argv) {
    AGLTFSimple viewer;
    viewer.run();
} */

