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

        loadMotion("../motions/cylinder.bvh");
        for (int i = 0; i < _skeleton.getNumJoints(); i++)
        {
            atk::Joint* joint = _skeleton.getByID(i);
            joint->setLocalTranslation(joint->getLocalTranslation());
        }

       


        //_geometry.load("../models/triangle.gltf");
        //_geometry.load("../models/cube.glb");
        //_geometry.load("../models/Borb.glb");
        //_geometry.load("../models/warrok.glb");
        //_geometry.load("../models/two-shapes.gltf");
        _geometry.load("../models/cylinder.glb");
        _origGeometry.load("../models/cylinder.glb");
        // _geometry.print(true);

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

        // try to edit vertices
        



        int nummesh = _geometry.getNumMeshes();

        for (int meshid = 0; meshid < nummesh; meshid++) {
            int numprims = _geometry.getNumPrimitives(meshid);

            for (int primid = 0; primid < numprims; primid++) {
                int numverts = _geometry.getNumVertices(meshid, primid, "POSITION");

                for (int vid = 0; vid < numverts; vid++) {

                    //std::cout << "start iteration" << std::endl;

                    vec4 weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);

                    //std::cout << "weights" << weights << std::endl;
                    vec4 joints = _geometry.getVertexData(meshid, primid, "JOINTS_0", vid);

                    //std::cout << joints << std::endl;
                    //std::cout << "matrix" << _geometry.getInverseBindMatrix(0, joints[0]) << endl;
                    vec4 pos = _origGeometry.getVertexData(meshid, primid, "POSITION", vid);
                    pos[3] = 1;

                    //std::cout << "initial position" << pos << std::endl;

                    mat4 skinMatrix = weights[0] * _geometry.getInverseBindMatrix(0, joints[0]) +
                         weights[1] * _geometry.getInverseBindMatrix(0, joints[1]) +
                         weights[2] * _geometry.getInverseBindMatrix(0, joints[2]) +
                         weights[3] * _geometry.getInverseBindMatrix(0, joints[3]); 

                    //std::cout << "skin matrix" << _geometry.getInverseBindMatrix(0, joints[0]) << std::endl;
                    

                    vec4 newpos = skinMatrix * pos;

                    //std::cout << "new position" << newpos << std::endl;

                    _geometry.setVertexData(meshid, primid, "POSITION", vid, newpos);

                    //std::cout << "end iteration" << std::endl;


                    //weights = _geometry.getVertexData(meshid, primid, "WEIGHTS_0", vid);
                    //std::cout << weights << std::endl;


                }
            }
        }

        _motion.update(_skeleton, elapsedTime());

        

        renderer.beginShader("skin");

        //setColor(vec3(0, 1, 0));
        renderer.push();
        //renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
        //renderer.rotate(1.5708, vec3(1, 0, 0));
        //renderer.scale(vec3(170, 50, 170));
        renderer.rotate(1.5708, vec3(1, 0, 0));
        renderer.translate(vec3(0, 50, 0));
        renderer.scale(vec3(70));
        _geometry.draw(renderer, _skeleton);
        renderer.pop();

        renderer.endShader();

        ASkeletonDrawer drawer;
        drawer.setJointRadius(0.05);
        drawer.setScale(100);
        drawer.draw(renderer, _skeleton);
    }

    virtual void loadMotion(const std::string& filename) {
        atk::BVHReader reader;
        reader.load(filename, _skeleton, _motion);
    }

private:

    float _factor = 1;
    Skeleton _skeleton;
    AGLTFGeometry _geometry;
    AGLTFGeometry _origGeometry;
    Motion _motion;
    mat4 invMatrix0;
    mat4 invMatrix1;
};

int main(int argc, char** argv) {
    AGLTFSimple viewer;
    viewer.run();
}
