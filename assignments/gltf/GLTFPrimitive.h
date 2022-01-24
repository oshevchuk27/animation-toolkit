
#ifndef GLTFPRIMITIVE_H_
#define GLTFPRIMITIVE_H_

#include "agl/mesh.h"

static void CheckErrors(std::string desc) {
  GLenum e = glGetError();
  if (e != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL error in \"%s\": %d (%d)\n", desc.c_str(), e, e);
    //exit(20);
  }
}

class GLTFPrimitive : public agl::Mesh {
 public:
  GLTFPrimitive(const tinygltf::Primitive& primitive, const tinygltf::Model& model) {

    int mode = -1;
    if (primitive.mode == TINYGLTF_MODE_TRIANGLES) {
      mode = GL_TRIANGLES;
    } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_STRIP) {
      mode = GL_TRIANGLE_STRIP;
    } else if (primitive.mode == TINYGLTF_MODE_TRIANGLE_FAN) {
      mode = GL_TRIANGLE_FAN;
    } else if (primitive.mode == TINYGLTF_MODE_POINTS) {
      mode = GL_POINTS;
    } else if (primitive.mode == TINYGLTF_MODE_LINE) {
      mode = GL_LINES;
    } else if (primitive.mode == TINYGLTF_MODE_LINE_LOOP) {
      mode = GL_LINE_LOOP;
    } else {
      std::cout << "ERROR: Unsupported primitive mode - " << primitive.mode << std::endl;
    }
    _mode = mode;
    _isDynamic = false;
    _initialized = true;

    GLuint posBuf = 0, normBuf = 0, texBuf = 0, idxBuf = 0, jntBuf = 0, wgtBuf = 0;
    if (primitive.indices >= 0) {
      idxBuf = bindBuffer(primitive.indices, model);
      const tinygltf::Accessor &ia = model.accessors[primitive.indices];
      _count = ia.count;
      _componentType = ia.componentType;
      _byteOffset = ia.byteOffset;
    }

    if (primitive.attributes.count("POSITION")) {
      int id = primitive.attributes.at("POSITION"); 
      posBuf = bindBuffer(id, model);
    }
    else {
      std::cout << "ERROR: No positions...primitive is empty\n";
      return;
    }

    if (primitive.attributes.count("NORMAL")) {
      int id = primitive.attributes.at("NORMAL"); 
      normBuf = bindBuffer(id, model);
    }

    if (primitive.attributes.count("TEXCOORD_0")) {
      int id = primitive.attributes.at("TEXCOORD_0"); 
      texBuf = bindBuffer(id, model);
      _hasUV = true;
    }

    if (primitive.attributes.count("JOINTS_0")) {
      int id = primitive.attributes.at("JOINTS_0"); 
      jntBuf = bindBuffer(id, model);
      _hasSkin = true;
    }
    
    if (primitive.attributes.count("WEIGHTS_0")) {
      int id = primitive.attributes.at("WEIGHTS_0"); 
      wgtBuf = bindBuffer(id, model);
      _hasSkin = true;
    }

    _buffers.push_back(idxBuf); // indices
    _buffers.push_back(posBuf); // positions
    _buffers.push_back(normBuf); // normals
    _buffers.push_back(texBuf); // uvs
    _buffers.push_back(jntBuf); // joint influences (for skinning)
    _buffers.push_back(wgtBuf); // weights (for skinning)

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxBuf);

    // Position
    glBindBuffer(GL_ARRAY_BUFFER, posBuf);
    setupVertexAttribArray(0, primitive.attributes.at("POSITION"), model);

    // Normal
    if (normBuf > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, normBuf);
      setupVertexAttribArray(1, primitive.attributes.at("NORMAL"), model);
    }

    // Tex coords
    if (texBuf > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, texBuf);
      setupVertexAttribArray(2, primitive.attributes.at("TEXCOORD_0"), model);
    }

    // Skinning
    if (jntBuf > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, jntBuf);
      setupVertexAttribArray(3, primitive.attributes.at("JOINTS_0"), model);
    }

    if (wgtBuf > 0) {
      glBindBuffer(GL_ARRAY_BUFFER, wgtBuf);
      setupVertexAttribArray(4, primitive.attributes.at("WEIGHTS_0"), model);
    }

    glBindVertexArray(0);
  }

  virtual ~GLTFPrimitive() {
    if (_buffers.size() > 0) {
      glDeleteBuffers((GLsizei)_buffers.size(), _buffers.data());
      _buffers.clear();
    }

    if (_vao != 0) {
      glDeleteVertexArrays(1, &_vao);
      _vao = 0;
    }
  }

  GLuint bindBuffer(int id, const tinygltf::Model& model) {
    GLuint buff = 0;
    glGenBuffers(1, &buff);
    const tinygltf::Accessor &accessor = model.accessors[id];
    const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
    if (bufferView.target == 0) {
      std::cout << "WARN: bufferView.target is zero" << std::endl;
      return 0;  // Unsupported bufferView.
    }
    
    const tinygltf::Buffer &buffer = model.buffers[bufferView.buffer];
    glBindBuffer(bufferView.target, buff);
    glBufferData(bufferView.target, bufferView.byteLength,
                  &buffer.data.at(0) + bufferView.byteOffset,
                  GL_STATIC_DRAW);
    CheckErrors("bind buffer");
    return buff;
  }

  int findAccessor(int viewId, const tinygltf::Model& model) {
    for (int i = 0; i < model.accessors.size(); i++) {
      if (model.accessors.at(i).bufferView == viewId) return i;
    }
    return -1;
  }

  void setupVertexAttribArray(int layoutId, int id, const tinygltf::Model& model) {
    const tinygltf::Accessor &accessor = model.accessors[id];
    int size = 1;
    if (accessor.type == TINYGLTF_TYPE_SCALAR) {
      size = 1;
    } else if (accessor.type == TINYGLTF_TYPE_VEC2) {
      size = 2;
    } else if (accessor.type == TINYGLTF_TYPE_VEC3) {
      size = 3;
    } else if (accessor.type == TINYGLTF_TYPE_VEC4) {
      size = 4;
    } else {
      std::cout << "ERROR: Invalid size - " << accessor.type << std::endl;
    }
    const tinygltf::BufferView &bufferView = model.bufferViews[accessor.bufferView];
    int byteStride = accessor.ByteStride(bufferView);
    assert(byteStride != -1);

    glVertexAttribPointer(layoutId, size,
                          accessor.componentType,
                          accessor.normalized ? GL_TRUE : GL_FALSE,
                          byteStride, BUFFER_OFFSET(accessor.byteOffset));
    CheckErrors("vertex attrib pointer");
    glEnableVertexAttribArray(layoutId);
    CheckErrors("enable vertex attrib array");
  }

  virtual void init() {
  }

  virtual void render() const {
    glBindVertexArray(_vao);
    glDrawElements(_mode, _count, _componentType, BUFFER_OFFSET(_byteOffset));
    CheckErrors("draw elements");
    glBindVertexArray(0);
  }

  bool hasSkin() const {
    return _hasSkin;
  }

 private:
  int _mode = -1;
  int _count = -1;
  int _componentType = -1;
  int _byteOffset = -1;
  bool _hasSkin = false;
};

#endif