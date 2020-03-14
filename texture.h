#ifndef TEXTURE_H
#define TEXTURE_H
#include "basic.h"
#include "perlin.h"

class Texture
{
public:
    virtual Vec3f value(float u, float v, const Vec3f &p) const = 0;
};

//常量纹理（基础纹理）
class Constant_texture : public Texture
{
public:
    Vec3f color_;
    Constant_texture(){}
    Constant_texture(Vec3f color):color_(color){}
    virtual Vec3f value(float u, float v, const Vec3f &p) const
    {
        return color_;
    }

};

class Checker_texture : public Texture
{
public:
    Texture *t0_;
    Texture *t1_;
    Checker_texture(){}
    Checker_texture(Texture *t0, Texture *t1)
    {
        t0_ = t0;
        t1_ = t1;
    }
    virtual Vec3f value(float u, float v, const Vec3f &p) const
    {
        float sines = sin(10*p.x) * sin(10*p.y) * sin(10*p.z);
        if(sines < 0)
        {
            return t0_->value(u,v,p);
        }
        else
        {
            return t1_->value(u,v,p);
        }
    }
};

//噪声纹理
class Noise_texture : public Texture
{
public:
    Perlin perlin_noise_;
    float scale_;
    Noise_texture(){}
    Noise_texture(float scale) : scale_(scale){}
    virtual Vec3f value(float u, float v, const Vec3f &p) const
    {
        return Vec3f(1,1,1)*0.5*(1 + sin(scale_ * p.x + 10 * perlin_noise_.turb(p)));
    }
};

class Image_texture : public Texture
{
public:
    unsigned char *data_;//图片的纹理数据
    int nx_, ny_;

    Image_texture(){}
    Image_texture(unsigned char *data, int nx, int ny) : data_(data), nx_(nx), ny_(ny){}
    virtual Vec3f value(float u, float v, const Vec3f &p) const;
};

Vec3f Image_texture::value(float u, float v, const Vec3f &p) const
{
    int i = (1-u) * nx_;
    int j = (1-v) * ny_ - 0.001;
    if(i < 0) i = 0;
    if(j < 0) j = 0;
    if(i > nx_ - 1) i = nx_ - 1;
    if(j > ny_ - 1) j = ny_ - 1;
    float r = int(data_[3*i + 3* nx_ *j])/255.0;
    float g = int(data_[3*i + 3* nx_ *j+1])/255.0;
    float b = int(data_[3*i + 3* nx_ *j+2])/255.0;
    return Vec3f(r,g,b);
}

#endif