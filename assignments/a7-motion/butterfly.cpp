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

      Joint* lUpperWing = new Joint("LUpperWing");
      lUpperWing->setLocalTranslation(vec3(0.1,0,0)*100.0f);
      skeleton.addJoint(lUpperWing, body);

      Joint* lLowerWing = new Joint("LLowerWing");
      lLowerWing->setLocalTranslation(vec3(0.1, 0, 0) * 100.0f);
      skeleton.addJoint(lLowerWing, body);

      Joint* rUpperWing = new Joint("RUpperWing");
      rUpperWing->setLocalTranslation(vec3(-0.1,0,0)*100.0f);
      skeleton.addJoint(rUpperWing, body);

      Joint* rLowerWing = new Joint("RLowerWing");
      rLowerWing->setLocalTranslation(vec3(-0.1, 0, 0) * 100.0f);
      skeleton.addJoint(rLowerWing, body);

      skeleton.fk();

      
   }

   void scene() {
      Joint* body = skeleton.getByName("Body");
      body->setLocalTranslation(vec3(150* cos(elapsedTime()), 250, 150*sin(elapsedTime())));


      Joint* lUpperWing = skeleton.getByName("LUpperWing");
      lUpperWing->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0,0,1)));


      Joint* lLowerWing = skeleton.getByName("LLowerWing");
      lLowerWing->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0, 0, 1)));


      Joint* rUpperWing = skeleton.getByName("RUpperWing");
      rUpperWing->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0,0,1))); 

      Joint* rLowerWing = skeleton.getByName("RLowerWing");
      rUpperWing->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0, 0, 1)));

      skeleton.fk();

      // attach geometry to skeleton 
      Transform B = body->getLocal2Global(); 
      Transform LUT = lUpperWing->getLocal2Global(); 
      Transform RUT = rUpperWing->getLocal2Global(); 
      Transform LLT = lLowerWing->getLocal2Global();
      Transform RLT = rUpperWing->getLocal2Global();

      

      // draw body
      Transform bodyGeometry(
         glm::angleAxis(glm::pi<float>()*0.5f, vec3(1,0,0)), // rotation
         vec3(0), vec3(25, 200, 25)); // position, scale

      Transform lUpperWingGeometry(
         eulerAngleRO(XYZ, vec3(0,2,0)),
         vec3(-80,0,50), 
         vec3(120,20,170));


      Transform lLowerWingGeometry(
          eulerAngleRO(XYZ, vec3(0, -2, 0)),
          vec3(-70, 0, -30),
          vec3(100, 20, 120));

      Transform rUpperWingGeometry(
         eulerAngleRO(XYZ, vec3(0,-2,0)),
         vec3(80,0,50), 
         vec3(120,20,170));


      Transform rLowerWingGeometry(
          eulerAngleRO(XYZ, vec3(0, 2, 0)),
          vec3(70, 0, -30),
          vec3(100, 20, 120));




      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(1));
      push();
      transform(B);

      if(drawAxes) {
          setColor(vec3(1, 0, 0));
          drawEllipsoid(vec3(-500, 0, 0), vec3(500, 0, 0), 10);
      
          setColor(vec3(0, 1, 0));
          drawEllipsoid(vec3(0, -500, 0), vec3(0, 500, 0), 10);
     
          setColor(vec3(0, 0, 1));
          drawEllipsoid(vec3(0, 0, -500), vec3(0, 0, 500), 10);
      }




      pop();


      push();


      setColor(vec3(1, 0, 1));
      transform(LUT * lUpperWingGeometry);
      drawSphere(vec3(0), 1);


      pop();


      push();



      setColor(vec3(1, 0, 1));
      transform(RUT * rUpperWingGeometry);
      drawSphere(vec3(0), 1);


      pop();

      push();


      setColor(vec3(0, 1, 1));
      transform(LLT * lLowerWingGeometry);
      drawSphere(vec3(0), 1);


      pop();


      push();


      setColor(vec3(0, 1, 1));
      transform(RLT * rLowerWingGeometry);
      drawSphere(vec3(0), 1);


      pop();



   }

   virtual void keyUp(int key, int mods) {

       if (key == '1') {
           drawAxes = !drawAxes;

       }

   }

private:
   Skeleton skeleton;
   bool drawAxes = false;
 

};


int main(int argc, char** argv) {
   Butterfly viewer;
   viewer.run();
}
