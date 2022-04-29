#include <cmath>
#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"
#include "ASkeletonDrawer.h"
#include "atkmath/constants.h"

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
       /*loadMotion("../motions/hand.bvh");

        for (int i = 0; i < _skeleton.getNumJoints(); i++)
        {
            atk::Joint* joint = _skeleton.getByID(i);
            joint->setLocalTranslation(joint->getLocalTranslation());
        }*/


        lookAt(vec3(50), vec3(0));



        Joint* root = new Joint("Bone.002");
        Joint* joint1 = new Joint("Bone.003");
        Joint* joint2 = new Joint("Bone");
        Joint* joint3 = new Joint("Bone.001");

        root->setLocalTranslation(vec3(0, 0, 0));
        float rootAngle = (float)(-25.0f * (atkmath::PI / 180));
        root->setLocalRotation(glm::angleAxis(rootAngle, vec3(1, 0, 0)));
        joint1->setLocalTranslation(vec3(0, 50, 0));
        float joint1Angle = (float)(31 * (atkmath::PI / 180));
        joint1->setLocalRotation(glm::angleAxis(joint1Angle, vec3(1, 0, 0)));
        joint2->setLocalTranslation(vec3(0, 50, 0));
        float joint2Angle = (float)(15 * (atkmath::PI / 180));
        joint2->setLocalRotation(glm::angleAxis(joint2Angle, vec3(1, 0, 0)));
        joint3->setLocalTranslation(vec3(0, 50, 0));
        float joint3Angle = (float)(7.4 * (atkmath::PI / 180));
        joint3->setLocalRotation(glm::angleAxis(joint3Angle, vec3(1, 0, 0)));






        _skeleton.addJoint(root);
        _skeleton.addJoint(joint1, root);
        _skeleton.addJoint(joint2, joint1);
        _skeleton.addJoint(joint3, joint2);


        Joint* joint4 = new Joint("Bone.004");
        Joint* joint5 = new Joint("Bone.005");
        Joint* joint6 = new Joint("Bone.006");
        Joint* joint7 = new Joint("Bone.007");

        joint4->setLocalTranslation(vec3(0, 50, 0));
        float joint4Angle = (float)(-14 * (atkmath::PI / 180));
        joint4->setLocalRotation(glm::angleAxis(joint4Angle, vec3(1, 0, 0)));

        joint5->setLocalTranslation(vec3(0, 50, 0));
        float joint5Angle = (float)(-30 * (atkmath::PI / 180));
        joint5->setLocalRotation(glm::angleAxis(joint5Angle, vec3(1, 0, 0)));

        joint6->setLocalTranslation(vec3(0, 50, 0));
        float joint6Angle = (float)(-29 * (atkmath::PI / 180));
        joint6->setLocalRotation(glm::angleAxis(joint6Angle, vec3(1, 0, 0)));


        joint7->setLocalTranslation(vec3(0, 50, 0));
        float joint7Angle = (float)(-28 * (atkmath::PI / 180));
        joint7->setLocalRotation(glm::angleAxis(joint7Angle, vec3(1, 0, 0)));



        _skeleton.addJoint(joint4, root);
        _skeleton.addJoint(joint5, joint4);
        _skeleton.addJoint(joint6, joint5);
        _skeleton.addJoint(joint7, joint6);


        Joint* joint8 = new Joint("Bone.008");
        Joint* joint9 = new Joint("Bone.009");
        Joint* joint10 = new Joint("Bone.010");
        Joint* joint11 = new Joint("Bone.011");

        joint8->setLocalTranslation(vec3(0, 50, 0));
        float joint8Angle = (float)(-30 * (atkmath::PI / 180));
        joint8->setLocalRotation(glm::angleAxis(joint8Angle, vec3(1, 0, 0)));

        joint9->setLocalTranslation(vec3(0, 50, 0));
        float joint9Angle = (float)(-22 * (atkmath::PI / 180));
        joint9->setLocalRotation(glm::angleAxis(joint9Angle, vec3(1, 0, 0)));

        joint10->setLocalTranslation(vec3(0, 50, 0));
        float joint10Angle = (float)(-22 * (atkmath::PI / 180));
        joint10->setLocalRotation(glm::angleAxis(joint10Angle, vec3(1, 0, 0)));

        joint11->setLocalTranslation(vec3(0, 50, 0));
        float joint11Angle = (float)(-27 * (atkmath::PI / 180));
        joint11->setLocalRotation(glm::angleAxis(joint11Angle, vec3(1, 0, 0)));


        _skeleton.addJoint(joint8, root);
        _skeleton.addJoint(joint9, joint8);
        _skeleton.addJoint(joint10, joint9);
        _skeleton.addJoint(joint11, joint10);

        Joint* joint12 = new Joint("Bone.012");
        Joint* joint13 = new Joint("Bone.013");
        Joint* joint14 = new Joint("Bone.014");
        Joint* joint15 = new Joint("Bone.015");

        joint12->setLocalTranslation(vec3(0, 50, 0));
        float joint12Angle = (float)(-48 * (atkmath::PI / 180));
        joint12->setLocalRotation(glm::angleAxis(joint12Angle, vec3(1, 0, 0)));

        joint13->setLocalTranslation(vec3(0, 50, 0));
        float joint13Angle = (float)(-32 * (atkmath::PI / 180));
        joint13->setLocalRotation(glm::angleAxis(joint13Angle, vec3(1, 0, 0)));

        joint14->setLocalTranslation(vec3(0, 50, 0));
        float joint14Angle = (float)(-30 * (atkmath::PI / 180));
        joint14->setLocalRotation(glm::angleAxis(joint14Angle, vec3(1, 0, 0)));

        joint15->setLocalTranslation(vec3(0, 50, 0));

        float joint15Angle = (float)(-30 * (atkmath::PI / 180));
        joint15->setLocalRotation(glm::angleAxis(joint15Angle, vec3(1, 0, 0)));


        _skeleton.addJoint(joint12, root);
        _skeleton.addJoint(joint13, joint12);
        _skeleton.addJoint(joint14, joint13);
        _skeleton.addJoint(joint15, joint14);

        Joint* joint16 = new Joint("Bone.016");
        Joint* joint17 = new Joint("Bone.017");
        Joint* joint18 = new Joint("Bone.018");
        Joint* joint19 = new Joint("Bone.019");

        joint16->setLocalTranslation(vec3(0, 50, 0));
        float joint16Angle = (float)(-70 * (atkmath::PI / 180));
        joint16->setLocalRotation(glm::angleAxis(joint16Angle, vec3(1, 0, 0)));

        joint17->setLocalTranslation(vec3(0, 50, 0));
        float joint17Angle = (float)(-44 * (atkmath::PI / 180));
        joint17->setLocalRotation(glm::angleAxis(joint17Angle, vec3(1, 0, 0)));

        joint18->setLocalTranslation(vec3(0, 50, 0));
        float joint18Angle = (float)(-38 * (atkmath::PI / 180));
        joint18->setLocalRotation(glm::angleAxis(joint18Angle, vec3(1, 0, 0)));

        joint19->setLocalTranslation(vec3(0, 50, 0));

        float joint19Angle = (float)(-45 * (atkmath::PI / 180));
        joint19->setLocalRotation(glm::angleAxis(joint19Angle, vec3(1, 0, 0)));


        _skeleton.addJoint(joint16, root);
        _skeleton.addJoint(joint17, joint16);
        _skeleton.addJoint(joint18, joint17);
        _skeleton.addJoint(joint19, joint18);


        _skeleton.fk();

        
        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/hand1.glb");
        _origGeometry.load("../models/hand1.glb"); // need to keep original vertices
        _geometry.print(false);


        /*renderer.loadShader("skin", "../shaders/skin.vs", "../shaders/skin.fs");
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

        */
    }

    virtual void scene() {


        //_motion.update(_skeleton, elapsedTime());

        // try to edit vertices
      //  _factor = 2 * sin(elapsedTime());
        //_skeleton.getByName("Bone.001")->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 0, 1)));
        //_skeleton.getByName("Bone.001")->setLocalRotation(glm::angleAxis(_factor, vec3(0, 0, 1)));
      // _skeleton.fk();


       /*int nummesh = _geometry.getNumMeshes();

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
                        std::string name = _geometry.getJointName(0, (int)joints[i]);
                        atk::Joint* joint = _skeleton.getByName(name);
                        if (!joint) std::cout << "Error: cannot find name! " << name << std::endl;

                        mat4 local2global = joint->getLocal2Global().matrix();
                        newpos += weights[i] * local2global * invMatrix * pos;
                    } */

                    //setColor(vec3(1));
                    //drawSphere(newpos, 10);


               // vec4 newpos = skinMatrix * pos;
                   // _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);

                    //weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    //std::cout << weights << std::endl;
     /*}
            }
        } */

       // _geometry.update();

        //renderer.beginShader("skin");


        //setColor(vec3(0, 1, 0));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));

        renderer.translate(vec3(0, 70, 0));
        renderer.scale(vec3(10));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        //renderer.endShader();

        ASkeletonDrawer drawer;
        //drawer.setJointRadius(0.05);
        //drawer.setScale(100);
        drawer.draw(renderer, _skeleton);

        /*for (int i = 0; i < _skeleton.getNumJoints(); i++) {
            Joint* joint = _skeleton.getByID(i);
            if (joint->getParent() == 0) continue;

            vec3 p1 = joint->getGlobalTranslation();
            vec3 p2 = joint->getParent()->getGlobalTranslation();
            drawEllipsoid(p1, p2, 5);
        }*/
    }

    virtual void keyPress(unsigned char key, int specialKey, int x, int y) {
    }

    virtual void loadDir(const std::string& dir) {
    }

    virtual void loadMotion(const std::string& filename) {
        atk::BVHReader reader;
        reader.load(filename, _skeleton, _motion);
    }

private:

    float _factor = 1;
    Skeleton _skeleton;
    Motion _motion;
    AGLTFGeometry _geometry;
    AGLTFGeometry _origGeometry;

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




