#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath {

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
{
	// TODO
	return Quaternion(1,0,0,0);
}

void Quaternion::toAxisAngle (Vector3& axis, double& angleRad) const
{
	angleRad = 2 * acos(mW);
	axis[0] = mX / (sin(angleRad / 2));
	axis[1] = mY / (sin(angleRad / 2));
	axis[2] = mZ / (sin(angleRad / 2));

}

void Quaternion::fromAxisAngle (const Vector3& axis, double angleRad)
{
	mX = sin(angleRad / 2) * axis[0];
	mY = sin(angleRad / 2) * axis[1];
	mZ = sin(angleRad / 2) * axis[2];
	mW = cos(angleRad / 2);
	normalize();
}

Matrix3 Quaternion::toMatrix () const
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
	float x = sqrt((1 / 4.0) * (1 + rot[0][0] - rot[1][1] - rot[2][2]));
}

}