#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "hitable.h"

class XY_rectangle : public Hitable
{
public:
    float x0_, y0_, x1_, y1_;
    float k_;
    Material *m_ptr_;

    XY_rectangle(){}
    XY_rectangle(float x0, float x1, float y0, float y1, float k, Material *mtl_ptr):
        x0_(x0), y0_(y0), x1_(x1), y1_(y1), k_(k), m_ptr_(mtl_ptr){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3f(x0_, y0_, k_-0.0001), Vec3f(x1_, y1_, k_+0.0001));
        return true;
    }
};

bool XY_rectangle::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    /**
     * p(t) = a + t*b
     * when xy plane, z=k:
     * k = a.z + t*b.z
     * => t = (k - a.z) / b.z
     * */
    float t = (k_ - ray.origin_.z) / ray.direction_.z;
    if(t < t_min || t > t_max)
    {
        return false;
    }
    //p(t) = a + t*b
    float x = ray.origin_.x + t * ray.direction_.x;
    float y = ray.origin_.y + t * ray.direction_.y;
    if(x < x0_ || x > x1_ || y < y0_ || y > y1_)
    {
        return false;
    }
    hitrecord.hitpoint_ = ray.get_t_location(t);
    hitrecord.hitnormal_ = Vec3f(0.0, 0.0, 1.0);
    hitrecord.u_ = (x - x0_) / (x1_ - x0_);
    hitrecord.v_ = (y - y0_) / (y1_ - y0_);
    hitrecord.t_ = t;
    hitrecord.mtl_ptr_ = m_ptr_;
    return true;
}

class XZ_rectangle : public Hitable
{
public:
    float x0_, z0_, x1_, z1_;
    float k_;
    Material *m_ptr_;

    XZ_rectangle(){}
    XZ_rectangle(float x0, float x1, float z0, float z1, float k, Material *mtl_ptr):
        x0_(x0), z0_(z0), x1_(x1), z1_(z1), k_(k), m_ptr_(mtl_ptr){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3f(x0_, k_-0.0001, z0_), Vec3f(x1_, k_+0.0001, z1_));
        return true;
    }
};

bool XZ_rectangle::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    float t = (k_ - ray.origin_.y) / ray.direction_.y;
    if(t < t_min || t > t_max)
    {
        return false;
    }
    //p(t) = a + t*b
    float x = ray.origin_.x + t * ray.direction_.x;
    float z = ray.origin_.z + t * ray.direction_.z;
    if(x < x0_ || x > x1_ || z < z0_ || z > z1_)
    {
        return false;
    }
    hitrecord.hitpoint_ = ray.get_t_location(t);
    hitrecord.hitnormal_ = Vec3f(0.0, 1.0, 0.0);
    hitrecord.u_ = (x - x0_) / (x1_ - x0_);
    hitrecord.v_ = (z - z0_) / (z1_ - z0_);
    hitrecord.t_ = t;
    hitrecord.mtl_ptr_ = m_ptr_;
    return true;
}


class YZ_rectangle : public Hitable
{
public:
    float y0_, z0_, y1_, z1_;
    float k_;
    Material *m_ptr_;

    YZ_rectangle(){}
    YZ_rectangle(float y0, float y1, float z0, float z1, float k, Material *mtl_ptr):
        y0_(y0), z0_(z0), y1_(y1), z1_(z1), k_(k), m_ptr_(mtl_ptr){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(Vec3f(k_-0.0001, y0_, z0_), Vec3f(k_+0.0001, y1_, z1_));
        return true;
    }
};

bool YZ_rectangle::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    float t = (k_ - ray.origin_.x) / ray.direction_.x;
    if(t < t_min || t > t_max)
    {
        return false;
    }
    //p(t) = a + t*b
    float y = ray.origin_.y + t * ray.direction_.y;
    float z = ray.origin_.z + t * ray.direction_.z;
    if(y < y0_ || y > y1_ || z < z0_ || z > z1_)
    {
        return false;
    }
    hitrecord.hitpoint_ = ray.get_t_location(t);
    hitrecord.hitnormal_ = Vec3f(1.0, 0.0, 0.0);
    hitrecord.u_ = (y - y0_) / (y1_ - y0_);
    hitrecord.v_ = (z - z0_) / (z1_ - z0_);
    hitrecord.t_ = t;
    hitrecord.mtl_ptr_ = m_ptr_;
    return true;
}

//翻转法向量
class Turn_normal : public Hitable
{
public:
    Hitable *ptr_;
    Turn_normal(Hitable *ptr):ptr_(ptr){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
    {
        if(ptr_->hit(ray, t_min, t_max, hitrecord))
        {
            hitrecord.hitnormal_ = -hitrecord.hitnormal_;
            return true;
        }
        else
        {
            return false;
        }   
    }

    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        return ptr_->bounding_box(t0, t1, box);
    } 
};

#endif