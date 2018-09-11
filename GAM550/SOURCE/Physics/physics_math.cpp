

#include "physics_math.h"

using namespace physics;

const Vector3D Vector3D::GRAVITY = Vector3D(0, -9.81f, 0);
const Vector3D Vector3D::HIGH_GRAVITY = Vector3D(0, -19.62f, 0);
const Vector3D Vector3D::UP = Vector3D(0, 1, 0);
const Vector3D Vector3D::RIGHT = Vector3D(1, 0, 0);
const Vector3D Vector3D::OUT_OF_SCREEN = Vector3D(0, 0, 1);
const Vector3D Vector3D::X = Vector3D(0, 1, 0);
const Vector3D Vector3D::Y = Vector3D(1, 0, 0);
const Vector3D Vector3D::Z = Vector3D(0, 0, 1);

/*
 * Definition of the sleep epsilon extern.
 */
real physics::sleepEpsilon = ((real)0.3);

/*
 * Functions to change sleepEpsilon.
 */
void physics::setSleepEpsilon(real value)
{
    physics::sleepEpsilon = value;
}

real physics::getSleepEpsilon()
{
    return physics::sleepEpsilon;
}

real Matrix4::getDeterminant() const
{
    return -data[8]*data[5]*data[2]+
        data[4]*data[9]*data[2]+
        data[8]*data[1]*data[6]-
        data[0]*data[9]*data[6]-
        data[4]*data[1]*data[10]+
        data[0]*data[5]*data[10];
}

void Matrix4::setInverse(const Matrix4 &m)
{
    // Make sure the determinant is non-zero.
    real det = getDeterminant();
    if (det == 0) return;
    det = ((real)1.0)/det;

    data[0] = (-m.data[9]*m.data[6]+m.data[5]*m.data[10])*det;
    data[4] = (m.data[8]*m.data[6]-m.data[4]*m.data[10])*det;
    data[8] = (-m.data[8]*m.data[5]+m.data[4]*m.data[9])*det;

    data[1] = (m.data[9]*m.data[2]-m.data[1]*m.data[10])*det;
    data[5] = (-m.data[8]*m.data[2]+m.data[0]*m.data[10])*det;
    data[9] = (m.data[8]*m.data[1]-m.data[0]*m.data[9])*det;

    data[2] = (-m.data[5]*m.data[2]+m.data[1]*m.data[6])*det;
    data[6] = (+m.data[4]*m.data[2]-m.data[0]*m.data[6])*det;
    data[10] = (-m.data[4]*m.data[1]+m.data[0]*m.data[5])*det;

    data[3] = (m.data[9]*m.data[6]*m.data[3]
               -m.data[5]*m.data[10]*m.data[3]
               -m.data[9]*m.data[2]*m.data[7]
               +m.data[1]*m.data[10]*m.data[7]
               +m.data[5]*m.data[2]*m.data[11]
               -m.data[1]*m.data[6]*m.data[11])*det;
    data[7] = (-m.data[8]*m.data[6]*m.data[3]
               +m.data[4]*m.data[10]*m.data[3]
               +m.data[8]*m.data[2]*m.data[7]
               -m.data[0]*m.data[10]*m.data[7]
               -m.data[4]*m.data[2]*m.data[11]
               +m.data[0]*m.data[6]*m.data[11])*det;
    data[11] =(m.data[8]*m.data[5]*m.data[3]
               -m.data[4]*m.data[9]*m.data[3]
               -m.data[8]*m.data[1]*m.data[7]
               +m.data[0]*m.data[9]*m.data[7]
               +m.data[4]*m.data[1]*m.data[11]
               -m.data[0]*m.data[5]*m.data[11])*det;
}

Matrix3 Matrix3::linearInterpolate(const Matrix3& a, const Matrix3& b, real prop)
{
    Matrix3 result;
    for (unsigned i = 0; i < 9; i++) {
        result.data[i] = a.data[i] * (1-prop) + b.data[i] * prop;
    }
    return result;
}
