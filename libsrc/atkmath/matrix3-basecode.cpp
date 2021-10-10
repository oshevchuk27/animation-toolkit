#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"

namespace atkmath {


Matrix3 computeRx(double alpha) {
	Matrix3 mRx;

	mRx[0][0] = 1;
	mRx[0][1] = 0;
	mRx[0][2] = 0;
	mRx[1][0] = 0;
	mRx[1][1] = cos(alpha);
	mRx[1][2] = -sin(alpha);
	mRx[2][0] = 0;
	mRx[2][1] = sin(alpha);
	mRx[2][2] = cos(alpha);

	return mRx;


}

Matrix3 computeRy(double theta) {
	Matrix3 mRy;

	mRy[0][0] = cos(theta);
	mRy[0][1] = 0;
	mRy[0][2] = sin(theta);
	mRy[1][0] = 0;
	mRy[1][1] = 1;
	mRy[1][2] = 0;
	mRy[2][0] = -sin(theta);
	mRy[2][1] = 0;
	mRy[2][2] = cos(theta);

	return mRy;

}

Matrix3 computeRz(double beta) {
	Matrix3 mRz;

	mRz[0][0] = cos(beta);
	mRz[0][1] = -sin(beta);
	mRz[0][2] = 0;
	mRz[1][0] = sin(beta);
	mRz[1][1] = cos(beta);
	mRz[1][2] = 0;
	mRz[2][0] = 0;
	mRz[2][1] = 0;
	mRz[2][2] = 1;

	return mRz;
}

Vector3 Matrix3::toEulerAnglesXYZ() const
{
	Matrix3 m_xyz = *this;

	double alpha, beta;
	double theta = asin(m_xyz[0][2]);

	if (theta == PI / 2 || theta == - PI / 2) {
		beta = 0;
		alpha = atan2(m_xyz[2][1], m_xyz[1][1]);
	}
	else {
		beta = -atan2(m_xyz[0][1], m_xyz[0][0]);
		alpha = -atan2(m_xyz[1][2], m_xyz[2][2]);
	}
   return Vector3(alpha, theta, beta);
}

Vector3 Matrix3::toEulerAnglesXZY() const
{
	Matrix3 m_xzy = *this;
	double beta = -asin(m_xzy[0][1]);
	double alpha, theta;

	if (beta == PI / 2 || beta == - PI / 2) {
		theta = 0;
		alpha = -atan2(m_xzy[1][2], m_xzy[2][2]);
	}

	else {
		alpha = atan2(m_xzy[2][1], m_xzy[1][1]);
		theta = atan2(m_xzy[0][2], m_xzy[0][0]);

	}
	
	return Vector3(alpha, theta, beta);
}

Vector3 Matrix3::toEulerAnglesYXZ() const
{
	Matrix3 m_yxz = *this;

	double alpha = -asin(m_yxz[1][2]);
	double beta, theta;

	if (alpha == PI / 2) {
		theta = 0;
		beta = atan2(m_yxz[2][0], m_yxz[2][1]);
	}
	else if (alpha == -PI / 2) {
		theta = 0;
		beta = -atan2(m_yxz[0][1], m_yxz[0][0]);
	}
	else {
		beta = atan2(m_yxz[1][0], m_yxz[1][1]);
		theta = atan2(m_yxz[0][2], m_yxz[2][2]);
	}
   return Vector3(alpha, theta, beta);
}

Vector3 Matrix3::toEulerAnglesYZX() const
{
	Matrix3 m_yzx = *this;

	double beta = asin(m_yzx[1][0]);
	double alpha, theta;

	if (beta == PI / 2 || beta == -PI / 2) {
		alpha = 0;
		theta = atan2(m_yzx[0][2], m_yzx[2][2]);
	}
	else {

		alpha = -atan2(m_yzx[1][2], m_yzx[1][1]);
		theta = -atan2(m_yzx[2][0], m_yzx[0][0]);

	}


   return Vector3(alpha, theta, beta);
}

Vector3 Matrix3::toEulerAnglesZXY() const
{
	Matrix3 m_zxy = *this;
	double alpha = asin(m_zxy[2][1]);
	double beta, theta;

	if (alpha == PI / 2) {
		beta = 0;
		theta = atan2(m_zxy[0][2], m_zxy[0][0]);

	}
	else if (alpha == -PI / 2) {
		beta = 0;
		theta = atan2(m_zxy[0][2], m_zxy[1][2]);

	}
	else {

		beta = -atan2(m_zxy[0][1], m_zxy[1][1]);
		theta = -atan2(m_zxy[2][0], m_zxy[2][2]);

	}

	
   return Vector3(alpha, theta, beta);
}

Vector3 Matrix3::toEulerAnglesZYX() const
{
	Matrix3 m_zyx = *this;

	double alpha, beta;

	double theta = - asin(m_zyx[2][0]);

	if (theta == PI / 2) {
		beta = 0;
		alpha = atan2(m_zyx[0][1], m_zyx[0][2]);
	}
	else if (theta == -PI / 2) {
		beta = 0;
		alpha = -atan2(m_zyx[0][1], m_zyx[1][1]);
	}
	else {

		beta = atan2(m_zyx[1][0], m_zyx[0][0]);
		alpha = atan2(m_zyx[2][1], m_zyx[2][2]);

	}
   return Vector3(alpha, theta, beta);
}

void Matrix3::fromEulerAnglesXYZ(const Vector3& angleRad)
{

   *this = computeRx(angleRad[0]) * computeRy(angleRad[1]) * computeRz(angleRad[2]);
}

void Matrix3::fromEulerAnglesXZY(const Vector3& angleRad)
{
   *this = computeRx(angleRad[0]) * computeRz(angleRad[2]) * computeRy(angleRad[1]);
}

void Matrix3::fromEulerAnglesYXZ(const Vector3& angleRad)
{
   *this = computeRy(angleRad[1]) * computeRx(angleRad[0]) * computeRz(angleRad[2]);
}

void Matrix3::fromEulerAnglesYZX(const Vector3& angleRad)
{
   *this = computeRy(angleRad[1]) * computeRz(angleRad[2]) * computeRx(angleRad[0]);
}

void Matrix3::fromEulerAnglesZXY(const Vector3& angleRad)
{
   *this = computeRz(angleRad[2]) * computeRx(angleRad[0]) * computeRy(angleRad[1]);
}

void Matrix3::fromEulerAnglesZYX(const Vector3& angleRad)
{

   *this = computeRz(angleRad[2]) * computeRy(angleRad[1]) * computeRx(angleRad[0]);
}


void Matrix3::toAxisAngle(Vector3& axis, double& angleRad) const
{
   // TODO
}

void Matrix3::fromAxisAngle(const Vector3& axis, double angleRad)
{
   // TODO
   *this = Identity;
}



}