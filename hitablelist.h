#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "basic.h"
#include "hitable.h"

class HitableList:public Hitable
{
public:
    //Hitable **list_;
    vector<Hitable *> m_list_;
    int list_size_;
    HitableList(){}
    // HitableList(int size, Hitable **list) 
    // {
    //     list_size_ = size;
    //     list_ = list;
    // }
    ~HitableList(){}
    //实现父类虚函数
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    AABB surrounding_box(AABB &box0, AABB &box1) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
    virtual float get_pdf_value(const Vec3f &o, const Vec3f &v) const;
    virtual Vec3f random(const Vec3f &o) const;
    bool is_empty() const { return m_list_.empty(); }
    void add_object(Hitable *target)
    {
        m_list_.push_back(target); 
        list_size_++;
    }
};

bool HitableList::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    //判断列表中所有物体是否被光线碰撞，每次判断一个物体。
    //如果有碰撞，记录在hitrecord中，可反复记录，最后得到的是有效碰撞点。单个物体会有一个局部有效点，多个物体中再得到最终有效点
    //如果当前撞点在范围内，将其设为范围最大值，然后寻找更近的碰撞点。
    HitRecord record_tmp;
    bool has_hit = false;
    double closest_tmp = t_max;
    for(int i = 0; i < list_size_; i++)
    {
        if(m_list_[i]->hit(ray, t_min, closest_tmp, record_tmp))
        {
            closest_tmp = record_tmp.t_;
            hitrecord = record_tmp;
            has_hit = true;
        }
    }
    return has_hit;
}

//重构包围盒
AABB HitableList::surrounding_box(AABB &box0, AABB &box1) const
{
    Vec3f boxmin(fmin(box0.getmin()[0], box1.getmin()[0]),
                fmin(box0.getmin()[1], box1.getmin()[1]),
                fmin(box0.getmin()[2], box1.getmin()[2]));
    Vec3f boxmax(fmax(box0.getmax()[0], box1.getmax()[0]),
            fmax(box0.getmax()[1], box1.getmax()[1]),
            fmax(box0.getmax()[2], box1.getmax()[2]));
    return AABB(boxmin, boxmax);
}

bool HitableList::bounding_box(float t0, float t1, AABB &box) const
{
    if(list_size_ < 1)
    {
        return false;
    }
    AABB tempbox;
    if(m_list_[0]->bounding_box(t0,t1,tempbox))
    {
        box = tempbox;
    }
    else
    {
        return false;
    }

    for(int i = 1; i < list_size_; i++)
    {
        if(m_list_[i]->bounding_box(t0,t1,tempbox))
        {
            box = surrounding_box(box, tempbox);
        }
        else
        {
            return false;
        }
    }
    return true;
}

float HitableList::get_pdf_value(const Vec3f &o, const Vec3f &v) const
{
    float weight = 1.0 / list_size_;
    float sum = 0.0;
    for(int i = 0; i < list_size_; i++)
    {
        sum +=  m_list_[i]->get_pdf_value(o, v);
    }
    return weight * sum;
}

Vec3f HitableList::random(const Vec3f &o) const
{
    int index = int(drand48() * m_list_.size());
    return m_list_[index]->random(o);
}

#endif