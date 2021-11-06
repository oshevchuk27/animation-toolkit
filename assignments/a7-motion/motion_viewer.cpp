#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective) {
   }


   void setFileName(std::string filename) {
       filename = filename;
       BVHReader reader;
       reader.load(filename, skeleton, motion);

       motion.update(skeleton, 0);
   }

   void setup() {

       
      
   }

   void scene() {
     

      if (!paused) {

          time += dt() * timeScale;
          motion.update(skeleton, time);
          currentFrame = motion.getKeyID(time);
          
      }
      

      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      drawText(paused? "Paused" : "Playing", 10, 15);
      drawText("Current frame: "+std::to_string(currentFrame), 10, 35);
      drawText("Time scale: "+std::to_string(timeScale), 10, 55);
   }

   virtual void keyUp(int key, int mods) {
       if (key == 'P') {
           paused = !paused;
       }
       else if (key == '0') {
           time = 0;
           currentFrame = 0;
           skeleton.setPose(motion.getKey(currentFrame));
       }
       else if (key == '.') {
           if (paused) {
               currentFrame = (currentFrame + 1) % motion.getNumKeys();
               skeleton.setPose(motion.getKey(currentFrame));
           }
       }
       else if (key == ',') {
           if (paused) {
               currentFrame = (currentFrame - 1 + motion.getNumKeys()) % motion.getNumKeys();
               skeleton.setPose(motion.getKey(currentFrame));
           }
       }
       else if (key == ']') {
           timeScale += 0.1;

       }
       else if (key == '[') {
           if (timeScale > 0) {
               timeScale -= 0.1;
           }
          
         
       }
   } 

private:
   Skeleton skeleton;
   Motion motion;
   std::string filename;

   float timeScale = 1.0f;
   int currentFrame = 0; 
   bool paused = false;
   float time = 0;
   bool setToZero = false;

};


int main(int argc, char** argv) {

    MotionViewer viewer;
 
   if (argc == 1) {
       viewer.setFileName("../motions/Beta/jump.bvh");
   }
   else {
       viewer.setFileName(argv[1]);
   }

   viewer.run();
}
