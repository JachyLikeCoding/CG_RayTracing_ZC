#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "basic.h"
#include "hitable.h"

class HitableList:public Hitable
{
public:
    Hitable **list_;
    int list_size_;
    HitableList(int size, Hitable **list) 
    {
        list_size_ = size;
        list_ = list;
    }
    ~HitableList(){}
    //实现父类虚函数
    virtual bool hit(Ray &ray, float t_min, 
                    float t_max, HitRecord &hitrecord) const;
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
        if(list_[i]->hit(ray, t_min, closest_tmp, record_tmp))
        {
            closest_tmp = record_tmp.t_;
            hitrecord = record_tmp;
            has_hit = true;
        }
    }
    return has_hit;
}

#endif