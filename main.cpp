#include <iostream>
#include <vector>
#include <string>
#include "objloader.h"
#include "mtlloader.h"

using namespace std;

int main()
{
    ObjLoader objloader;
    string objpath = "/Users/zhangchi/Desktop/CG_RayTracing/diningroom/";
    string objname = objpath + "diningroom.obj";
    //objLoader.load_obj();

    MtlLoader mtlloader;
    string mtlname = objpath + "diningroom.mtl";
    //mtlloader.load_mtl(mtlname);
    cout << "hello ray tracing2\t";
    return 0;
}
