
#include "GLTFPrimitive.h"

GLTFPrimitive::GLTFPrimitive(int numTris, GLushort* indices, 
  GLfloat* positions, GLfloat* normals, GLfloat* uvs) {

  _isDynamic = true;

  int numVerts = numTris * 3;
  _positions.insert(_positions.end(), &positions[0], &positions[numVerts]);
  _indices.insert(_indices.end(), &indices[0], &indices[numTris]);

  if (normals) _normals.insert(_normals.end(), &normals[0], &normals[numVerts]);
  if (uvs) _uvs.insert(_uvs.end(), &uvs[0], &uvs[numVerts]);
}

void GLTFPrimitive::init() {
  initBuffers(&_positions, 
    _normals.size() > 0? &_normals : NULL, 
    _uvs.size() > 0? &_uvs : NULL, NULL);
}