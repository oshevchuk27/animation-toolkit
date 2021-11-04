#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      Joint* body = new Joint("Body");
      body->setLocalTranslation(vec3(1,2,1)*100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0,1,0)));
      skeleton.addJoint(body);

      Joint* lwing = new Joint("LWing");
      lwing->setLocalTranslation(vec3(0.1,0,0)*100.0f);
      skeleton.addJoint(lwing, body);

      Joint* rwing = new Joint("RWing");
      rwing->setLocalTranslation(vec3(-0.1,0,0)*100.0f);
      skeleton.addJoint(rwing, body);

      skeleton.fk();

      drawX = false;
      drawY = false;
      drawZ = false;
   }

   void scene() {
      Joint* body = skeleton.getByName("Body");
      Joint* lwing = skeleton.getByName("LWing");
      lwing->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0,0,1)));

      Joint* rwing = skeleton.getByName("RWing");
      rwing->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0,0,1))); 
      skeleton.fk();

      // attach geometry to skeleton 
      Transform B = body->getLocal2Global(); 
      Transform LT = lwing->getLocal2Global(); 
      Transform RT = rwing->getLocal2Global(); 

      // draw body
      Transform bodyGeometry(
         glm::angleAxis(glm::pi<float>()*0.5f, vec3(1,0,0)), // rotation
         vec3(0), vec3(25, 200, 25)); // position, scale

      Transform lwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,0), 
         vec3(120,20,200));

      Transform rwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,0), 
         vec3(120,20,200));

      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(1));
      push();
      transform(B);

      if (drawX && !(drawY) && !(drawZ)) {
          setColor(vec3(1, 0, 0));
          drawEllipsoid(vec3(-100, 0, 0), vec3(100, 0, 0), 10);
      }
      else if (drawY && !(drawX) && !(drawZ)) {
          setColor(vec3(0, 1, 0));
          drawEllipsoid(vec3(0, -100, 0), vec3(0, 100, 0), 10);
      }
      else if (drawZ && !(drawX) && !(drawY)) {
          setColor(vec3(0, 0, 1));
          drawEllipsoid(vec3(0, 0, -500), vec3(0, 0, 500), 10);
      }

      pop();


      setColor(vec3(0.8, 0, 0.0));
      push();
      transform(LT * lwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RT * rwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      

   }

   virtual void keyUp(int key, int mods) {

       if (key == '1') {
           drawX = true;
           drawY = false;
           drawZ = false;

       }
       else if (key == '2') {
           drawY = true;
           drawX = false;
           drawZ = false;
       }
       else if (key == '3') {
           drawZ = true;
           drawX = false;
           drawY = false;
       }

   }

private:
   Skeleton skeleton;
   bool drawX;
   bool drawY;
   bool drawZ;

};


int main(int argc, char** argv) {
   Butterfly viewer;
   viewer.run();
}
