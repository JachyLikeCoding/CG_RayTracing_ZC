#ifndef MATERIAL_H
#define MATERIAL_H

#include "basic.h"
#include "hitable.h"
#include "texture.h"
#include "pdf.h"

// class MaterialAttribute
// {
// public:
//     MaterialAttribute(){}
// };

// class Kd : public MaterialAttribute
// {
//     /*
//     Kd:使用RGB值指定漫反射率。
//     “ r g b”是大气中红色，绿色和蓝色分量的值。 g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Kd():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// };

// class Ka : public MaterialAttribute
// {
//     /*
//     Ka:使用RGB值指定环境反射率。
//     “ r g b”是该颜色的红色，绿色和蓝色分量的值。 g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。 r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Ka():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// }; 

// class Ks : public MaterialAttribute
// {
//     /*
//     Ks:使用RGB值指定镜面反射率。 
//     “r g b”是大气中红色，绿色和蓝色分量的值。g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Ks():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// };

// class Tf :public MaterialAttribute
// {
// 	/*
//     Tf：透射率。“r g b”是大气中红色，绿色和蓝色分量的值。 
//     g和b参数是可选的。 如果仅指定r，则假定g和b等于r。
//     r g b值通常在0.0到1.0的范围内。超出此范围的值会相应增加或降低反射率。
//     */
// public:
// 	float r_, g_, b_;
// 	Tf() :MaterialAttribute() 
// 	{
// 		r_ = 0.0;
// 		g_ = 0.0;
// 		b_ = 0.0;
// 	}
// };

// class Ns:public MaterialAttribute
// {
//     /*
//     Ns:指定当前材质的镜面反射指数。 
//     这定义了镜面高光的焦点。 “指数”是镜面指数的值。 
//     高指数导致紧密，集中的高光。 Ns值通常在0到1000之间。
//     */
// public:
//     float exponent_;
//     Ns():MaterialAttribute()
//     {
//         exponent_ = 1;
//     }
// };

// class Tr:public MaterialAttribute
// {
//     /*
//     Tr:当前材料的透明率。 
//     如果Tr = 0，则表示它是完全不透明的。 
//     值越高，材料越透明。 Tr值通常在0.0到1.0之间。
//     */
// public:
//     float ratio_;
//     Tr():MaterialAttribute()
//     {
//         ratio_ = 0;
//     }
// };

// class Ni:public MaterialAttribute
// {
//     /*
//     Ni:光密度
//     指定表面的光密度。也称为折射率。值的范围是0.001至10。
//     值1.0表示光在通过时不会弯曲，通过增加物体的光密度会增加弯曲量。
//     玻璃的折射率约为1.5。小于1.0的值会产生奇怪的结果，因此不建议使用。
//     */
// public:
//     float optical_dencity_;
//     Ni():MaterialAttribute()
//     {
//         optical_dencity_ = 1;
//     }
// };

// class Material
// {
// public:
//     string mtlname_;
//     int illum_;
//     Kd kd_;
//     Ka ka_;
//     Ks ks_;
//     Ns ns_;
//     Tf tf_;
//     Tr tr_;
//     Ni ni_;

//     bool self_luminous_;
//     bool transparent_;
//     bool specular_;
//     bool diffuse_;
//     Material(){}
//     Material(string mtlname)
//     {
//         mtlname_ = mtlname;
//         self_luminous_ = false;
//         transparent_ = false;
//         specular_ = false;
//         diffuse_ = false;
//     }
// };
// #endif

struct ScatterRecord
{
    Ray specular_ray_;
    bool is_specular_;
    Vec3f attenuation_;
    PDF *pdf_ptr_;
};

class Material
{
public:
    string material_name_;
    bool is_important_sample;
    Material(){is_important_sample = false;}
    virtual ~Material(){}
    //散射
    //virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered, float &pdf) const{return false;}
    virtual bool scatter(Ray &ray_in, HitRecord &hitrecord, ScatterRecord &scatterrecord) const
    {
        return false;
    }

    virtual float scatter_pdf(Ray &ray_in, HitRecord &record, Ray &scattered) const
    {
        //TODO: CHECK here
        return 0.0;
    }
    //不是自发光材质的默认黑色，自发光材料子类重写
    virtual Vec3f emitted(Ray &ray_in, HitRecord &record, float u, float v, const Vec3f &p) const
    {
        return Vec3f(0.0, 0.0, 0.0);//默认黑色
    }
};

//自发光材质-----------------------------------------------------------------------diffuse light
class Diffuse_light : public Material{
public:
    Texture *emit_texture_;
    Texture *albedo_texture_;
    Diffuse_light(Texture *emittex, Texture *albedotex):emit_texture_(emittex), albedo_texture_(albedotex)
    {
        is_important_sample = true;
    }
    Diffuse_light(string name, Texture *emittex, Texture *albedotex):emit_texture_(emittex), albedo_texture_(albedotex)
    {
        material_name_ = name;
        is_important_sample = true;
    }
   
    virtual bool scatter(Ray &ray_in, HitRecord &hitrecord, ScatterRecord &scatterrecord) const
    {
        return false;
    }

    virtual Vec3f emitted(Ray &ray_in, HitRecord &record, float u, float v, const Vec3f &p) const
    {
        //反射光线与反射之后与法向量的夹角为锐角的时候才进行纹理计算
        if(record.hitnormal_.dotProduct(ray_in.direction_) < 0.0)
        {
            return emit_texture_->value(u, v, p);
        }
        else
        {
            return Vec3f(0, 0, 0);
        }
    }
};

//漫反射材质-----------------------------------------------------------------lambertian
class Lambertian : public Material {
public:
    Texture *albedo_;   

    Lambertian(Texture *a) : albedo_(a) {is_important_sample = false;}
    Lambertian(string name, Texture *a) : albedo_(a) 
    {
        is_important_sample = false;
        material_name_ = name;
    }
    virtual ~Lambertian(){}

    bool scatter(Ray &ray_in, HitRecord &hitrecord, ScatterRecord &srecord) const  
    {
        srecord.is_specular_ = false;
        srecord.attenuation_ = albedo_->value(hitrecord.u_, hitrecord.v_, hitrecord.hitpoint_);
        srecord.pdf_ptr_ = new Cosine_PDF(hitrecord.hitnormal_);
        return true;
    }

    float scatter_pdf(Ray &ray_in, HitRecord &record, Ray &scattered) const
    {
        float cos = record.hitnormal_.dotProduct(scattered.direction_.normalize());
        //cosine值是散射光线和表面法线的夹角余弦，角大于90°无效，表示采样失败
        if(cos < 0.0)
        {
            return 0.0;
        }
        return cos / PI;
    }
};
#endif