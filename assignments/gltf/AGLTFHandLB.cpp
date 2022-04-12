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
        loadMotion("../motions/hand.bvh");

        for (int i = 0; i < _skeleton.getNumJoints(); i++)
        {
            atk::Joint* joint = _skeleton.getByID(i);
            joint->setLocalTranslation(joint->getLocalTranslation());
        }


        lookAt(vec3(50), vec3(0));

      
        
        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/hand.glb");
        _origGeometry.load("../models/hand.glb"); // need to keep original vertices
        _geometry.print(false);


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

        // try to edit vertices
        _factor = 2 * sin(elapsedTime());
        _skeleton.getByID(0)->setLocalRotation(glm::angleAxis(0.0f, vec3(0, 0, 1)));
        _skeleton.getByID(1)->setLocalRotation(glm::angleAxis(_factor, vec3(0, 0, 1)));
        _skeleton.fk();


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

        renderer.beginShader("skin");


        //setColor(vec3(0, 1, 0));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));

        renderer.translate(vec3(0, 70, 0));
        renderer.scale(vec3(70));
        //renderer.scale(vec3(170, 50, 170));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        renderer.endShader();

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




