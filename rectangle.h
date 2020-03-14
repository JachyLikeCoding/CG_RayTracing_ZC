#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "hitable.h"

class XY_rectangle : public Hitable
{
public:
    float x0_, y0_, x1_, y1_;
    float k_;
    Material *mtl_ptr_;

    XY_rectangle(){}
    XY_rectangle(float x0, float x1, float y0, float y1, float k, Material *mtl_ptr):
        x0_(x0), y0_(y0), x1_(x1), y1_(y1), k_(k), mtl_ptr_(mtl_ptr){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3f(x0_, y0_, k_-0.0001), Vec3f(x1_, y1_, k_+0.0001));
        return true;
    }
};

bool XY_rectangle::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    
    return true;
}

#endif