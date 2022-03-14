#include "AGLTFGeometry.h"
#include "agl/agl.h"
#include <map>

// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define BUFFER_OFFSET(i) ((char *)NULL + (i))
#include "gltf/tiny_gltf.h"
#include "GLTFPrimitive.h"

using glm::mat4;
using glm::vec3;
using glm::quat;

static bool LoadImageDataCb(tinygltf::Image *, const int, std::string *,
                                      std::string *, int, int,
                                      const unsigned char *, int, void *) {

  return true;
}

AGLTFGeometry::AGLTFGeometry() : _model(0) {

}
 
AGLTFGeometry::~AGLTFGeometry() {
  clear();
}

void AGLTFGeometry::clear() {
  for (auto item : _geometry) {
    for (int i = 0; i < item.second.size(); i++) {
      delete item.second[i];
    }
  }
  _geometry.clear();

  delete _model;
  _model = 0;
}

void AGLTFGeometry::load(const std::string& filename)
{
  clear();

  std::string err;
  std::string warn;

  auto extIdx = filename.find_last_of(".");
  std::string ext = "";
  if (extIdx != std::string::npos) 
  {
    ext = filename.substr(extIdx + 1); 
  }

  tinygltf::TinyGLTF loader;
  loader.SetImageLoader(LoadImageDataCb, NULL);

  _model = new tinygltf::Model();
  bool success = false;
  if (ext == "glb")
  {
    success = loader.LoadBinaryFromFile(_model, &err, &warn, filename);
  }
  else if (ext == "gltf")
  {
    success = loader.LoadASCIIFromFile(_model, &err, &warn, filename);
  }

  if (!warn.empty()) 
  {
    printf("Warn: %s\n", warn.c_str());
    return;
  }

  if (!err.empty()) 
  {
    printf("Err: %s\n", err.c_str());
    return;
  }

  if (!success)
  {
    clear();
    std::cerr << "Cannot load file: " << filename << std::endl;
    return;
  }

  setupMeshes();
}

bool AGLTFGeometry::hasSkin() const {
  return _model && _model->skins.size() > 0;
}

void AGLTFGeometry::setupMeshes() {
  for (size_t i = 0; i < _model->meshes.size(); i++) {
    const tinygltf::Mesh &mesh = _model->meshes[i]; 
    std::vector<GLTFPrimitive*> glPrimitives;
    for (size_t p = 0; p < mesh.primitives.size(); p++) {
      tinygltf::Primitive primitive = mesh.primitives[p];
      
      if (primitive.indices < 0) {
        std::cout << "WARNING: only index mode is supported.\n" << std::endl;
        continue;
      }

      GLTFPrimitive* geometry = new GLTFPrimitive(primitive, *_model);
      glPrimitives.push_back(geometry);

      // ASN TODO: material
    }
    _geometry[i] = glPrimitives;
  }
}

int AGLTFGeometry::getNumMeshes() const {
  return _geometry.size();
}

int AGLTFGeometry::getNumPrimitives(int meshid) const {
  assert(meshid >= 0 && meshid < _geometry.size());
  const std::vector<GLTFPrimitive*>& prims = _geometry.at(meshid);
  return prims.size();
}

int AGLTFGeometry::getNumVertices(int meshid, int primid, const char* attrib) const {
  assert(meshid >= 0 && meshid < _geometry.size());
  const std::vector<GLTFPrimitive*>& prims = _geometry.at(meshid);
  assert(primid >= 0 && primid < prims.size());
  return prims[primid]->getNumber(attrib);
}

void AGLTFGeometry::setVertexData(int meshid, int primid, const char* attrib, int vertexid, const glm::vec4& v) {
  assert(meshid >= 0 && meshid < _geometry.size());
  const std::vector<GLTFPrimitive*>& prims = _geometry.at(meshid);
  assert(primid >= 0 && primid < prims.size());
  prims[primid]->setVertexData(attrib, vertexid, v);
}

