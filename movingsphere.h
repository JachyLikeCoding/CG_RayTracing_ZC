#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H
#include "hitable.h"

class Movingsphere : public Hitable
{
public:
    Vec3f c0_, c1_;//始末球心位置
    float t0_, t1_;//始末时间戳
    float radius_;
    Material *m_ptr_;//材质

    Movingsphere(){}
    Movingsphere(Vec3f c0, Vec3f c1, float t0, float t1, float radius, Material *m_ptr):
                c0_(c0), c1_(c1), t0_(t0), t1_(t1), radius_(radius), m_ptr_(m_ptr){};
    Vec3f center(float t) const
    {
        return c0_ + (t - t0_)/(t1_ - t0_) * (c1_ - c0_);
    }
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
    AABB surrounding_box(AABB &box0, AABB &box1) const;
};


bool Movingsphere::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    Vec3f oc = ray.origin_ - center(ray.time_);//圆心与时间相关
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
            hitrecord.hitnormal_ = (hitrecord.hitpoint_ - center(ray.time_)) / radius_;
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
            hitrecord.hitnormal_ = (hitrecord.hitpoint_ - center(ray.time_)) / radius_;
            hitrecord.mtl_ptr_ = m_ptr_;
            return true;
        }
    }
    return false;
}

bool Movingsphere::bounding_box(float t0, float t1, AABB &box) const
{
    AABB box0(center(t0) - Vec3f(radius_,radius_,radius_),
                center(t0) + Vec3f(radius_,radius_,radius_));
    AABB box1(center(t1) - Vec3f(radius_,radius_,radius_),
                center(t1) + Vec3f(radius_,radius_,radius_));
    box = surrounding_box(box0, box1);
    return true;
}

//将t0时的盒子和t1时的盒子放进一个大盒子里
AABB Movingsphere::surrounding_box(AABB &box0, AABB &box1) const
{
    Vec3f boxmin(fmin(box0.getmin()[0], box1.getmin()[0]),
                fmin(box0.getmin()[1], box1.getmin()[1]),
                fmin(box0.getmin()[2], box1.getmin()[2]));
    Vec3f boxmax(fmax(box0.getmax()[0], box1.getmax()[0]),
            fmax(box0.getmax()[1], box1.getmax()[1]),
            fmax(box0.getmax()[2], box1.getmax()[2]));
    return AABB(boxmin, boxmax);
}
#endif