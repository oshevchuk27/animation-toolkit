#ifndef AGLFGeometry_H_
#define AGLFGeometry_H_

#include <string>
#include <map>
#include "atk/skeleton.h"
#include "agl/renderer.h"

namespace tinygltf {
   class Model;
}

class AGLTFGeometry
{
 public:
   AGLTFGeometry();
   virtual ~AGLTFGeometry();

   void load(const std::string& filename);
   void draw(agl::Renderer& renderer, const atk::Skeleton& skeleton);
   void draw(agl::Renderer& renderer);
   void print(bool printBuffers = false);
   bool hasSkin() const;

 protected:
   void clear();
   void setupMeshes();
   void drawNode(agl::Renderer& renderer, int nodeId);
   void drawMesh(agl::Renderer& renderer, int meshId);
   //void setShaderJointMats()

 private:
   class tinygltf::Model* _model;
   std::map<int, std::vector<class GLTFPrimitive*>> _geometry;
};

#endif