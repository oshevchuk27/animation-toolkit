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
        float H03_u = 1 - 3.0f * (float)pow(u, 2) + 2.0f * (float)pow(u, 3);
        float H13_u = u * (float)pow(1 - u, 2);
        float H23_u = -1.0f * (float)pow(u, 2) + (float)pow(u, 3);
        float H33_u = 3.0f * (float)pow(u, 2) - 2.0f * (float)pow(u, 3);
        glm::vec3 p = getControlPoint(segment * 2) * H03_u + getControlPoint(segment * 2 + 1) * H13_u
            + getControlPoint(segment * 2 + 3) * H23_u + getControlPoint(segment * 2 + 2) * H33_u;
        return p;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
        // todo: your code here

        mCtrlPoints.clear();

        Eigen::MatrixXd A(keys.size(), keys.size());
        Eigen::MatrixXd p(keys.size(), 3);
        Eigen::MatrixXd pPrime(keys.size(), 3); // slopes for each control point

            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < keys.size(); j++) {
                    if (i == 0) {
                        if (mIsClamped) {
                            if (j == 0) {
                                A(i, j) = 1;
                            }
                            else {
                                A(i, j) = 0;
                            }

                        }
                        else {
                            if (j == 0) {
                                A(i, j) = 2;
                            }
                            else if (j == 1) {
                                A(i, j) = 1;
                            }
                            else {
                                A(i, j) = 0;
                            }
                        }
                        
                    }
                    else if (i == keys.size() - 1) {
                        if (mIsClamped) {
                            if (j == keys.size() - 1) {
                                A(i, j) = 1;
                            }
                            else {
                                A(i, j) = 0;
                            }

                        } else {
                            if (j == keys.size() - 1) {
                                A(i, j) = 2;
                            }
                            else if (j == keys.size() - 2) {
                                A(i, j) = 1;
                            }
                            else {
                                A(i, j) = 0;
                            }
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
                for (int j = 0; j < keys.size(); j++) {
                    std::cout << A(i, j) << std::endl;
                }
            }

            

            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < 3; j++) {
                    // hardcoding the first and last slopes
                    if (i == 0 || i == keys.size() - 1) {
                        if (mIsClamped) {
                            p(i, j) = mClampDir[j];
                        }
                        else {
                            if (i == 0) {
                                p(i, j) = (3.0f * (keys[i + 1] - keys[i]))[j];
                            }
                            else {
                                p(i, j) = (3.0f * (keys[i] - keys[i - 1]))[j];
                            }
                        }
                       
                    }
                    else {
                        p(i, j) = (3.0f * (keys[i + 1] - keys[i - 1]))[j];
                    }

                }

            }

            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < 3; j++) {
                    std::cout << "now p values" << p(i, j) << std::endl;
                }
            }


            pPrime = A.inverse() * p;

            for (int i = 0; i < keys.size(); i++) {
                mCtrlPoints.push_back(keys[i]);
                mCtrlPoints.push_back(glm::vec3(pPrime(i, 0), pPrime(i, 1), pPrime(i, 2)));
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
