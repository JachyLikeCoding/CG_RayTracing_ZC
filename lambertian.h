#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "texture.h"
#include "onb.h"
Vec3f random_in_unit_sphere()
{
    Vec3f p;
    do{
        //产生（-1，1）之间的随机数，采取接受-拒绝方式进而产生单位球中的点
        p = 2.0 * Vec3f((rand()%(100)/(float)(100)),
                        (rand()%(100)/(float)(100)),
                        (rand()%(100)/(float)(100))) - Vec3f(1, 1, 1);
    }while(p.length() >= 1.0);
    return p;
}

inline Vec3f random_cos_direction()
{
    float r1 = drand48();
    float r2 = drand48();
    float z = sqrt(1 - r2);
    float phi = 2 * PI * r1;
    float x = 2 * cos(phi) * sqrt(r2);
    float y = 2 * sin(phi) * sqrt(r2);
    return Vec3f(x, y, z);
}

/*
class Lambertian : public Material
{
public:
    Vec3f albedo_; //衰减系数

    Lambertian(Vec3f albedo){ albedo_ = albedo; }
    //获得反射光线和衰减系数
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const;
};

bool Lambertian::scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const
{
    //step 1: 获得漫反射的反射光线
    Vec3f target = record.hitpoint_ + record.hitnormal_ + random_in_unit_sphere();
    //scattered = Ray(record.hitpoint_, target - record.hitpoint_);
    scattered = Ray(record.hitpoint_, target - record.hitpoint_, ray_in.time_);
    //step 2: 获得材料的衰减系数
    attenuation = albedo_;
    return true;
}*/

class Lambertian2 : public Material {
public:
    Texture *albedo_;   
    Lambertian2(Texture *a) : albedo_(a) {}
    // virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered, float &pdf) const  {
    //     Vec3f target = record.hitpoint_ + record.hitnormal_ + random_in_unit_sphere();
    //     //scattered = Ray(record.hitpoint_, target - record.hitpoint_);
    //     scattered = Ray(record.hitpoint_, (target - record.hitpoint_).normalize(), ray_in.time_);
    //     attenuation = albedo_->value(record.u_, record.v_, record.hitpoint_);
    //     pdf = record.hitnormal_.dotProduct(scattered.direction_)/PI;
    //     return true;
    // }
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &albedo, Ray &scattered, float &pdf) const  
    {
        ONB uvw;
        uvw.build_from_w(record.hitnormal_);
        do{
            Vec3f direction = uvw.local(random_cos_direction());
            scattered = Ray(record.hitpoint_, direction.normalize(), ray_in.time_);
            pdf = uvw.w().dotProduct(scattered.direction_)/PI;
        }while(pdf - 0.0 < kEpsilon);
        albedo = albedo_->value(record.u_, record.v_, record.hitpoint_);
        return true;
    }
    virtual float scatter_pdf(Ray &ray_in, HitRecord &record, Ray &scattered) const
    {
        float cos = record.hitnormal_.dotProduct(scattered.direction_.normalize());
        //cosine值是散射光线和表面法线的夹角余弦，角大于90°无效，表示采样失败
        if(cos < 0)
        {
            return 0;
        }
        return cos / PI;
    }
};

#endif