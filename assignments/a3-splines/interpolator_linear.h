#ifndef _interpolator_linear_H_
#define _interpolator_linear_H_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Linear 
//--------------------------------

class InterpolatorLinear : public Interpolator
{
public:
    InterpolatorLinear() : Interpolator("Linear") {}
    virtual glm::vec3 interpolate(int segment, double u) const {

       // todo: your code here

        glm::vec3 p = getControlPoint(segment) * (float) (1 - u) + getControlPoint(segment + 1) * (float) u;

        return p;

    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {

        if (keys.size() < 2) {
            return;
        }
       
        mCtrlPoints.clear();
       // todo: your code here
        for (int i = 0; i < keys.size(); i++) {
            mCtrlPoints.push_back(keys[i]);
        }
       

    }
};

#endif
