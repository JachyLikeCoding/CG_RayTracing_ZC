#ifndef OBJECT_H
#define OBJECT_H

#include "basic.h"

class object
{
public:
    string objname;
    Vec3f color;
    object(){};
    //与一条光线进行相交检测，如果和该光线相交返回true
    virtual bool intersect(const Vec3f &origin, 
                        const Vec3f &direction, 
                        float &t) const = 0;
    virtual void get_intersect_info(const Vec3f &hit_point) const = 0;
};


#endif