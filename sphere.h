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

//取球形光源上的随机一点采样算法
Vec3f random_to_sphere(float radius, float distance2)
{
    float r1 = drand48();
    float r2 = drand48();
    float z = 1 + r2 * (sqrt(1 - radius*radius/distance2) - 1);
    float phi = 2 * PI * r1;
    float x = cos(phi) * sqrt(1 - z*z);
    float y = sin(phi) * sqrt(1 - z*z);
    return Vec3f(x, y, z);
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
    virtual ~Sphere(){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
    virtual float get_pdf_value(const Vec3f &o, const Vec3f &v) const;
    virtual Vec3f random(const Vec3f &o) const;
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

//对球形光源的随机采样以及求取概率密度函数的值
float Sphere::get_pdf_value(const Vec3f &o, const Vec3f &v) const
{
    HitRecord record;
    Ray *ray = new Ray(o, v);
    //TODO:CHECK RAY
    if(this->hit(*ray, 0.001, MAXFLOAT, record))
    {
        float cos_theta_max = sqrt(1 - radius_*radius_/((center_-o).length() * (center_-o).length()));
        //立体角
        float solid_angle = 2 * PI * (1-cos_theta_max);
        return 1/solid_angle;
    }
    else
    {
        return 0;
    }   
}

Vec3f Sphere::random(const Vec3f &o) const
{
    Vec3f direction = center_ - o;
    float distance2 = direction.length() * direction.length();
    ONB uvw;
    uvw.build_from_w(direction);
    return uvw.local(random_to_sphere(radius_, distance2));
}

#endif