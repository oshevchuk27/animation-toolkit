#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "AGLTFGeometry.h"

using namespace atk;
using namespace std;
using namespace glm;

class AGLTFSimple : public atkui::Framework {
public:
    AGLTFSimple() : atkui::Framework(atkui::Perspective) {
    }

    virtual void setup() {
        

        _geometry.load("../models/cube.glb");
        _geometry.print();
        

       
    }

    virtual void scene() {
        
    }

    virtual void keyPress(unsigned char key, int specialKey, int x, int y) {
    }

    virtual void loadDir(const std::string& dir) {
    }

    virtual void loadMotion(const std::string& filename) {
        
    }

protected:
    Skeleton _skeleton;
    Motion _motion;
    AGLTFGeometry _geometry;
};

int main(int argc, char** argv)
{
    AGLTFSimple viewer;
    //viewer.loadMotion("../motions/Mia/idle.bvh");
    viewer.run();
}
