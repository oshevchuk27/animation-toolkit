#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace atk;
using namespace agl;

using glm::vec3;
using glm::vec2;
using glm::vec4;
using glm::dualquat;
using glm::quat;
using glm::mat4;

void print(const mat4& M) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << M[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

mat4 makeMat4(const dualquat& dq) {
  quat R = dq.real;
  quat t = (dq.dual * 2.0f) * inverse(R);
  vec3 test = vec3(t.x, t.y, t.z);
  return Transform(R, test).matrix();
}

int main() {
  float pi = glm::radians<float>(90);
  quat r0 = glm::angleAxis(0.0f, vec3(0.0f, 0.0f, 1.0f));
  quat r90 = glm::angleAxis(pi, vec3(0.0f, 0.0f, 1.0f));
  quat r45 = glm::angleAxis(glm::radians<float>(45), vec3(0.0f, 0.0f, 1.0f));
  quat r135 = glm::angleAxis(glm::radians<float>(135), vec3(0.0f, 0.0f, 1.0f));

  // Inverse bind matrices
  // j0 local bind pose: r90, (0,0,0)
  Transform j0(r90, vec3(0));

  // j1 local bind pose: identity, (0,4,0)
  Transform j1(r0, vec3(0,4,0));

  Transform rest0 = j0;
  Transform rest1 = j0 * j1;
  std::cout << "rest0: " << rest0 << "\nrest1: " << rest1 << std::endl;

  Transform invBind0 = rest0.inverse();
  Transform invBind1 = rest1.inverse();
  std::cout << "invBind0: " << invBind0 << "\ninvBind1: " << invBind1 << std::endl;

  dualquat ib0 = dualquat(invBind0.r(), invBind0.t());
  dualquat ib1 = dualquat(invBind1.r(), invBind1.t());
  std::cout << "ib0: " << to_string(ib0) << "\nib1: " << to_string(ib1) << std::endl;

  vec3 tt = invBind1.t(); 
  quat test = (quat(0, tt[0], tt[1], tt[2]) * invBind1.r())*0.5f;
  std::cout << "test: " << to_string(test) << std::endl;

  // Test transforms
  Transform t0(r0, vec3(0, 0, 0));
  Transform t1(r135, vec3(0,4,0));

  Transform local2global0 = t0;
  Transform local2global1 = t0 * t1;

  Transform T0 = local2global0 * invBind0;
  Transform T1 = local2global1 * invBind1;
  std::cout << "T0: " << T0 << "\nT1: " << T1 << std::endl;

  dualquat dq0 = dualquat(T0.r(), T0.t());
  dualquat dq1 = dualquat(T1.r(), T1.t());
  std::cout << "dq0: " << to_string(dq0) << "\ndq1: " << to_string(dq1) << std::endl;

  float w0 = 0.5f;
  float w1 = 0.5f;
  dualquat blend = lerp(dq0, dq1, 0.5f); 
  blend = normalize(blend);
  std::cout << "blend: " << to_string(blend) << std::endl;

  vec4 p = vec4(40,40,0,1);
  vec4 newp = makeMat4(blend) * p;
  std::cout << p << " " << blend * p << std::endl;
  std::cout << p << " " << newp << std::endl;

  // linear blend skinning
  mat4 T = w0 * (T0.matrix()) + w1 * (T1.matrix());
  vec4 newplb = T * p;
  std::cout << p << " " << newplb << std::endl;

  newplb = w0 * (T0.matrix() * p) +
           w1 * (T1.matrix() * p);
  std::cout << p << " " << newplb << std::endl;

  dualquat dq = dualquat(quat(T), vec3(T[3]));
  newp = dq * p;
  std::cout << p << " " << newp << std::endl;

  T = makeMat4(dq);
  newp = T * p;
  std::cout << p << " " << newp << std::endl;

  return 0;
}
