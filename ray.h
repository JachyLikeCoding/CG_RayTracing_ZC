#ifndef RAY_H
#define RAY_H

#include "basic.h"

class Ray
{
public:
    Vec3f origin_;
    Vec3f direction_;
    Ray(){}
    Ray(const Vec3f &origin, const Vec3f &direction)
    {
        origin_ = origin;
        direction_ = direction;
        //direction_.normalize();
    }

    Vec3f get_origin() { return origin_; }
    Vec3f get_direction() { return direction_; }
    Vec3f get_t_location(float t) {
        return origin_ + t * direction_;
    }
};

#endif