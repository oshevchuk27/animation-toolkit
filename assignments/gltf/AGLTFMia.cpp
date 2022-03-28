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
      loadMotion("../motions/Mia/gesture.bvh");
      for (int i = 0; i < _skeleton.getNumJoints(); i++)
      {
         atk::Joint* joint = _skeleton.getByID(i);
         joint->setLocalTranslation(joint->getLocalTranslation());
      }

      _geometry.load("../models/Mia/Mia-v2.0.glb"); 
      _geometry.print();

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

      renderer.beginShader("skin");
      setColor(vec4(1));
      renderer.push();
      renderer.rotate(-3.14 / 2.0, vec3(1, 0, 0));
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
};

int main(int argc, char** argv)
{
   AGLTFSimple viewer;
   //viewer.loadMotion("../motions/Mia/idle.bvh");
   viewer.run(); 
}

