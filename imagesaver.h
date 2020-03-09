#ifndef IMAGESAVER_H
#define IMAGESAVER_H

#include "basic.h"
//bool save_ppm_image(string filename, int width, int height, Vec3f *data);

bool save_ppm_image(string filename, int width, int height, Vec3f *data)
{
    //图像尺寸
    int nx = width;
    int ny = height;
    int pixelsize = nx * ny;
    //图像存储路径
    string filepath = "/Users/zhangchi/Desktop/CG_RayTracing/results/";
    filename = filepath + filename;
    std::cout << "filename: " << filename << endl; 
    ofstream outfile(filename, ios_base::out);
    //文件头
    outfile << "P3\n" << nx << " " << ny << "\n255\n";
    //文件数据
    for(int i = 0; i < pixelsize; i++)
    {
        Vec3f rgb = data[i];
        /*double red = data[i].x * 255;
        double green = data[i].y * 255;
        double blue = data[i].z * 255;
        int ir = int(red);
        int ig = int(green);
        int ib = int(blue);*/

        outfile << rgb.x << " " << rgb.y << " " << rgb.z << "\n";
    }
    return true;
}

#endif