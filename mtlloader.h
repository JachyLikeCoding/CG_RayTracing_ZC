#ifndef MTLLOADER_H
#define MTLLOADER_H

#include "basic.h"
#include "material.h"

class MtlLoader{
public:
    vector<Material> mtl_list;
    bool load_mtl(const string mtlpath);
    bool find_mtl(string mtlname, Material *&mtl);
};




#endif