#ifndef HITABLE_H
#define HITABLE_H

#include "basic.h"
#include "ray.h"
#include "aabb.h"

class Material;

//记录碰撞点信息
struct HitRecord
{
    Vec3f hitpoint_;
    Vec3f hitnormal_;
    float t_;//ray的t
    Material *mtl_ptr_;//记录所碰到物体的材质
    float u_;//平面坐标的uv映射
    float v_;
};

//抽象类：表示能被光线碰撞到的任何物体
class Hitable
{
public:
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const = 0;
    virtual bool bounding_box(float t0, float t1, AABB &box) const = 0;
};

#endif