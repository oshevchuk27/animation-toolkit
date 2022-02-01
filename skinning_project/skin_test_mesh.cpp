#include "agl/window.h"
#include "agl/mesh/plane.h"
#include "atkui/framework.h"


using glm::vec2;
using glm::vec4;
using glm::vec3;


class SkinTestMesh : public agl::TriangleMesh {
public:
    SkinTestMesh() {
        // compute verts,normals,indices here

        setIsDynamic(true);
    }

    // try to animate one vertex

    void update(float elapsedTime) {
        vec3 p = vec3(vertexData(POSITION, 0));
        setVertexData(POSITION, 0, vec4(-0.5, 0, sin(elapsedTime), 0));

    }

    void init() {


        std::vector<GLuint> indices;
        std::vector<GLfloat> points;
        std::vector<GLfloat> normals;

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



        initBuffers(&indices, &points, &normals);

    }


    vec3 position(const vec3& p, float t) {
        float angle = t;
        float frequency = 7.0;
        float amplitude = 0.05;

        float heightFn = (angle + frequency * p[0] * frequency * p[2]);
        float y = amplitude * sin(heightFn);
        return vec3(p.x, y, p.z);
    }

    



    

};

class MyWindow : public atkui::Framework {
public:

    MyWindow() : atkui:: Framework(atkui:: Perspective) {

    }


    void setup() {

        _mesh.init();
    }

    void scene() {
        renderer.push();
        setColor(vec3(0, 1, 0));
        renderer.scale(vec3(100));
        renderer.mesh(_mesh);
        renderer.pop();

        _mesh.update(elapsedTime());
       
    }

    SkinTestMesh _mesh = SkinTestMesh();
};

int main() {
    MyWindow window;
    window.run();
}