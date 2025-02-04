#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

using namespace glm;

namespace atkmath {

	Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
	{
		float omega = acos(q0[0] * q1[0] + q0[1] * q1[1] + q0[2] * q1[2] + q0[3] * q1[3]);
		Quaternion q = (sin(omega * (1 - t)) / sin(omega)) * q0 + (sin(omega * t) / sin(omega)) * q1;
		return q;

	}

	void Quaternion::toAxisAngle(Vector3& axis, double& angleRad) const
	{
		angleRad = 2 * acos(mW);
		// edge case
		if (angleRad == 0) {
			axis = Vector3(1, 0, 0);
		}
		else {
			axis[0] = mX / (sin(angleRad / 2));
			axis[1] = mY / (sin(angleRad / 2));
			axis[2] = mZ / (sin(angleRad / 2));
		}


	}

	void Quaternion::fromAxisAngle(const Vector3& axis, double angleRad)
	{
		mX = sin(angleRad / 2) * axis[0];
		mY = sin(angleRad / 2) * axis[1];
		mZ = sin(angleRad / 2) * axis[2];
		mW = cos(angleRad / 2);
		normalize();
	}

	Matrix3 Quaternion::toMatrix() const
	{

		Matrix3 matrix;

		matrix[0][0] = 1 - 2 * (pow(mY, 2) + pow(mZ, 2));
		matrix[0][1] = 2 * (mX * mY - mW * mZ);
		matrix[0][2] = 2 * (mX * mZ + mW * mY);
		matrix[1][0] = 2 * (mX * mY + mW * mZ);
		matrix[1][1] = 1 - 2 * (pow(mX, 2) + pow(mZ, 2));
		matrix[1][2] = 2 * (mY * mZ - mW * mX);
		matrix[2][0] = 2 * (mX * mZ - mW * mY);
		matrix[2][1] = 2 * (mY * mZ + mW * mX);
		matrix[2][2] = 1 - 2 * (pow(mX, 2) + pow(mY, 2));



		return matrix;
	}

	void Quaternion::fromMatrix(const Matrix3& rot)
	{
		double xSquare = (1 / 4.0) * (1 + rot[0][0] - rot[1][1] - rot[2][2]);
		double ySquare = (1 / 4.0) * (1 - rot[0][0] + rot[1][1] - rot[2][2]);
		double zSquare = (1 / 4.0) * (1 - rot[0][0] - rot[1][1] + rot[2][2]);
		double wSquare = (1 / 4.0) * (rot[0][0] + rot[1][1] + rot[2][2] + 1);

		double max = 0;

		if (xSquare > ySquare && xSquare > zSquare && xSquare > wSquare) {
			max = xSquare;
		}
		else if (ySquare > xSquare && ySquare > zSquare && ySquare > wSquare) {
			max = ySquare;
		}
		else if (zSquare > xSquare && zSquare > ySquare && zSquare > wSquare) {
			max = zSquare;
		}
		else if (wSquare > xSquare && wSquare > ySquare && wSquare > zSquare) {
			max = wSquare;
		}

		if (xSquare == max) {
			mX = sqrt(xSquare);
			mW = (1 / (4 * mX)) * (rot[2][1] - rot[1][2]);
			mZ = (1 / (4 * mW)) * (rot[1][0] - rot[0][1]);
			mY = (1 / (4 * mW)) * (rot[0][2] - rot[2][0]);
		}
		else if (ySquare == max) {
			mY = sqrt(ySquare);
			mW = (1 / (4 * mY)) * (rot[0][2] - rot[2][0]);
			mX = 1 / (4 * mW) * (rot[2][1] - rot[1][2]);
			mZ = (1 / (4 * mW)) * (rot[1][0] - rot[0][1]);
		}
		else if (zSquare == max) {
			mZ = sqrt(zSquare);
			mW = (1 / (4 * mZ)) * (rot[1][0] - rot[0][1]);
			mX = (1 / (4 * mW)) * (rot[2][1] - rot[1][2]);
			mY = (1 / (4 * mW)) * (rot[0][2] - rot[2][0]);
		}
		else {
			mW = sqrt(wSquare);
			mX = (1 / (4 * mW)) * (rot[2][1] - rot[1][2]);
			mY = (1 / (4 * mW)) * (rot[0][2] - rot[2][0]);
			mZ = (1 / (4 * mW)) * (rot[1][0] - rot[0][1]);
		}

	}

}