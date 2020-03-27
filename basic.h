#ifndef BASIC_H
#define BASIC_H
#define TINYOBJLOADER_IMPLEMENTATION    

#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <string>
#include <iterator>
#include <limits>
#include <random>
#include "geometry.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"

using namespace std;
#define PI 3.141592653589793

static const float kEpsilon = 1e-6;
static const Vec3f kDefaultBackgroundColor(0, 0, 0);

//解决color为NAN的情况
inline Vec3f de_nan(const Vec3f &color)
{
    Vec3f temp = color;
    if(!(temp.x == temp.x)) temp.x = 0;
    if(!(temp.y == temp.y)) temp.y = 0;
    if(!(temp.z == temp.z)) temp.z = 0;
    return temp;
}
#endif