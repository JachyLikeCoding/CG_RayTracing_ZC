#ifndef HITABLE_H
#define HITABLE_H

#include "basic.h"
#include "ray.h"
#include "aabb.h"

class Material;

//记录碰撞点信息
struct HitRecord
{
    Vec3f hitpoint_;
    Vec3f hitnormal_;
    float t_;//ray的t
    Material *mtl_ptr_;//记录所碰到物体的材质
    float u_;//平面坐标的uv映射
    float v_;
};

struct Vertex
{
    Vec3f m_position_;
    Vec3f m_normal_;
    Vec2f m_texcoord_;
};


//抽象类：表示能被光线碰撞到的任何物体
class Hitable
{
public:
    Hitable(){}
    virtual ~Hitable(){}

    virtual bool isleaf() const { return true; }
    virtual void prerender(){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const = 0;
    virtual bool bounding_box(float t0, float t1, AABB &box) const = 0;
    virtual float get_pdf_value(const Vec3f &o, const Vec3f &v) const{return 0.0;}
    virtual Vec3f random(const Vec3f &o) const{return Vec3f(1, 0, 0);}
};


class Translate : public Hitable
{
public:
    Hitable *ptr_;
    Vec3f offset_;
    Translate(Hitable *ptr, Vec3f offset) :ptr_(ptr), offset_(offset){}
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
};

bool Translate::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    Ray move(ray.origin_ - offset_, ray.direction_, ray.time_);
    if(ptr_->hit(move, t_min, t_max, hitrecord))
    {
        hitrecord.hitpoint_ += offset_;
        return true;
    }
    else
        return false;
}

bool Translate::bounding_box(float t0, float t1, AABB &box) const
{
    if(ptr_->bounding_box(t0, t1, box))
    {
        //包围盒也移动
        box = AABB(box.min_ + offset_, box.max_ + offset_);
        return true;
    }
    else
        return false;
}


class Yrotate : public Hitable
{
public:
    float sin_;
    float cos_;
    bool hasbox_;
    AABB bbox_;
    Hitable *ptr_;
    Yrotate(Hitable *ptr, float angle);
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = bbox_;
        return hasbox_;
    }
};

//x’ = cos(theta)*x + sin(theta)*z 
//z’ = -sin(theta)*x + cos(theta)*z
Yrotate::Yrotate(Hitable *ptr, float angle):ptr_(ptr)
{
    float radians = angle/180.0 * PI;
    sin_ = sin(radians);
    cos_ = cos(radians);
    hasbox_ = ptr_->bounding_box(0, 1, bbox_);
    Vec3f min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    Vec3f max(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            for(int k = 0; k < 2; k++)
            {
                float x = i * bbox_.max_.x + (1-i) * bbox_.min_.x;
                float y = j * bbox_.max_.y + (1-j) * bbox_.min_.y;
                float z = k * bbox_.max_.z + (1-k) * bbox_.min_.z;
                float newx = cos_ * x + sin_ * z;
                float newz = -sin_ * x + cos_ * z;
                Vec3f tmp(newx, y, newz);
                for(int c = 0; c < 3; c++)
                {
                    if(tmp[c] > max[c])
                        max[c] = tmp[c];
                    if(tmp[c] < min[c])
                        min[c] = tmp[c];
                }
            }
        }
    }
    bbox_ = AABB(min, max);
}

bool Yrotate::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    Vec3f origin = ray.origin_;
    Vec3f direction = ray.direction_;
    origin[0] = cos_ * origin[0] - sin_ * origin[2];
    origin[2] = sin_ * origin[0] + cos_ * origin[2];
    direction[0] = cos_ * direction[0] - sin_ * direction[2];
    direction[2] = sin_ * direction[0] + cos_ * direction[2];
    Ray rotated_ray(origin, direction, ray.time_);
    if(ptr_->hit(rotated_ray, t_min, t_max, hitrecord))
    {
        Vec3f p = hitrecord.hitpoint_;
        Vec3f n = hitrecord.hitnormal_;
        p[0] = cos_ * p[0] + sin_ * p[2];
        p[2] = -sin_ * p[0] + cos_ * p[2];
        n[0] = cos_ * n[0] + sin_ * n[2];
        n[2] = -sin_ * n[0] + cos_ * n[2];
        hitrecord.hitpoint_ = p;
        hitrecord.hitnormal_ = n;
        return true;
    }
    else
        return false;
}

#endif