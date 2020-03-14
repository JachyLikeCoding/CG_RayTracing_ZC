#ifndef SPHERE_H
#define SPHERE_H

#include "basic.h"
#include "object.h"
#include "hitable.h"
#include "material.h"

void get_sphere_uv(const Vec3f &p, float &u, float &v)
{
    float phi = atan2(p.z, p.x);    //phi ∈ [-PI,PI)
    float theta = asin(p.y);    //theta ∈ [-PI/2, PI/2)
    u = 1 - (phi + PI)/(2*PI);
    v = (theta + PI/2)/PI;
}

class Sphere: public Hitable
{
public:
    Vec3f center_;
    float radius_;
    Material *m_ptr_;
    Sphere(){};
    Sphere(Vec3f center, float radius, Material *m_ptr)
    {
        center_ = center;
        radius_ = radius;
        m_ptr_ = m_ptr;
    }
    ~Sphere();
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
};


bool Sphere::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    Vec3f oc = ray.origin_ - center_;
    float a = ray.direction_.dotProduct(ray.direction_);
    float b = oc.dotProduct(ray.direction_) * 2.0;
    float c = oc.dotProduct(oc) - radius_ * radius_;
    float delta = b*b - 4*a*c;
    if(delta > 0)
    {
        float t_temp = (-b - sqrt(delta)) / (2.0 * a);
        //优先考虑小根是否在范围内
        if(t_temp > t_min && t_temp < t_max)
        {
            hitrecord.t_ = t_temp;
            hitrecord.hitpoint_ = ray.get_t_location(t_temp);
            get_sphere_uv((hitrecord.hitpoint_ - center_)/radius_, hitrecord.u_, hitrecord.v_);
            hitrecord.hitnormal_ = (hitrecord.hitpoint_ - center_) / radius_;
            if(isnan(hitrecord.hitnormal_.x))
            {
                cout << "normal nan!!!" << endl;
            }
            hitrecord.mtl_ptr_ = m_ptr_;
            return true;
        }
        //小根不在范围在考虑大根是否在范围内
        t_temp = (-b + sqrt(delta)) / (2.0 * a);
        if(t_temp > t_min && t_temp < t_max)
        {
            hitrecord.t_ = t_temp;
            hitrecord.hitpoint_ = ray.get_t_location(t_temp);
            get_sphere_uv((hitrecord.hitpoint_ - center_)/radius_, hitrecord.u_, hitrecord.v_);
            hitrecord.hitnormal_ = (hitrecord.hitpoint_ - center_) / radius_;
            hitrecord.mtl_ptr_ = m_ptr_;
            return true;
        }
    }
    return false;
}

bool Sphere::bounding_box(float t0, float t1, AABB &box) const
{
    box = AABB(center_ - Vec3f(radius_,radius_,radius_),
                center_ + Vec3f(radius_,radius_,radius_));
    return true;
}

#endif