#include "atkui/framework.h"
#include "atk/toolkit.h"

#include "agl/window.h"
#include "agl/mesh/plane.h"
#include "atkui/framework.h"



using namespace atk;
using namespace agl;

using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::dualquat;
using glm::quat;


class NewCylinder : public agl::TriangleMesh {
public:

    NewCylinder::NewCylinder(float r, float len, GLuint nSlices) {
        _r1 = r;
        _r2 = r;
        _length = len;
        _nSlices = nSlices;
    }

    NewCylinder::NewCylinder(float r1, float r2, float len, GLuint nSlices) {
        _r1 = r1;
        _r2 = r2;
        _length = len;
        _nSlices = nSlices;
    }


    void NewCylinder::init() {
        computeMesh(_r1, _r2, _length, _nSlices);
    }

    void NewCylinder::computeMesh(float r1, float r2, float len, GLuint nSlices) {
        int nVerts = (nSlices + 1) * 2 + nSlices * 2;

        // num top triangles = nSlices
        // num bottom triangles = nSlices
        // num side triangles = nSlices * 2
        int elements = (nSlices) * 4 * 3;

        // Verts
        std::vector<GLfloat> p(3 * nVerts);
        // Normals
        std::vector<GLfloat> n(3 * nVerts);
        // Tex coords
        std::vector<GLfloat> tex(2 * nVerts);
        // Elements
        std::vector<GLuint> el(elements);

        // Generate positions and normals
        GLfloat theta;
        GLfloat thetaFac = glm::two_pi<float>() / nSlices;
        GLfloat hLen = len;
        GLuint offsets[4] = {
          0,
          nSlices + 1,
          (nSlices + 1) * 2 - 1,
          (nSlices + 1) * 2 + nSlices - 1
        };

        GLuint idx = 0, tIdx = 0;
        for (GLuint i = 0; i <= nSlices; i++) {
            if (i == 0) {
                for (GLuint j = 0; j < 2; j++) {
                    int offset = offsets[j];
                    float y = j % 2 == 0 ? 0 : hLen;
                    p[idx + 0 + 3 * offset] = 0.0;
                    p[idx + 1 + 3 * offset] = y;
                    p[idx + 2 + 3 * offset] = 0.0;


                    n[idx + 0 + 3 * offset] = 0.0;
                    n[idx + 1 + 3 * offset] = j % 2 == 0 ? -1.0 : 1.0;
                    n[idx + 2 + 3 * offset] = 0.0;

                    tex[tIdx + 0 + 2 * offset] = 0.0;
                    tex[tIdx + 1 + 2 * offset] = (y+ hLen) / len;
                }
            }
            else {
                theta = (i - 1) * thetaFac;
                float nz = cosf(theta);
                float nx = sinf(theta);

                float s = 0.5 * (theta / glm::pi<float>());

                for (GLuint j = 0; j < 4; j++) {
                    int offset = offsets[j];
                    float y = j % 2 == 0 ? 0: hLen;
                    float r = j % 2 == 0 ? r1 : r2;
                    float t = (y + hLen) / len;

                    p[idx + 0 + 3 * offset] = r * nx;
                    p[idx + 1 + 3 * offset] = y;
                    p[idx + 2 + 3 * offset] = r * nz;

                    n[idx + 0 + 3 * offset] = j < 2 ? 0.0 : nx;
                    n[idx + 1 + 3 * offset] = j < 2 ? (j % 2 == 0 ? -1.0 : 1.0) : 0.0;
                    n[idx + 2 + 3 * offset] = j < 2 ? 0.0 : nz;  // todo: fix normal

                    tex[tIdx + 0 + 2 * offset] = s;
                    tex[tIdx + 1 + 2 * offset] = t;

                    
                }

            }

           
            idx += 3;
            tIdx += 2;
        }

        // Generate the element list
        idx = 0;
        for (GLuint i = 0; i < nSlices; i++) {
            el[idx + 0] = 0;
            el[idx + 1] = (i + 1) % (nSlices)+1;
            el[idx + 2] = (i + 1);

            el[idx + 0 + 3 * nSlices] = el[idx + 0] + offsets[1];
            el[idx + 1 + 3 * nSlices] = el[idx + 2] + offsets[1];
            el[idx + 2 + 3 * nSlices] = el[idx + 1] + offsets[1];

            idx += 3;
        }


        int offset = nSlices * 2 + 2;
        idx = (nSlices) * 2 * 3;
        for (GLuint i = 0; i < nSlices; i++) {
            int back1 = i + offset;
            int back2 = (i + 1) % nSlices + offset;

            int front1 = back1 + nSlices;
            int front2 = back2 + nSlices;

            el[idx + 0] = front1;
            el[idx + 1] = back1;
            el[idx + 2] = back2;

            el[idx + 3] = front1;
            el[idx + 4] = back2;
            el[idx + 5] = front2;

            idx += 6;
        }

      

        setIsDynamic(true);

        initBuffers(&el, &p, &n, &tex);
    }






