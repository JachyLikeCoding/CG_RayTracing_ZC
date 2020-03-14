#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.h"
#include "texture.h"

class Lambertian : public Material
{
public:
    Vec3f albedo_; //衰减系数

    Lambertian(Vec3f albedo){ albedo_ = albedo; }
    //获得反射光线和衰减系数
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const;
};

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

bool Lambertian::scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const
{
    //step 1: 获得漫反射的反射光线
    Vec3f target = record.hitpoint_ + record.hitnormal_ + random_in_unit_sphere();
    //scattered = Ray(record.hitpoint_, target - record.hitpoint_);
    scattered = Ray(record.hitpoint_, target - record.hitpoint_, ray_in.time_);
    //step 2: 获得材料的衰减系数
    attenuation = albedo_;
    return true;
}

class Lambertian2 : public Material {
public:
    Texture *albedo_;   
    Lambertian2(Texture *a) : albedo_(a) {}
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const  {
        Vec3f target = record.hitpoint_ + record.hitnormal_ + random_in_unit_sphere();
        scattered = Ray(record.hitpoint_, target - record.hitpoint_);
        //scattered = Ray(record.hitpoint_, target - record.hitpoint_, ray_in.time_);
        attenuation = albedo_->value(record.u_, record.v_, record.hitpoint_);
        return true;
    }
};

#endif