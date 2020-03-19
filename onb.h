#ifndef ONB_H
#define ONB_H
#include "basic.h"

//ortho-normal bases
class ONB
{
public:
    Vec3f axis_[3];//3个正交基向量

    ONB(){}
    inline Vec3f operator[](int i) const {return axis_[i];}
    inline Vec3f u() const {return axis_[0];}
    inline Vec3f v() const {return axis_[1];}
    inline Vec3f w() const {return axis_[2];}
    Vec3f local(const Vec3f &vec) const
    {
        return vec.x * u() + vec.y * v() + vec.z * w();
    }
    Vec3f local(float a, float b, float c) const
    {
        return a * u() + b * v() + c * w();
    }
    void build_from_w(const Vec3f &n);
};

void ONB::build_from_w(const Vec3f &n)
{
    Vec3f normal = n;
    axis_[2] = normal.normalize();//w = n
    Vec3f vup;
    if(fabs(w().x) > 0.9)//单位法向量n的x分量大于0.9，认为n//x
    {
        vup = Vec3f(0, 1, 0);
    }
    else
    {
        vup = Vec3f(1, 0, 0);
    }
    axis_[1] = (w().crossProduct(vup)).normalize();//v = vup x n
    axis_[0] = w().crossProduct(v());//u = w x v
}

#endif