glm::vec4 AGLTFGeometry::getVertexData(int meshid, int primid, const char* attrib, int vertexid) const {
  assert(meshid >= 0 && meshid < _geometry.size());
  const std::vector<GLTFPrimitive*>& prims = _geometry.at(meshid);
  assert(primid >= 0 && primid < prims.size());
  return prims[primid]->getVertexData(attrib, vertexid);
}

void AGLTFGeometry::drawMesh(agl::Renderer& renderer, int meshId) {
  const std::vector<GLTFPrimitive*>& prims = _geometry[meshId];
  for (int i = 0; i < prims.size(); i++) {
    renderer.mesh(*prims[i]);
  }
}

void AGLTFGeometry::drawNode(agl::Renderer& renderer, int nodeId) 
{
  const tinygltf::Node &node = _model->nodes[nodeId]; 

  // Apply xform
  renderer.push();
  if (node.matrix.size() == 16) {
    // Use `matrix' attribute
    // ASN: TODO - Is matrix row-major or col-major?
    mat4 m(node.matrix[0], node.matrix[4], node.matrix[8], node.matrix[12], 
           node.matrix[1], node.matrix[5], node.matrix[9], node.matrix[13],
           node.matrix[2], node.matrix[6], node.matrix[10], node.matrix[14],
           node.matrix[3], node.matrix[7], node.matrix[11], node.matrix[15]);
    renderer.transform(m);
  } else {
    // Assume Trans x Rotate x Scale order
    if (node.translation.size() == 3) {
      renderer.translate(vec3(node.translation[0], node.translation[1], node.translation[2]));
    }

    if (node.rotation.size() == 4) {
      quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
      vec3 axis = glm::axis(q);
      float angle = glm::angle(q);
      renderer.rotate(angle, axis);
    }

    if (node.scale.size() == 3) {
      renderer.scale(vec3(node.scale[0], node.scale[1], node.scale[2]));
    }
  }

  //std::cout << "node " << node.name << ", Meshes " << node.mesh << std::endl;

  if (node.mesh > -1) {
    assert(node.mesh < (int) _model->meshes.size());
    drawMesh(renderer, node.mesh); 
  }

  // Draw child nodes.
  for (size_t i = 0; i < node.children.size(); i++) {
    assert(node.children[i] < (int)_model->nodes.size());
    drawNode(renderer, node.children[i]); 
  }

  renderer.pop();
}

void AGLTFGeometry::draw(agl::Renderer& renderer)
{
  if (!_model) return;

  // If the glTF asset has at least one scene, and doesn't define a default one
  // just show the first one we can find
  assert(_model->scenes.size() > 0);
  int scene_to_display = _model->defaultScene > -1 ? _model->defaultScene : 0;
  const tinygltf::Scene &scene = _model->scenes[scene_to_display];
  for (size_t i = 0; i < scene.nodes.size(); i++) {
    drawNode(renderer, scene.nodes[i]); 
  }
}

int AGLTFGeometry::getNumSkins() const{
  assert(_model);
  return _model->skins.size();
}

int AGLTFGeometry::getNumSkinJoints(int skinid) const{
  assert(_model);
  const tinygltf::Skin& skin = _model->skins[skinid]; 
  return skin.joints.size();
}

std::string AGLTFGeometry::getJointName(int skinid, int jointid) const {
  const tinygltf::Skin& skin = _model->skins[skinid]; 
  
  int nodeId = skin.joints[jointid];
  const tinygltf::Node& joint = _model->nodes[nodeId];
  return joint.name; 
}

glm::mat4 AGLTFGeometry::getInverseBindMatrix(int skinid, int jointid) const {
  const tinygltf::Skin& skin = _model->skins[skinid]; 

  int id = skin.inverseBindMatrices;
  assert(id != -1);
  
  const tinygltf::Accessor& accessor = _model->accessors[id];
  const tinygltf::BufferView& view = _model->bufferViews[accessor.bufferView];
  const tinygltf::Buffer& raw = _model->buffers[view.buffer];

  // ASN TODO: Should only copy the matrix I need
  std::vector<mat4> ibm(accessor.count);
  memcpy((void*) (ibm.data()), 
    raw.data.data() + view.byteOffset + accessor.byteOffset, view.byteLength);

  return ibm[jointid];
}

