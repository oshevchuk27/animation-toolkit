#ifndef ACHARACTERDRAWER_H_
#define ACHARACTERDRAWER_H_

#include "atk/skeleton.h"
#include "agl/renderer.h"

class ASkeletonDrawer 
{
public:
    ASkeletonDrawer();
    virtual ~ASkeletonDrawer();

    void setColor(const glm::vec3& c);
    glm::vec3 getColor() const;

    void setJointRadius(float r);
    float getJointRadius() const;

    void setAxisLength(float r);
    float getAxisLength() const;

    void setShowAxis(bool b);
    bool getShowAxis() const;

    void setOffsetPosition(const glm::vec3& p);
    glm::vec3 getOffsetPosition() const;

    void setOffsetRotation(const glm::mat3& m);
    glm::mat3 getOffsetRotation() const;

    void setScale(float m);
    float getScale() const;

    virtual void draw(agl::Renderer& renderer, const atk::Skeleton& skeleton);

protected:

    virtual void drawJoint(agl::Renderer& renderer, const atk::Transform& globalTransform);
    virtual void drawBodyParts(agl::Renderer& renderer, atk::Joint* currentJoint);

protected:
    glm::vec3 _color;
    glm::vec3 _offsetPosition;
    glm::mat3 _offsetRotation;
    float _scale;
    float _axisLength;
    float _jointRadius;
    bool _drawAxes;

};

#endif
