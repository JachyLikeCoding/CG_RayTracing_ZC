#ifndef AABB_H
#define AABB_H

#include "basic.h"
#include "ray.h"

//AABB包围盒
class AABB
{
public:
    Vec3f min_;
    Vec3f max_;

    AABB(){}
    AABB(Vec3f min, Vec3f max) : min_(min), max_(max){}
    Vec3f getmin(){ return min_; }
    Vec3f getmax(){ return max_; }

    //左大右小(左区间区max，右区间取min，比较2个值，如果左<右，为真)发生重叠。
    bool hit(float t_min, float t_max, Ray &ray) const
    {
        Vec3f origin = ray.origin_;
        Vec3f dir = ray.direction_;
        float invdir, t0, t1;
        for(int i = 0; i < 3; i++)
        {
            //t = [p(t)-origin] / direction
            invdir = 1.0f / dir[i];
            t0 = (min_[i] - origin[i]) * invdir;
            t1 = (max_[i] - origin[i]) * invdir;
            if(invdir < 0.0f)
            {
                std::swap(t0, t1);
            }
            t_min = t0 > t_min ? t0 : t_min;
            t_min = t1 < t_max ? t1 : t_max;
            if(t_max <= t_min)
            {
                return false;
            }
        }
        return true;
    }

    bool is_inside(const Vec3f &point) const
    {
        return (
            (point.x >= min_.x && point.x <= max_.x) &&
            (point.y >= min_.y && point.y <= max_.y) &&
            (point.z >= min_.z && point.z <= max_.z)
        );
    }

    static AABB surrounding_box(const AABB &box0, const AABB &box1)
    {
        Vec3f small(fmin(box0.min_.x, box1.min_.x), fmin(box0.min_.y, box1.min_.y), fmin(box0.min_.z, box1.min_.z));
        Vec3f big(fmax(box0.max_.x, box1.max_.x), fmax(box0.max_.y, box1.max_.y),fmax(box0.max_.z, box1.max_.z));
        return AABB(small, big);
    }

    vector<AABB> get_sub_aabb() const;
};


vector<AABB> AABB::get_sub_aabb() const
{
    Vec3f center = (min_ + max_) * 0.5;
    float edgex = (max_.x - min_.x) * 0.5;
    float edgey = (max_.y - min_.y) * 0.5;
    float edgez = (max_.z - min_.z) * 0.5;
    vector<AABB> record;
    record.resize(8);
    record[0] = AABB(min_, center);
    record[1] = AABB(min_ + Vec3f(edgex, 0, 0), center + Vec3f(edgex, 0, 0));
    record[2] = AABB(min_ + Vec3f(edgex, 0, edgez), center + Vec3f(edgex, 0,edgez));
    record[3] = AABB(min_ + Vec3f(0, 0, edgez), center + Vec3f(0, 0, edgez));
    record[4] = AABB(min_ + Vec3f(0, edgey, 0), center + Vec3f(0, edgey, 0));
    record[5] = AABB(min_ + Vec3f(edgex, edgey, 0), center + Vec3f(edgex, edgey, 0));
    record[6] = AABB(min_ + Vec3f(edgex, edgey, edgez), center + Vec3f(edgex, edgey, edgez));
    record[7] = AABB(min_ + Vec3f(0, edgey, edgez), center + Vec3f(0, edgey, edgez));

    return record;
}

#endif