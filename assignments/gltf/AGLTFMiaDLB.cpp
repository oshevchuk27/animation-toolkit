#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include <chrono>

using namespace std::chrono;
using namespace atk;
using namespace std;
using namespace glm;

class AGLTFSimple : public atkui::Framework {
public:
    AGLTFSimple() : atkui::Framework(atkui::Perspective) {
    }

    virtual void setup() {
        //loadMotion("../motions/Mia/gesture.bvh");
        loadMotion("../motions/Mia/mia_restpose.bvh");
        for (int i = 0; i < _skeleton.getNumJoints(); i++)
        {
            atk::Joint* joint = _skeleton.getByID(i);
            joint->setLocalTranslation(joint->getLocalTranslation());
        }

        _geometry.load("../models/Mia/Mia-v2.0.glb");
        _origGeometry.load("../models/Mia/Mia-v2.0.glb");
        //_geometry.print();


        for (int i = 0; i < _geometry.getNumSkinJoints(0); i++) {
            std::string name = _geometry.getJointName(0, i);
            Joint* joint = _skeleton.getByName(name);
            if (!joint) continue;

            mat4 M = inverse(_geometry.getInverseBindMatrix(0, i));
            if (i > 0) {
                mat4 PM = joint->getParent()->getLocal2Global().matrix();
                M = inverse(PM) * M;
            }
            vec3 t = vec3(M[3]);
            quat r = quat(M);

            joint->setLocalTranslation(t);
            joint->setLocalRotation(r);
            _skeleton.fk();
        }

        renderer.loadShader("skin", "../shaders/skin.vs", "../shaders/skin.fs");
        renderer.beginShader("skin");
        renderer.setUniform("Gamma", 0.8f);
        renderer.setUniform("Material.specular", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("Material.diffuse", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("Material.ambient", 0.1f, 0.1f, 0.1f);
        renderer.setUniform("Material.shininess", 80.0f);
        renderer.setUniform("Light.position", 100.0f, 100.0f, 100.0f, 1.0f);
        renderer.setUniform("Light.color", 1.0f, 1.0f, 1.0f);
        renderer.setUniform("Fog.enabled", false);
        renderer.setUniform("HasUV", false);

        renderer.setUniform("MainTexture.enabled", false);
        renderer.setUniform("MainTexture.offset", vec2(0.0f));
        renderer.setUniform("MainTexture.tile", vec2(1.0f));

        renderer.setUniform("DetailTexture.enabled", false);
        renderer.setUniform("DetailTexture.offset", vec2(0.0f));
        renderer.setUniform("DetailTexture.tile", vec2(1.0f));
    }

    virtual void scene() {
        _motion.update(_skeleton, elapsedTime());



        _skeleton.getByName("LeftArm")->setLocalRotation(glm::angleAxis(3.14f / 2, vec3(0, 0, 1)));

        _skeleton.fk(); // computes local2global transforms


        auto start = high_resolution_clock::now();



        int nummesh = _geometry.getNumMeshes();

        //std::cout << nummesh << std::endl;

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



                    quat real(0, 0, 0, 0);
                    quat dual(0, 0, 0, 0);
                    for (int i = 0; i < 4; i++) {
                        std::string name = _geometry.getJointName(0, (int)joints[i]);
                        Joint* joint = _skeleton.getByName(name);
                        if (!joint) std::cout << "Error: cannot find name! " << name << std::endl;

                        mat4 invMatrix = _geometry.getInverseBindMatrix(0, joints[i]);
                        Transform local2global = joint->getLocal2Global();

                        dualquat im = dualquat(quat(invMatrix), vec3(invMatrix[3]));
                        dualquat lg = dualquat(local2global.r(), local2global.t());
                        dualquat dq = lg * im;

                        real = real + weights[i] * dq.real;
                        dual = dual + weights[i] * dq.dual;
                    }

                    dualquat newquatnorm = normalize(dualquat(real, dual));
                    vec4 newpos = newquatnorm * pos; // HERE!!!!

                    /*dualquat newquat = dualquat(glm::angleAxis(0.0f, vec3(0, 0, 1)), vec3(0));

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

                    vec4 newpos = newquatnorm * pos * inverse(newquatnorm);*/

                    _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);

                    //weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    //std::cout << weights << std::endl;
                }
            }
        }


        auto stop = high_resolution_clock::now();


        auto duration = duration_cast<microseconds>(stop - start);
        //std::cout << "Time taken by frame "<< count << "is " << duration.count()/1000.0f<< "milliseconds." << std::endl;

        /*if (count < 1000) {
            std::cout << "the count is " << count << endl;
            sum += duration.count() / 1000.0f;
            std::cout << "the sum is " << sum << std::endl;
            std::cout << "the average is " << sum / 1000 << std::endl;

        }*/



        count++;

        renderer.beginShader("skin");
        setColor(vec4(1));
        renderer.push();
        renderer.rotate(-3.14, vec3(1, 0, 0));
        renderer.scale(vec3(100));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();
        renderer.endShader();
    }

    virtual void keyPress(unsigned char key, int specialKey, int x, int y) {
    }

    virtual void loadDir(const std::string& dir) {
    }

    virtual void loadMotion(const std::string& filename) {
        atk::BVHReader reader;
        reader.load(filename, _skeleton, _motion);
    }

protected:
    Skeleton _skeleton;
    Motion _motion;
    AGLTFGeometry _geometry;
    AGLTFGeometry _origGeometry;
    int count = 0;
    float sum = 0.0f;

};

int main(int argc, char** argv)
{
    AGLTFSimple viewer;
    //viewer.loadMotion("../motions/Mia/idle.bvh");
    viewer.run();
}
