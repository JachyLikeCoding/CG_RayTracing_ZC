#ifndef MODELHITABLE_H
#define MODELHITABLE_H
#include "meshhitable.h"
#include "objloader.h"

class ModelHitable : public MeshHitable
{
public:
    Vec3f m_center;
    Vec3f m_scale;
    //bool is_important;
    ModelHitable(Material *mtl_ptr)
    { 
        mtl_ptr_ = mtl_ptr; 
        //is_important = false;
    }
    ModelHitable(const string &objpath, Vec3f position, Vec3f len, Material *mtl_ptr);
    virtual ~ModelHitable(){}

    virtual float get_pdf_value(const Vec3f &o, const Vec3f &v) const {return 0.0;}
    virtual Vec3f random(const Vec3f &o) const {return Vec3f(1, 0, 0);}
    //void load_objfile(const string &path);
};

#endif