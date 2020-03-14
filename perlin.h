#ifndef PERLIN_H
#define PERLIN_H
#include "basic.h"

inline float linear_interpolate(float a[2][2][2], float u, float v, float w)
{
    float accum = 0;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            for(int k = 0; k < 2; k++)
            {
                accum += (i*u + (1-i)*(1-u)) * (j*v + (1-j)*(1-v)) * (k*w + (1-k)*(1-w)) * a[i][j][k];
            }
        }
    }
    return accum;
}

inline float perlin_interpolate(Vec3f c[2][2][2], float u, float v, float w)
{
    //hermite cubic 埃尔米特立方体方法平滑
    float u2 = u * u * (3 - 2*u);
    float v2 = v * v * (3 - 2*v);
    float w2 = w * w * (3 - 2*w);
    float accum = 0;
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {
            for(int k = 0; k < 2; k++)
            {
                Vec3f weight(u-i, v-j, w-k);
                accum += (i*u2 + (1-i)*(1-u2)) * (j*v2 + (1-j)*(1-v2)) * (k*w2 + (1-k)*(1-w2)) 
                        * (c[i][j][k].dotProduct(weight));
            }
        }
    }
    return accum;
}

class Perlin
{
public:
    //static float *randfloat_;
    static Vec3f *randvec_;
    static int *permx_;
    static int *permy_;
    static int *permz_;
    Perlin(){}
    //Perlin随机函数生成的随机序列以及三个方向的辅助随机分量序列
    float noise(const Vec3f &p) const //p:空间某点的位置
    {   
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);
        //hermite cubic 埃尔米特立方体方法平滑
        u = u * u * (3 - 2*u);
        v = v * v * (3 - 2*v);
        w = w * w * (3 - 2*w);
        int i = floor(p.x);
        int j = floor(p.y);
        int k = floor(p.z);
        //return randfloat_[permx_[i] ^ permy_[j] ^ permz_[k]];
        //插值
        // float list[2][2][2];
        // for(int a = 0; a < 2; a++)
        // {
        //     for(int b = 0; b < 2; b++)
        //     {
        //         for(int c = 0; c < 2; c++)
        //         {
        //             list[a][b][c] = randfloat_[permx_[(i+a)&255] ^ permy_[(j+b)&255] ^ permz_[(k+c)&255]];
        //         }
        //     }
        // }
        //return linear_interpolate(list, u, v, w);
        Vec3f c[2][2][2];
        for(int di=0; di<2; di++)
        {
            for(int dj=0; dj<2; dj++)
            {
                for(int dk=0; dk<2; dk++)
                {
                    c[di][dj][dk] = randvec_[permx_[(i+di)&255]^permy_[(j+dj)&255]^permz_[(k+dk)&255]];
                }
            }
        }
        return perlin_interpolate(c, u, v, w);
    }

    //噪声扰动
    float turb(const Vec3f &p, int depth = 7) const
    {
        float accum = 0;
        Vec3f temp = p;
        float weight = 1.0;
        for(int i = 0; i < depth; i++)
        {
            accum += weight * noise(temp);
            weight *= 0.5;
            temp *= 2;
        }
        return fabs(accum);
    }
    ~ Perlin();
};

// static float* perlin_generate()
// {
//     float *p = new float[256];
//     for(int i = 0; i < 256; i++)
//     {
//         p[i] = drand48();
//     }
//     return p;
// }

void permute(int *p, int n)
{
    for(int i = n-1; i > 0; i--)
    {
        int target = int(drand48()*(i+1));
        swap(p[i], p[target]);
    }
}

static int* perlin_generate_perm()
{
    int *p = new int[256];
    for(int i = 0; i < 256; i++)
    {
        p[i] = i;
    }
    permute(p, 256);
    return p;
}

static Vec3f* perlin_generate_vec()
{
    Vec3f *p = new Vec3f[256];
    for(int i = 0; i < 256; i++)
    {
        p[i] = Vec3f(-1 + 2*drand48(), -1 + 2*drand48(), -1 + 2*drand48());
        p[i].normalize();
    }
    return p;
}

/** 用成员函数初始化静态数据成员
 * 其中，总随机序列由第一种方法生成，序列中的每一个元素均为0~1的随机数
 * 分量的随机序列由第二种方法生成，即，初始序列为1-255，之后遍历整个序列，
 * 当前位置和一个随机生成的位置进行交换，已达到序列随机化
 * */
//float *Perlin::randfloat_ = perlin_generate();
int *Perlin::permx_ = perlin_generate_perm();
int *Perlin::permy_ = perlin_generate_perm();
int *Perlin::permz_ = perlin_generate_perm();
Vec3f *Perlin::randvec_ = perlin_generate_vec();

#endif