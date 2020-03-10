#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "basic.h"
#include "material.h"
#include "metal.h"

class Dielectric : public Material
{
public:
    float refract_idx_;//光密介质的折射指数和光疏介质的折射指数的比值
    Dielectric(float refract_idx){ refract_idx_ = refract_idx; }
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const;
};

//计算折射光线的方向向量
/**
 * v:入射光线向量
 * n:单位法向量
 * ni_over_nt:折射介质/入射介质
 * refracted:折射光线向量
*/
bool refract(Vec3f &v, Vec3f &n, float ni_over_nt, Vec3f &refracted)
{
    Vec3f uv = v.normalize();
    float cos_a1 = -1.0 * uv.dotProduct(n);
    float temp = 1.0 - ni_over_nt * ni_over_nt * (1.0 - cos_a1 * cos_a1);
    if(temp > 0.0)
    {
        refracted = ni_over_nt * uv + (ni_over_nt * cos_a1 - sqrt(temp)) * n;
        return true;
    }
    else//折射光线方向向量无实根，即出现全反射
    {
        return false;
    }
    
}

//Schlick's approximation逼近公式:R(θ) = R0 + (1-R0)(1-cosθ)^5
//R0 = [(n1 - n2)/(n1 + n2)]^2, refract_idx = n2/n1
float schlick(float cos, float refract_idx)
{
    float r0 = (1 - refract_idx) / (1 + refract_idx);
    r0 = r0 * r0;
    return (r0 + (1 - r0) * pow((1-cos), 5));
}

bool Dielectric::scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const
{
    Vec3f outward_normal;
    Vec3f reflected = reflect_dir(ray_in.direction_, record.hitnormal_);
    float ni_over_nt;//入射介质的折射指数和折射介质的折射指数的比值
    attenuation = Vec3f(1.0, 1.0, 1.0);//不是光线不衰减
    Vec3f refracted;//折射光
    float cos;
    float reflect_prob;//反射系数，决定反射光线和折射光线的叠加
    if(ray_in.direction_.dotProduct(record.hitnormal_) > 0)
    {
        //光线从球内部射入空气。所以入射时法向量与球的法向量方向相反。
        //此时入射介质是光密介质，折射介质是光疏介质
        outward_normal = -record.hitnormal_;
        ni_over_nt = refract_idx_;
        //入射角余弦
        cos = ray_in.direction_.dotProduct(record.hitnormal_) 
                / (ray_in.direction_.length());
        cos = sqrt(1 - ni_over_nt * ni_over_nt * (1-cos*cos));
    }
    else
    {
        //从空气->球体。所以入射时法向量与球的法向量方向相同。
        outward_normal = record.hitnormal_;
        ni_over_nt = 1.0 / refract_idx_;
        cos = -ray_in.direction_.dotProduct(record.hitnormal_) 
                / (ray_in.direction_.length());
    }
    if(refract(ray_in.direction_, outward_normal, ni_over_nt, refracted))
    {
        //发生了折射，计算反射系数
        reflect_prob = schlick(cos, ni_over_nt);
        //scattered = Ray(record.hitpoint_, refracted);
    }
    else
    {
        //全反射
        //scattered = Ray(record.hitpoint_, reflected);
        //return false;
        reflect_prob = 1.0;
    }
    //叠加反射光线和折射光线
    if((rand()%(100)/(float)(100))< reflect_prob)
    {
        scattered = Ray(record.hitpoint_, reflected);
    }
    else
    {
        scattered = Ray(record.hitpoint_, refracted);
    }
    return true;//有折射光线
}

#endif