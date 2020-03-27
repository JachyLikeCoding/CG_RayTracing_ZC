#ifndef CUBE_H
#define CUBE_H
#include "rectangle.h"
#include "hitablelist.h"

class Cube : public Hitable
{
public:
    Vec3f pmin_, pmax_;
    Hitable *list_;

    Cube(){}
    virtual ~Cube(){}
    Cube(const Vec3f &p0, const Vec3f &p1, Material *m_ptr);
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
    {
        return list_->hit(ray, t_min, t_max, hitrecord);
    }
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = AABB(pmin_, pmax_);
        return true;
    }
};

Cube::Cube(const Vec3f &p0, const Vec3f &p1, Material *m_ptr)
{
    pmin_ = p0;
    pmax_ = p1;
    
    vector<Hitable *> hitablelist;
    hitablelist.push_back(new XY_rectangle(p0.x, p1.x, p0.y, p1.y, p1.z, m_ptr));
    hitablelist.push_back(new Turn_normal(new XY_rectangle(p0.x, p1.x, p0.y, p1.y, p0.z, m_ptr)));
    hitablelist.push_back(new XZ_rectangle(p0.x, p1.x, p0.z, p1.z, p1.y, m_ptr));
    hitablelist.push_back(new Turn_normal(new XZ_rectangle(p0.x, p1.x, p0.z, p1.z, p0.y, m_ptr)));
    hitablelist.push_back(new YZ_rectangle(p0.y, p1.y, p0.z, p1.z, p1.x, m_ptr));
    hitablelist.push_back(new Turn_normal(new YZ_rectangle(p0.y, p1.y, p0.z, p1.z, p0.x, m_ptr)));
    //TODO: check modify here
    Hitable **list = new Hitable*[6];
    for(int i = 0; i < 6; i++)
    {
        list[i] = hitablelist[i];
    }
}

#endif