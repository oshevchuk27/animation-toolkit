#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"
#include "agl/aglm.h"

//--------------------------------
// Hermite 
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1,0,0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       // todo: your code here
       return glm::vec3(0);
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
        // todo: your code here

        Eigen::MatrixXd A(keys.size(), keys.size());
        Eigen::MatrixXd p(keys.size(), 3);
        Eigen::MatrixXd pPrime(keys.size(), 3); // slopes for each control point

        if (mIsClamped) {
            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < keys.size(); j++) {
                    if (i == 0) {
                        if (j == 0) {
                            A(i, j) = 1;
                        }
                        else {
                            A(i, j) = 0;
                        }
                    }
                    else if (i == keys.size() - 1) {
                        if (j == keys.size() - 1) {
                            A(i, j) = 1;
                        }
                        else {
                            A(i, j) = 0;
                        }
                    }
                    else {
                        if (j == i - 1) {
                            A(i, j) = 1;
                        }
                        else if (j == i) {
                            A(i, j) = 4;
                        }
                        else if (j == i + 1) {
                            A(i, j) = 1;
                        }
                        else {
                            A(i, j) = 0;
                        }

                    }


                }
            }

            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < 3; j++) {
                    // hardcoding the first and last slopes
                    if (i == 0 || i == keys.size() - 1) {
                        p(i, j) = mClampDir[j];
                    }
                    else {
                        p(i, j) = (3.0f * (keys[i + 1] - keys[i - 1]))[j];
                    }

                }

            }



            pPrime = A.inverse() * p;

            for (int i = 0; i < keys.size(); i++) {
                mCtrlPoints.push_back(keys[i]);
                mCtrlPoints.push_back(glm::vec3(pPrime(i, 0), pPrime(i, 1), pPrime(i, 2)));
            }


        }

    }
        
       
    

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3& v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;
};

#endif
