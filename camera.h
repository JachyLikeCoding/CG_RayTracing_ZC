#ifndef CAMERA_H
#define CAMERA_H
#include "basic.h"
#include "ray.h"

class Camera
{
public:
    Vec3f origin_;
    Vec3f lower_left_corner_;
    Vec3f horizontal_;
    Vec3f vertical_;

    Camera()
    {
        origin_ = Vec3f(0.0, 0.0, 0.0);
        lower_left_corner_ = Vec3f(-2.0, -1.0, -1.0);
        horizontal_ = Vec3f(4.0, 0.0, 0.0);
        vertical_ = Vec3f(0.0, 2.0, 0.0);
    }

    Camera(Vec3f origin, Vec3f lower_left_corner, Vec3f horizontal, Vec3f vertical)
    {
        origin_ = origin;
        lower_left_corner_ = lower_left_corner;
        horizontal_ = horizontal;
        vertical_ = vertical;
    }

    Ray get_uv_ray(float u, float v)
    {
        return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }


};

#endif