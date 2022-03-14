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
   int getNumMeshes() const;
   int getNumPrimitives(int meshid) const;
   int getNumVertices(int meshid, int primid, const char* attrib) const;
   glm::vec4 getVertexData(int meshid, int primid, const char* attrib, int vertexid) const;
   void setVertexData(int meshid, int primid, const char* attrib, int vertexid, const glm::vec4& val);

   int getNumSkins() const;
   void getInverseBindMatrices(int skinid, std::vector<glm::mat4>& ibm) const;
   int getNumSkinJoints(int skinid) const;
   std::string getJointName(int skinid, int jointid) const ;
   glm::mat4 getInverseBindMatrix(int skinid, int jointid) const ;

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