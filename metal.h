#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "lambertian.h"

//计算反射光线的方向向量
Vec3f reflect_dir(Vec3f &in, Vec3f &normal)
{
    return in - 2 * in.dotProduct(normal) * normal;
}

//----------------------------------------------------------------------------metal
class Metal : public Material
{
public:
    Vec3f albedo_;//衰减系数
    float fuzz_;//添加的模糊程度

    Metal(Vec3f albedo, float fuzz) : albedo_(albedo)
    {
        if(fuzz < 1) 
            fuzz_ = fuzz;
        else 
            fuzz_ = 1;
        is_important_sample = false;
    }

    Metal(string name, Vec3f albedo, float fuzz) : albedo_(albedo)
    {
        material_name_ = name;
        if(fuzz < 1) 
            fuzz_ = fuzz;
        else 
            fuzz_ = 1;
        is_important_sample = false;
    }
    
    virtual ~Metal(){}
    virtual bool scatter(Ray &ray_in, HitRecord &hitrecord, ScatterRecord &srecord) const;
};

bool Metal::scatter(Ray &ray_in, HitRecord &hitrecord, ScatterRecord &srecord) const
{
    //step 1: 获得镜面反射的反射光线
    Vec3f reflected = reflect_dir(ray_in.direction_.normalize(), hitrecord.hitnormal_);
    srecord.specular_ray_ = Ray(hitrecord.hitpoint_, reflected + fuzz_ * random_in_unit_sphere());
    //step 2: 获得材料的衰减系数
    srecord.attenuation_ = albedo_;
    srecord.is_specular_ = true;
    srecord.pdf_ptr_ = nullptr;
    return true;
}

#endif