   /*void update(float elapsedTime, Skeleton skeleton) {



        for (int i = 0; i < numVertices(); i++) {

            vec3 p = vec3(points[3 * i], points[3 * i + 1], points[3 * i + 2]);


            quat Bone1Rot = skeleton.getByName("root")->getLocal2Global().r();
            vec3 Bone1Trans = skeleton.getByName("root")->getLocal2Global().t();

            dualquat Bone1 = dualquat(Bone1Rot, Bone1Trans);


            quat Bone2Rot = skeleton.getByName("joint1")->getLocal2Global().r();
            vec3 Bone2Trans = skeleton.getByName("joint1")->getLocal2Global().t();

            dualquat Bone2 = dualquat(Bone2Rot, Bone2Trans);




            dualquat RestBone1 = dualquat(RestBone1Rot, RestBone1Trans);

            dualquat RestBone2 = dualquat(RestBone2Rot, RestBone2Trans);


            dualquat newquat = normalize(weights[i * 2] * Bone1 * inverse(RestBone1) +
                weights[i * 2 + 1] * Bone2 * inverse(RestBone2));

           


            vec3 newp = newquat * p;

            setVertexData(POSITION, i, vec4(newp, 0));

        }



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


        RestBone1Transform = skeleton.getByName("root")->getLocal2Global();
        RestBone1Rot = RestBone1Transform.r();
        RestBone1Trans = RestBone1Transform.t();


        RestBone2Transform = skeleton.getByName("joint1")->getLocal2Global();

        RestBone2Rot = RestBone2Transform.r();
        RestBone2Trans = RestBone2Transform.t();

        init();

    }*/

private:

    float _r1;
    float _r2;
    float _length;
    float _nSlices;

    int stacks;

    /*std::vector<GLuint> indices;
    std::vector<GLfloat> points;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> weights;

    Transform  RestBone1Transform;
    Transform  RestBone2Transform;

    quat RestBone1Rot;
    vec3 RestBone1Trans;
    quat RestBone2Rot;
    vec3 RestBone2Trans;*/



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
                parent->setLocalRotation(glm::angleAxis<float>(sin(1.5f * elapsedTime() + i), vec3(0, 0, 1)));
                //parent->setLocalRotation(glm::angleAxis<float>(0.3f, vec3(0, 0, 1)));

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

        setColor(vec3(1));


        for (int i = 0; i < _mesh.numVertices(); i++) {
            glm::vec3 p = glm::vec3(_mesh.vertexData(Mesh::POSITION, i));
            drawSphere(p, 0.3);
        }


        renderer.pop();

       // _mesh.update(elapsedTime(), skeleton);

        drawFloor(200, 4, 1);
    }



protected:
    Skeleton skeleton;
    NewCylinder _mesh = NewCylinder(0.5, 2, 8);


};

int main(int argc, char** argv)
{
    LinearBlendSkinning viewer;
    viewer.run();
}



























/*#include "atkui/framework.h"
#include "agl/mesh/cylinder.cpp"

using glm::vec3;
using agl::randomUnitVector;



class Sphere3D : public atkui::Framework {
public:
    Sphere3D() : atkui::Framework(atkui::Perspective) {
    };
    // the initial position of the sphere
    vec3 currentPos;
    // the direction of the sphere movement
    vec3 velocity;
    // indicates whether the SPACEBAR key is pressed
    bool spaceKey;

    virtual void setup() {
        // initial position of the sphere
        currentPos = vec3(0);
        // initially, the SPACEBAR is not pressed
        spaceKey = false;
    }

    virtual void scene() {
        // colors are RGB triplets in range [0,1]
        setColor(vec3(0, 1, 0));

        // draw a sphere at center of the world
        float radius = 50.0;


        if (spaceKey) {
            currentPos = currentPos + 100.0f * velocity * dt();
        }

        agl::Cylinder cylinder = agl::Cylinder(5, 10, 6, 8);

        init();

        
        //drawCylinder(currentPos, radius);
    }

    virtual void keyUp(int key, int mods) {
        if (key == GLFW_KEY_SPACE) {
            spaceKey = true;
            velocity = randomUnitVector();
        }
        else if (key == GLFW_KEY_R) {
            spaceKey = false;
            // going back to the original position
            currentPos = vec3(0);
        }
    }
};

int main(int argc, char** argv)
{
    Sphere3D viewer;
    viewer.run();
    return 0;
}*/
