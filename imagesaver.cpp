#include "imagesaver.h"

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
        double red = data[i].x * 255;
        double green = data[i].y * 255;
        double blue = data[i].z * 255;
        int ir = int(red);
        int ig = int(green);
        int ib = int(blue);
        outfile << ir << " " << ig << " " << ib << "\n";
    }
    return true;
}

int test_imagesaver()
{
    vector<Vec3f> testdata;
    for(int i=0; i<20; i++)
    {
        Vec3f data;
        data.x = 0.2;
        data.y = 0.2;
        data.z = 0.0;
        testdata.push_back(data);
    }
    for(int i=0; i<testdata.size(); i++)
    {
        cout << testdata[i] << endl;
    }
    Vec3f *data = &testdata[0];
    save_ppm_image("test_image1.ppm", 4, 5, data);
    cout << "try again";
    return 0;
}