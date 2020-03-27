#include "modelhitable.h"


ModelHitable::ModelHitable(const string &path, Vec3f position, Vec3f len, Material *mtl_ptr)
{
    mtl_ptr_ = mtl_ptr;
    //load_objfile(path);
    //translate(position - m_center);
    //scale(len);
}

// void ModelHitable::load_objfile(const string &path)
// {
//     vector<Vec3f> vertices;
//     vector<Vec3f> normals;
//     vector<Vec2f> texcoords;
//     Vec3f min_point(MAXFLOAT, MAXFLOAT, MAXFLOAT);
//     Vec3f max_point(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);

//     TestLoadObj("cbox/try.obj", "cbox/", false);

//     m_center = (max_point - min_point) / 2;
//     m_scale = max_point - min_point;

// }

