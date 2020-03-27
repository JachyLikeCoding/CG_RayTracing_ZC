#ifndef AABB_H
#define AABB_H

#include "basic.h"
#include "ray.h"

//AABB包围盒
class AABB
{
public:
    Vec3f min_;
    Vec3f max_;

    AABB(){}
    AABB(Vec3f min, Vec3f max) : min_(min), max_(max){}
    Vec3f getmin(){ return min_; }
    Vec3f getmax(){ return max_; }

    //左大右小(左区间区max，右区间取min，比较2个值，如果左<右，为真)发生重叠。
    inline bool hit(float t_min, float t_max, Ray &ray) const
    {
        Vec3f origin = ray.origin_;
        Vec3f dir = ray.direction_;
        for(int i = 0; i < 3; i++)
        {
            //t = [p(t)-origin] / direction
            float invdir = 1.0f / dir[i];
            float t0 = (min_[i] - origin[i]) * invdir;
            float t1 = (max_[i] - origin[i]) * invdir;
            if(invdir < 0.0f)
            {
                std::swap(t0, t1);
            }
            t_min = t0 > t_min ? t0 : t_min;
            t_min = t1 < t_max ? t1 : t_max;
            if(t_max <= t_min)
            {
                return false;
            }
        }
        return true;
    }

};

#endif