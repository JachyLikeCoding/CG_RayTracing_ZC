#ifndef RAY_H
#define RAY_H

#include "basic.h"

class Ray
{
public:
    Vec3f origin_;
    Vec3f direction_;
    float time_;//光线时间戳

    Ray(){}
    Ray(const Vec3f &origin, const Vec3f &direction)
    {
        origin_ = origin;
        direction_ = direction;
        //direction_.normalize();
    }
    Ray(const Vec3f &origin, const Vec3f &direction, float time)
    {
        origin_ = origin;
        direction_ = direction;
        //direction_.normalize();
        time_ = time;
    }

    Vec3f get_origin() { return origin_; }
    Vec3f get_direction() { return direction_; }
    //返回光线位置
    Vec3f get_t_location(float t) {
        return origin_ + t * direction_;
    }
    float get_time() { return time_;}
};

#endif