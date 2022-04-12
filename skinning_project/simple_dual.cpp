#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace atk;
using namespace agl;

using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::dualquat;
using glm::quat;

int main() {
  quat identity = glm::angleAxis(0.0f, vec3(0.0f, 0.0f, 1.0f));
  dualquat translate_only(identity, vec3(0,0,1));

  vec4 p = vec4(1,0,0,1);
  vec4 testp = translate_only * p;
  std::cout << "test translate: " << glm::to_string(testp) << std::endl;

  quat r45 = glm::angleAxis(glm::radians<float>(45), vec3(0.0f, 0.0f, 1.0f));
  dualquat rotate_only(r45, vec3(0,0,0));
  testp = rotate_only * p;
  std::cout << "test rotate: " << glm::to_string(testp) << std::endl;

  dualquat trans(r45, vec3(0,1,1));
  testp = trans * p;
  std::cout << "test trans: " << glm::to_string(testp) << std::endl;

  dualquat translate_2(identity, vec3(0,0,-1));
  dualquat blend = normalize(0.5f*translate_only + 0.5f*translate_2); 
  testp = blend * p;
  std::cout << "test blend: " << glm::to_string(testp) << std::endl;
  return 0;
}
