#ifndef PDF_H
#define PDF_H
#include "basic.h"
#include "onb.h"
#include "lambertian.h"

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

class PDF
{
public: 
    //得到此处PDF的值
    virtual float pdf_value(Vec3f &direction) const = 0;
    //根据某个随机模型生成随机向量
    virtual Vec3f generate_random() const = 0;
    virtual ~PDF(){}
};

//cosine概率密度模型
class Cosine_PDF : public PDF
{
public:
    ONB uvw_;

    Cosine_PDF(const Vec3f &w)
    {
        uvw_.build_from_w(w);
    }

    //得到此处PDF的值
    virtual float pdf_value(Vec3f &direction) const
    {
        float cos = direction.normalize().dotProduct(uvw_.w());
        if(cos > 0)
        {
            return cos/PI;
        }
        else
            return 0;
    }

    //根据某个随机模型生成随机向量
    virtual Vec3f generate_random() const
    {
        return uvw_.local(random_cos_direction());
    }
};


class Hitable_PDF : public PDF
{
public:
    Hitable *ptr_;
    Vec3f origin_;

    Hitable_PDF(Hitable *ptr, Vec3f &origin) : ptr_(ptr), origin_(origin){}
    
    virtual float pdf_value(Vec3f &direction) const
    {
        return ptr_->get_pdf_value(origin_, direction);
    }
    //根据某个随机模型生成随机向量
    virtual Vec3f generate_random() const
    {
        return ptr_->random(origin_);
    }
};

class Mixture_PDF : public PDF
{
public:
    PDF *pdf_[2];

    Mixture_PDF(PDF *p1, PDF *p2)
    {
        pdf_[0] = p1;
        pdf_[1] = p2;
    }
    //返回混合pdf值
    virtual float pdf_value(Vec3f &direction) const
    {
        return 0.5 * pdf_[0]->pdf_value(direction) + 0.5 * pdf_[1]->pdf_value(direction);
    }
    //根据某个随机模型生成随机向量
    virtual Vec3f generate_random() const
    {
        if(drand48() < 0.5)
        {
            return pdf_[0]->generate_random();
        }
        else
        {
            return pdf_[1]->generate_random();
        }
    }
};

#endif