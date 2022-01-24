#include "ASkeletonDrawer.h"
#include <algorithm>
#include <iostream>

using glm::vec3;
using glm::mat3;
using namespace atk;
using namespace agl;

const vec3 LIGHT_BLUE(0.459f, 0.729f, 1.0f);
const vec3 DARK_BLUE(0.0510, 0.1255, 0.4627);
const vec3 DARK_YELLOW(0.9922, 0.8392, 0.1725);
const vec3 LIGHT_RED(1.0, 0.5, 0.5);
const vec3 LIGHT_GREEN(0.0, 1.0, 0.25);

void ADrawLimb(Renderer& renderer, const vec3& startPosition, const vec3& endPosition, float radius)
{
    vec3 direction = endPosition - startPosition; 
    float length = glm::length(direction);
    direction /= length;
    float x = direction[0] * direction[0];
    float y = direction[1] * direction[1];
    float z = direction[2] * direction[2];
    float dx = y + z;
    float dy = x + z;
    float dz = x + y;
    unsigned int eAxis;
    vec3 axis;
    float angle;
    if (dx >= dy && dx >= dz)
    {
        eAxis = 0;
        axis = glm::cross(vec3(1, 0, 0), direction);
        angle = acos(glm::dot(direction, vec3(1, 0, 0)));
    }
    else if (dy >= dx && dy >= dz)
    {
        eAxis = 1;
        axis = glm::cross(vec3(0, 1, 0), direction);
        angle = acos(glm::dot(direction, vec3(0, 1, 0)));
    }
    else
    {
        eAxis = 2;
        axis = glm::cross(vec3(0, 0, 1), direction);
        angle = acos(glm::dot(direction, vec3(0, 0, 1)));
    }
    vec3 center = (endPosition + startPosition) / 2.0f;
    vec3 scale;
    switch(eAxis)
    {
    case 0: scale = vec3(length, radius, radius); break;
    case 1: scale = vec3(radius, length, radius); break;
    case 2: scale = vec3(radius, radius, length); break;
    }

    renderer.push();
    renderer.translate(center);
    renderer.rotate(angle, axis);
    renderer.scale(scale);
    renderer.sphere();
    renderer.pop();
}

ASkeletonDrawer::ASkeletonDrawer() :
    _color(LIGHT_BLUE),
    _offsetPosition(vec3(0)),
    _offsetRotation(mat3(0)),
    _scale(1.0),
    _axisLength(10.0f),
    _jointRadius(5.0f),
    _drawAxes(false)
{
}

ASkeletonDrawer::~ASkeletonDrawer()
{
}

void ASkeletonDrawer::setColor(const vec3& c)
{
    _color = c;
}

vec3 ASkeletonDrawer::getColor() const
{
    return _color;
}


void ASkeletonDrawer::setJointRadius(float r)
{
    _jointRadius = r;
}

float ASkeletonDrawer::getJointRadius() const
{
    return _jointRadius;
}


void ASkeletonDrawer::setAxisLength(float r)
{
    _jointRadius = r;
}

float ASkeletonDrawer::getAxisLength() const
{
    return _axisLength;
}

void ASkeletonDrawer::setShowAxis(bool b)
{
    _drawAxes = b;
}

bool ASkeletonDrawer::getShowAxis() const
{
    return _drawAxes;
}

void ASkeletonDrawer::setOffsetPosition(const vec3& p)
{
    _offsetPosition = p;
}

vec3 ASkeletonDrawer::getOffsetPosition() const
{
    return _offsetPosition;
}

void ASkeletonDrawer::setOffsetRotation(const mat3& m)
{
    _offsetRotation = m;
}

mat3 ASkeletonDrawer::getOffsetRotation() const
{
    return _offsetRotation;
}

void ASkeletonDrawer::setScale(float m)
{
    _scale = m;
}

float ASkeletonDrawer::getScale() const
{
    return _scale;
}

void ASkeletonDrawer::draw(Renderer& renderer, const Skeleton& skeleton)
{
    renderer.setUniform("Material.diffuse", _color);

    renderer.push();
    renderer.translate(_offsetPosition);
    //renderer.rotate(_offsetRotation);
    renderer.scale(vec3(_scale));

    if (skeleton.getNumJoints() > 0)
    {
        Joint* pJoint = skeleton.getRoot();
        drawJoint(renderer, pJoint->getLocal2Global());

        int totalChildren = pJoint->getNumChildren();
        for (int i = 0; i < totalChildren; i++)
            drawBodyParts(renderer, pJoint->getChildAt(i));
    }
    renderer.pop();
}

void ASkeletonDrawer::drawBodyParts(Renderer& renderer, Joint* currentJoint)
{
    drawJoint(renderer, currentJoint->getLocal2Global());    

    Joint* pParent = currentJoint->getParent();
    ADrawLimb(renderer, pParent->getGlobalTranslation(), 
        currentJoint->getGlobalTranslation(), _jointRadius);

    // don't draw hands
    if (currentJoint->getName().find("Hand") != std::string::npos) return; 
   
    int totalChildren = currentJoint->getNumChildren();
    for (int i = 0; i < totalChildren; i++)
        drawBodyParts(renderer, currentJoint->getChildAt(i));
}

void ASkeletonDrawer::drawJoint(Renderer& renderer, const Transform& globalTransform)
{
    const vec3& globalPosition = globalTransform.t();
    const mat3& globalRotation = mat3(globalTransform.r());

    renderer.push();
    renderer.translate(globalPosition);
    renderer.scale(vec3(_jointRadius));
    renderer.sphere();
    renderer.pop();

    if (_drawAxes) // Draw axes
    {
        vec3 x = globalRotation * _axisLength * vec3(1,0,0);
        vec3 y = globalRotation * _axisLength * vec3(0,1,0);
        vec3 z = globalRotation * _axisLength * vec3(0,0,1);

        vec3 red(1.0f,0.0f,0.0f);
        renderer.line(globalPosition, globalPosition + x, red, red);

        vec3 green(0.0f,1.0f,0.0f);
        renderer.line(globalPosition, globalPosition + y, green, green);

        vec3 blue(0.0f,0.0f,1.0f);
        renderer.line(globalPosition, globalPosition + z, blue, blue);
    }
}