void AGLTFGeometry::draw(agl::Renderer& renderer, const atk::Skeleton& skeleton)
{
  if (!_model || _model->skins.size() == 0) return;
  const tinygltf::Skin& skin = _model->skins[0]; 

  // Save joint information (should move to drawMesh?)
  int id = skin.inverseBindMatrices;
  assert(id != -1);
  
  const tinygltf::Accessor& accessor = _model->accessors[id];
  const tinygltf::BufferView& view = _model->bufferViews[accessor.bufferView];
  const tinygltf::Buffer& raw = _model->buffers[view.buffer];

  // ASN TODO: Should only do this once
  std::vector<mat4> ibm(accessor.count);
  memcpy((void*) (ibm.data()), 
    raw.data.data() + view.byteOffset + accessor.byteOffset, view.byteLength);

  /*for (int i = 0; i < ibm.size(); i++) {
    std::cout << "skin ibm: " << glm::to_string(ibm[i]) << std::endl;
  }*/
  
  std::vector<glm::mat4> jointMats(skin.joints.size());
  for (unsigned int i = 0; i < skin.joints.size(); i++)
  {
    jointMats[i] = mat4(1.0); // identity matrix

    int nodeId = skin.joints[i];
    const tinygltf::Node& joint = _model->nodes[nodeId];
    atk::Joint* ajoint = skeleton.getByName(joint.name); 
    if (!ajoint) 
    {
       std::cout << "Cannot find: " << joint.name << std::endl;
       continue;
    }
    
    glm::mat4 local2global = ajoint->getLocal2Global().matrix();
    jointMats[i] = local2global * ibm[i];
    //std::cout << i<< " " << ajoint->getName() << " " << local2global << std::endl;
    //std::cout << i<< " " << jointMats[i]  << std::endl;
    //std::cout << "---------------------------\n";
  }
  /*
  for (int i = 0; i < jointMats.size(); i++) {
    std::cout << "jointMats : " << i << "\n" << glm::to_string(jointMats[i]) << std::endl;
  }*/
  renderer.setUniform("JointMats", jointMats);
  draw(renderer);
}

