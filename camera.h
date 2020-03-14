#ifndef CAMERA_H
#define CAMERA_H
#include "basic.h"
#include "ray.h"

Vec3f random_in_unit_circle();
class Camera
{
public:
    Vec3f origin_;
    Vec3f lower_left_corner_;
    Vec3f horizontal_;
    Vec3f vertical_;
    Vec3f u, v, w;//相机坐标系
    float lens_radius_;//光圈半径
    float t0_, t1_;//按快门开始和结束时间

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

    /**
     * vfov:垂直范围的视野(角度)
     * aspect:宽高比
     * lookfrom:相机位置
     * lookat:观察点
     * up:相机的view up方向
    **/
    Camera(float vfov, float aspect, Vec3f lookfrom, Vec3f lookat, Vec3f up)
    {
        float theta = (vfov/180) * PI;//转成弧度
        float half_height = (float)tan(theta/2);
        float half_width = aspect * half_height;
        origin_ = lookfrom;
        w = (lookfrom - lookat).normalize();    //w-z
        u = up.crossProduct(w).normalize(); //u-x
        v = w.crossProduct(u).normalize();  //v-y
        lower_left_corner_ = origin_ - half_width * u - half_height * v - 1.0 * w;
        horizontal_ = 2 * half_width * u;
        vertical_ = 2 * half_height * v;
    }

    /**有运动模糊的相机
     * t0:开始时间
     * t1:结束时间
    **/
    Camera(float vfov, float aspect, Vec3f lookfrom, Vec3f lookat, Vec3f up, float aperture, float focus_distance)
    {
        float theta = (vfov/180) * PI;//转成弧度
        float half_height = (float)tan(theta/2);
        float half_width = aspect * half_height;
        origin_ = lookfrom;
        lens_radius_ = aperture / 2.0;
        w = (lookfrom - lookat).normalize();    //w-z
        u = up.crossProduct(w).normalize(); //u-x
        v = w.crossProduct(u).normalize();  //v-y
        lower_left_corner_ = origin_ - focus_distance * half_width * u - focus_distance * half_height * v - focus_distance * w;
        horizontal_ = 2 * half_width * focus_distance * u;
        vertical_ = 2 * half_height * focus_distance * v;
    }

    Camera(float vfov, float aspect, Vec3f lookfrom, Vec3f lookat, Vec3f up, float aperture, float focus_distance,
        float t0, float t1)
    {
        float theta = (vfov/180) * PI;//转成弧度
        float half_height = (float)tan(theta/2);
        float half_width = aspect * half_height;
        origin_ = lookfrom;
        lens_radius_ = aperture / 2.0;
        w = (lookfrom - lookat).normalize();    //w-z
        u = up.crossProduct(w).normalize(); //u-x
        v = w.crossProduct(u).normalize();  //v-y
        lower_left_corner_ = origin_ - focus_distance * half_width * u - focus_distance * half_height * v - focus_distance * w;
        horizontal_ = 2 * half_width * focus_distance * u;
        vertical_ = 2 * half_height * focus_distance * v;
        t0_ = t0;
        t1_ = t1;
    }

    Ray get_uv_ray(float u, float v)
    {
        Vec3f random_dir = random_in_unit_circle() * lens_radius_;
        Vec3f offset = random_dir.x * u + random_dir.y * v;
        float time = t0_ + (t1_ - t0_) * drand48();
        return Ray(origin_ + offset, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_ - offset, time);
    }
};

//在 z = 0 平面产生起点在原点，长度<1，方向随机的向量。因为相机的倾斜方向设置为（0，1，0），所以在 z = 0 平面取。
Vec3f random_in_unit_circle()
{
    Vec3f dir;
    do{
        //(-1, 1)随机数
        dir = 2.0*Vec3f((rand()%(10000)/(float)(10000)), (rand()%(10000)/(float)(10000)), 0) - Vec3f(1,1,0);
    }while(dir.length() >= 1.0);
    return dir;
}

#endif