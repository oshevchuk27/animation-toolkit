#include <iostream>
#include "atk/toolkit.h"
#include "atkmath/quaternion.h"
#include <glm/mat3x3.hpp>             // mat3, dmat3
#include "atkmath/matrix3.h"

using namespace atk;
using namespace glm;

using atkmath::Quaternion;

int main(int argc, char** argv)
{
   Skeleton skeleton;
   Motion motion;

   BVHReader reader;
   reader.load("../motions/Beta/walking.bvh", skeleton, motion);

   // How many frames (aka keys)?
   int numFrames = motion.getNumKeys();
   std::cout << "The number of frames is " << numFrames << std::endl;

   // What is the framerate?
   float fps = motion.getFramerate();
   std::cout << "The frame rate is " << fps << std::endl;

   // What is the duration?
   float duration = motion.getDuration();
   std::cout << "The duration of the animation is " << duration << std::endl;

   // What is the root position on frame 5 (aka Key #5)?
   // Hint: Get a pose from the motion
   Pose frame5 = motion.getKey(5);
   vec3 rootPos = frame5.rootPos;
   std::cout << "The root position on frame 5 is: " << glm::to_string(rootPos) << std::endl;

   // What is the local joint rotation of the left hand on frame 5? 
   // Give your answer in XYZ euler angles
   // Hint: use the skeleton to get the ID for the hand
   // Hint: use extractEulerAngleRO (defined in atk/glmmath.h)
   Joint* hand = skeleton.getByName("Beta:LeftHand");
   int id = hand->getID();
   quat quaternion = frame5.jointRots[hand->getID()];

   std::cout << glm::to_string(quaternion) << std::endl;

   Quaternion q = Quaternion(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
   atkmath::Matrix3 matrix = q.toMatrix();

   vec3 column1 = vec3(matrix[0][0], matrix[1][0], matrix[2][0]);
   vec3 column2 = vec3(matrix[0][1], matrix[1][1], matrix[2][1]);
   vec3 column3 = vec3(matrix[0][1], matrix[1][2], matrix[2][2]);



   glm::mat3 new_matrix = mat3(column1, column2, column3);
  
   
   vec3 eulerXYZ = extractEulerAngleRO(RotOrder::XYZ, new_matrix);
   std::cout << "The local rotation of the left hand on frame 5 is: " << glm::to_string(eulerXYZ) << std::endl;

   // What is the speed of the character?
   // Hint: Use the first and last frames to get the distance traveled over the motion clip
   Pose frame0 = motion.getKey(0);
   Pose frameN = motion.getKey(motion.getNumKeys() - 1);
   vec3 delta = frameN.rootPos - frame0.rootPos;
   float distance = delta.length();
   float speed = distance/ duration;
   std::cout << "The speed of the motion is (units/s): " << speed << std::endl;
}

