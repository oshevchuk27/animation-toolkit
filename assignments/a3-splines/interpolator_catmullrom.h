#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       // todo: your code here
       // see: interpolator.h and writeup for details
        glm::vec3 p = getControlPoint(segment * 3) * (float)pow((float)(1 - u), 3) + getControlPoint(segment*3 + 1) * 3.0f * (float)u  * (float) pow((1 - u), 2) +
            getControlPoint(segment*3 + 2) * 3.0f * (float) pow(u, 2) * (float)(1 - u)  + getControlPoint(segment*3 + 3) * (float) pow(u, 3);
        return p;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       // todo: your code here
       // see: interpolator.h and writeup for details
        mCtrlPoints.clear();
        for (int i = 0; i < keys.size() - 1; i++) {
            if (i == 0) {
                mCtrlPoints.push_back(keys[i]);
            }
            if (i == 0) {
                mCtrlPoints.push_back(keys[i] + ((1.0f / 6) * (keys[i + 1] - keys[i])));
            }
            else {
                mCtrlPoints.push_back(keys[i] + ((1.0f / 6) * (keys[i + 1] - keys[i - 1])));
            }
            if (i == keys.size() - 2) {
                mCtrlPoints.push_back(keys[i + 1] - ((1.0f / 6) * (keys[i + 1] - keys[i])));
            }
            else {
                mCtrlPoints.push_back(keys[i + 1] - ((1.0f / 6) * (keys[i + 2] - keys[i])));
            }
            mCtrlPoints.push_back(keys[i + 1]);


        }
    }
};

#endif