void AGLTFGeometry::print(bool printBuffers)
{
  if (!_model) return;
  for (size_t i = 0; i < _model->nodes.size(); i++) 
  {
    const tinygltf::Node& node = _model->nodes[i];
    std::string scalestr = "";
    if (node.scale.size() > 0)
    {
        scalestr = std::to_string(node.scale[0]) + " " +
                  std::to_string(node.scale[1]) + " " + 
                  std::to_string(node.scale[2]);
    }
    std::string translationstr = "";
    if (node.translation.size() > 0)
    {
        translationstr = std::to_string(node.translation[0]) + " " +
                  std::to_string(node.translation[1]) + " " + 
                  std::to_string(node.translation[2]);
    }
    glm::quat q = atk::IdentityQ;
    if (node.rotation.size() > 0)
    {
        float x = node.rotation[0];
        float y = node.rotation[1]; 
        float z = node.rotation[2];
        float w = node.rotation[3];
        q = glm::quat(x, y, z, w);
    }

    std::cout << "NODE: " << i << " name: " << node.name 
              << " skin: " << node.skin 
              << " camera: " << node.camera 
              << " mesh: " << node.mesh 
              << " scale: " << scalestr
              << " rotation: " << glm::to_string(q)
              << " translation: " << translationstr
              << " matrix: " << node.matrix.size() 
              << " weights: " << node.weights.size() 
              << std::endl;
  }

  for (size_t i = 0; i < _model->skins.size(); i++) 
  {
    const tinygltf::Skin& skin = _model->skins[i];
    std::cout << "SKIN: " << i << " name: " << skin.name 
              << " skeleton: " << skin.skeleton 
              << " inverseBindMatrices: " << skin.inverseBindMatrices 
              << " numJoints: " << skin.joints.size()
              << std::endl;
  }

  for (size_t i = 0; i < _model->bufferViews.size(); i++) 
  {
    const tinygltf::BufferView& bufferview = _model->bufferViews[i];
    std::cout << "VIEW: " << i << " buffer: " << bufferview.buffer 
              << " offset: " << bufferview.byteOffset 
              << " length: " << bufferview.byteLength 
              << " stride: " << bufferview.byteStride 
              << " target: " << bufferview.target 
              << std::endl;

  }

  for (size_t i = 0; i < _model->accessors.size(); i++) 
  {
    const tinygltf::Accessor& accessor = _model->accessors[i];
    std::cout << "ACCESSOR: " << i << " view: " << accessor.bufferView
              << " offset: " << accessor.byteOffset 
              << " component type: " << accessor.componentType 
              << " count: " << accessor.count 
              << " type: " << accessor.type 
              << std::endl;

    if (printBuffers) {
      const tinygltf::BufferView& bufferview = _model->bufferViews[accessor.bufferView];
      const tinygltf::Buffer &buffer = _model->buffers[bufferview.buffer];
      const unsigned char *tmp_buffer = &buffer.data.at(0) + bufferview.byteOffset + accessor.byteOffset;
      int size = 0;
      if (accessor.type == TINYGLTF_TYPE_VEC2) size = 2;
      if (accessor.type == TINYGLTF_TYPE_VEC3) size = 3;
      if (accessor.type == TINYGLTF_TYPE_VEC4) size = 4;
      if (accessor.type == TINYGLTF_TYPE_MAT2) size = 4;
      if (accessor.type == TINYGLTF_TYPE_MAT3) size = 9;
      if (accessor.type == TINYGLTF_TYPE_MAT4) size = 16;
      if (accessor.type == TINYGLTF_TYPE_SCALAR) size = 1;
      // ASN TODO:Handle stride
      for(size_t p = 0; p < accessor.count * size; p++)
      {
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT) {
          short* b = (short*)tmp_buffer;
          std::cout << "buffer contents [" << p << "] = " << b[p] << '\n';
        }
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
          ushort* b = (ushort*)tmp_buffer;
          std::cout << "buffer contents [" << p << "] = " << b[p] << '\n';
        }
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_INT) {
          int* b = (int*)tmp_buffer;
          std::cout << "buffer contents [" << p << "] = " << b[p] << '\n';
        }
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
          uint* b = (uint*)tmp_buffer;
          std::cout << "buffer contents [" << p << "] = " << b[p] << '\n';
        }
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
          float* b = (float*)tmp_buffer;
          std::cout << "buffer contents [" << p << "] = " << b[p] << '\n';
        }
      }
    }
  }

  for (size_t i = 0; i < _model->meshes.size(); i++) 
  {
    const tinygltf::Mesh& mesh = _model->meshes[i];
    std::cout << "MESH: " << i << " " << mesh.name << std::endl;
    for (size_t p = 0; p < mesh.primitives.size(); p++) 
    {
        const tinygltf::Primitive& primitive = mesh.primitives[p];
        std::cout << "PRIMITIVE: " << p << "\n"; 
        for (auto attribute : primitive.attributes) 
        {
          std::cout << "ATTR: " << attribute.first << " " 
              << attribute.second << std::endl;
        }
        std::cout << "MODE: " << primitive.mode << std::endl;
        for (size_t j = 0; j < primitive.targets.size(); j++) 
        {
          for (auto attribute : primitive.targets[j]) 
          {
              std::cout << "TARGET ATTR: " << attribute.first << " " 
                << attribute.second << std::endl;
          }
        }
    }
  }
}
