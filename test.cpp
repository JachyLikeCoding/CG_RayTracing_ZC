#include <iostream>
#include <fstream>
#include "ray.h"
#include "imagesaver.h"
#include "hitablelist.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#define testnum 10

#if testnum == 1
bool test_image_saver()
{
    int nx = 200;//width
    int ny = 100;//height

    ofstream outfile( "/Users/zhangchi/Desktop/CG_RayTracing/results/mytest3.ppm", ios_base::out);
    outfile << "P3\n" << nx << " " << ny << "\n255\n";

    //std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int (255.99*r);
            int ig = int (255.99*g);
            int ib = int (255.99*b);

            outfile << ir << " " << ig << " " << ib << "\n";
            //std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return true;
}

#elif testnum == 2
//test ray data structure
void test_ray()
{
    Vec3f origin(0.0, 0.0, 0.0);
    Vec3f direction(0.0, 0.0, -1.0);
    Ray ray(origin, direction);
    cout << ray.get_direction();
    cout << ray.get_origin();
    cout << ray.get_t_location(3.0);
}

#elif testnum == 3
//try to draw sky image
Vec3f color(const Ray &ray)
{
    float t = 0.5 * (ray.direction_.y + 1.0);
    //cout << "t = " << t << endl;
    Vec3f col = (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    cout << col << endl;
    return col;
}

int main()
{
    int nx = 200;
    int ny = 100;
    Vec3f lower_left_corner(-2.0, -1.0, -1.0);
    Vec3f horizontal(4.0, 0.0, 0.0);
    Vec3f vertical(0.0, 2.0, 0.0);
    Vec3f origin(0.0, 0.0, 0.0);
    vector<Vec3f> pixeldata;
    
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            cout << u << ", " << v << endl;
            Vec3f dir = lower_left_corner + u * horizontal + v * vertical;
            cout << "dir----" << dir << endl;
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
            cout << i << "," << j << " ray.direction_" << ray.direction_ << endl;
            Vec3f col = color(ray);
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "sky.ppm";
    save_ppm_image(filename, nx, ny, data);
    return 0;
}

#elif testnum == 4
bool hit_sphere(const Vec3f &center, float radius, const Ray &ray)
{
    Vec3f oc = ray.origin_ - center;
    float a = ray.direction_.dotProduct(ray.direction_);
    float b = oc.dotProduct(ray.direction_) * 2.0;
    float c = oc.dotProduct(oc) - radius * radius;
    float delta = b * b - 4 * a * c;
    return (delta > 0);
}

Vec3f color(const Ray &ray)
{
    if(hit_sphere(Vec3f(0,0,-1), 0.5, ray))
    {
        return Vec3f(1,0,0);
    }
    float t = 0.5 * (ray.direction_.y + 1.0);
    //cout << "t = " << t << endl;
    return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
}

int main()
{
    int nx = 200;
    int ny = 100;
    Vec3f lower_left_corner(-2.0, -1.0, -1.0);
    Vec3f horizontal(4.0, 0.0, 0.0);
    Vec3f vertical(0.0, 2.0, 0.0);
    Vec3f origin(0.0, 0.0, 0.0);
    vector<Vec3f> pixeldata;
    
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Vec3f dir = lower_left_corner + u * horizontal + v * vertical;
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
            //cout << i << "," << j << " ray.direction_" << ray.direction_ << endl;
            Vec3f col = color(ray);
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "sky_with_sphere.ppm";
    save_ppm_image(filename, nx, ny, data);
    return 0;
}

#elif testnum == 5
float hit_sphere(const Vec3f &center, float radius, const Ray &ray)
{
    Vec3f oc = ray.origin_ - center;
    float a = ray.direction_.dotProduct(ray.direction_);
    float b = oc.dotProduct(ray.direction_) * 2.0;
    float c = oc.dotProduct(oc) - radius * radius;
    float delta = b*b - 4*a*c;
    if(delta < 0)
    {
        return -1;
    }
    else
    {
        return (-b - sqrt(delta)) / (2*a);
    }
}

Vec3f color(Ray &ray)
{
    float t = hit_sphere(Vec3f(0,0,-1), 0.5, ray);
    if(t > 0)
    {
        Vec3f normal = ray.get_t_location(t) - Vec3f(0,0,-1);
        normal = normal.normalize();
        return Vec3f(normal.x + 1, normal.y + 1, normal.z + 1) * 0.5;
    }
    float t2 = 0.5 * (ray.direction_.y + 1.0);
    //cout << "t = " << t << endl;
    return (1.0 - t2) * Vec3f(1.0, 1.0, 1.0) + t2 * Vec3f(0.5, 0.7, 1.0);
}

int main()
{
    int nx = 2000;
    int ny = 1000;
    Vec3f lower_left_corner(-2.0, -1.0, -1.0);
    Vec3f horizontal(4.0, 0.0, 0.0);
    Vec3f vertical(0.0, 2.0, 0.0);
    Vec3f origin(0.0, 0.0, 0.0);
    vector<Vec3f> pixeldata;
    
    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Vec3f dir = lower_left_corner + u * horizontal + v * vertical;
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
            //cout << i << "," << j << " ray.direction_" << ray.direction_ << endl;
            Vec3f col = color(ray);
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "sky_with_sphere.ppm";
    save_ppm_image(filename, nx, ny, data);
    return 0;
}

#elif testnum == 6
Vec3f color(Ray &ray, Hitable *world)
{
    HitRecord hitrecord;
    if(world->hit(ray, 0, (numeric_limits<float>::max)(), hitrecord))
    {
        return 0.5 * Vec3f(hitrecord.hitnormal_.x + 1,
                            hitrecord.hitnormal_.y + 1,
                            hitrecord.hitnormal_.z + 1);
    }
    else
    {
        float t = 0.5 * (ray.direction_.y + 1.0);
        return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 200;
    int ny = 100;
    Vec3f lower_left_corner(-2.0, -1.0, -1.0);
    Vec3f horizontal(4.0, 0.0, 0.0);
    Vec3f vertical(0.0, 2.0, 0.0);
    Vec3f origin(0.0, 0.0, 0.0);
    vector<Vec3f> pixeldata;
    Hitable *list[2];
    list[0] = new Sphere(Vec3f(0,0,-1), 0.5);
    list[1] = new Sphere(Vec3f(0,-100.5,-1), 100);
    Hitable *world = new HitableList(2, list);

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            Ray ray(origin, lower_left_corner + u * horizontal + v * vertical);
            Vec3f col = color(ray, world);
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            //cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "sky_with_several_sphere.ppm";
    save_ppm_image(filename, nx, ny, data);

    return 0;
}

#elif testnum == 7
Vec3f color(Ray &ray, Hitable *world)
{
    HitRecord hitrecord;
    if(world->hit(ray, 0, (numeric_limits<float>::max)(), hitrecord))
    {
        return 0.5 * Vec3f(hitrecord.hitnormal_.x + 1,
                            hitrecord.hitnormal_.y + 1,
                            hitrecord.hitnormal_.z + 1);
    }
    else
    {
        float t = 0.5 * (ray.direction_.y + 1.0);
        return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 200;
    int ny = 100;
    int times = 100;
    Camera camera;
    vector<Vec3f> pixeldata;
    Hitable *list[2];
    list[0] = new Sphere(Vec3f(0,0,-1), 0.5);
    list[1] = new Sphere(Vec3f(0,-1,-2), 1);
    Hitable *world = new HitableList(2, list);

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < times; s++)
            {
                float u = float(i + rand()%(100)/(float)(100)) / float(nx);
                float v = float(j + rand()%(100)/(float)(100)) / float(ny);
                Ray ray = camera.get_uv_ray(u, v);
                col += color(ray, world);
            }
            col /= float(times);
            
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            //cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "sky_with_several_sphere_antialiasing2.ppm";
    save_ppm_image(filename, nx, ny, data);

    return 0;
}

#elif testnum == 8

Vec3f color(Ray &ray, Hitable *world, int depth)
{
    HitRecord hitrecord;
    //TODO:modify this threshold 0.001
    if(world->hit(ray, 0.001, (numeric_limits<float>::max)(), hitrecord))
    {
        Ray scattered;
        Vec3f attenuation;
        if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, attenuation, scattered)
            && depth < 50)
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return Vec3f(0, 0, 0);
        }
    }
    else
    {
        float t = 0.5 * (ray.direction_.y + 1.0);
        return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 800;
    int ny = 400;
    int times = 100;
    //Camera camera(90, float(nx)/float(ny));
    Camera camera(20, float(nx)/float(ny), Vec3f(-2,2,1), Vec3f(0,0,-1), Vec3f(0,1,0));
    vector<Vec3f> pixeldata;
    Hitable *list[5];
    list[0] = new Sphere(Vec3f(0,0,-1), 0.5, new Lambertian(Vec3f(0.1, 0.2, 0.5)));
    list[1] = new Sphere(Vec3f(0,-100.5,-1), 100.0, new Lambertian(Vec3f(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Vec3f(1,0,-1), 0.5, new Metal(Vec3f(0.8, 0.6, 0.2), 0.0));
    list[3] = new Sphere(Vec3f(-1,0,-1), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(Vec3f(-1,0,-1), -0.45, new Dielectric(1.5));
    //float R = cos(PI/4);
    // Hitable *list[2];
    // list[0] = new Sphere(Vec3f(-R,0,-1), R, new Lambertian(Vec3f(0.0, 0.0, 1.0)));
    // list[1] = new Sphere(Vec3f( R,0,-1), R, new Lambertian(Vec3f(1.0, 0.0, 0.0)));
    
    Hitable *world = new HitableList(5, list);

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < times; s++)
            {
                float u = float(i + rand()%(100)/(float)(100)) / float(nx);
                float v = float(j + rand()%(100)/(float)(100)) / float(ny);
                Ray ray = camera.get_uv_ray(u, v);
                col += color(ray, world, 0);
            }
            col /= float(times);
            col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            //cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "camera_uvw_test2.ppm";
    save_ppm_image(filename, nx, ny, data);

    cout << "test finished!"<< endl;
    return 0;
}

#elif testnum == 9

Vec3f color(Ray &ray, Hitable *world, int depth)
{
    HitRecord hitrecord;
    //TODO:modify this threshold 0.001
    if(world->hit(ray, 0.001, (numeric_limits<float>::max)(), hitrecord))
    {
        Ray scattered;
        Vec3f attenuation;
        if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, attenuation, scattered)
            && depth < 50)
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return Vec3f(0, 0, 0);
        }
    }
    else
    {
        float t = 0.5 * (ray.direction_.y + 1.0);
        return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 400;
    int ny = 200;
    int times = 100;
    //Camera camera(90, float(nx)/float(ny));
    Vec3f lookfrom(3,3,2);
    Vec3f lookat(0,0,-1);
    float focus_distance = (lookfrom - lookat).length();
    float aperture = 2.0;
    //散焦效果相机
    Camera camera(20, float(nx)/float(ny), lookfrom, lookat, Vec3f(0,1,0), aperture, focus_distance);
    vector<Vec3f> pixeldata;
    Hitable *list[5];
    list[0] = new Sphere(Vec3f(0,0,-1), 0.5, new Lambertian(Vec3f(0.1, 0.2, 0.5)));
    list[1] = new Sphere(Vec3f(0,-100.5,-1), 100.0, new Lambertian(Vec3f(0.8, 0.8, 0.0)));
    list[2] = new Sphere(Vec3f(1,0,-1), 0.5, new Metal(Vec3f(0.8, 0.6, 0.2), 0.0));
    list[3] = new Sphere(Vec3f(-1,0,-1), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(Vec3f(-1,0,-1), -0.45, new Dielectric(1.5));
    //float R = cos(PI/4);
    // Hitable *list[2];
    // list[0] = new Sphere(Vec3f(-R,0,-1), R, new Lambertian(Vec3f(0.0, 0.0, 1.0)));
    // list[1] = new Sphere(Vec3f( R,0,-1), R, new Lambertian(Vec3f(1.0, 0.0, 0.0)));
    
    Hitable *world = new HitableList(5, list);

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < times; s++)
            {
                float u = float(i + rand()%(100)/(float)(100)) / float(nx);
                float v = float(j + rand()%(100)/(float)(100)) / float(ny);
                Ray ray = camera.get_uv_ray(u, v);
                col += color(ray, world, 0);
            }
            col /= float(times);
            col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            //cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "camera_aperture_test3.ppm";
    save_ppm_image(filename, nx, ny, data);

    cout << "test finished!"<< endl;
    return 0;
}

#elif testnum == 10

Hitable *random_scene()
{
    int n = 500;
    //定义一个包含n+1个元素的数组，数组的每个元素是指向hitable对象的指针。
    //然后将数组的指针赋值给list。所以，list是指针的指针
    Hitable **list = new Hitable*[n+1];
    //先创建一个中心在（0，-1000，0）半径为1000的超大漫射球，将其指针保存在list的第一个元素中。
    list[0] = new Sphere(Vec3f(0, -1000, 0), 1000, new Lambertian(Vec3f(0.5, 0.5, 0.5)));
    int i = 1;
    //两个for循环中会产生（11+11）*(11+11)=484个随机小球
    for(int a = -8; a < 8; a++)
    {
        for(int b = -11; b < 11; b++)
        {
            //产生一个（0，1）的随机数，作为设置小球材料的阀值
            float choose_mat = (rand()%(100)/(float)(100));
            //a+0.9*(rand()%(100)/(float)(100))”配合[-11,11]产生（-11，11）之间的随机数，而不是[-11,11)之间的22个整数。
            //使得球心的x,z坐标是（-11，11）之间的随机数
            Vec3f center(a + 0.9 * (rand()%(100)/(float)(100)), 0.2, 
                        b + 0.9 * (rand()%(100)/(float)(100)));
            //避免小球的位置和最前面的大球的位置太靠近
            if((center - Vec3f(4, 0.2, 0)).length() > 0.9)
            {
                //材料阀值小于0.8，则设置为漫反射球，漫反射球的衰减系数x,y,z都是（0，1）之间的随机数的平方
                if(choose_mat < 0.8)//diffuse
                {
                    list[i++] = new Sphere(center, 0.2, new Lambertian(Vec3f(
                        (rand()%100/(float)(100))*(rand()%100/(float)(100)),
                        (rand()%100/(float)(100))*(rand()%100/(float)(100)),
                        (rand()%100/(float)(100))*(rand()%100/(float)(100))
                    )));
                }
                //材料阀值大于等于0.8小于0.95，则设置为镜面反射球，镜面反射球的衰减系数x,y,z及模糊系数都是（0，1）之间的随机数加一再除以2
                else if(choose_mat < 0.95)//metal
                {
                    list[i++] = new Sphere(center, 0.2, new Metal(Vec3f(0.5*(1+(rand()%(100)/(float)(100))),
                    0.5*(1+(rand()%(100)/(float)(100))),
                    0.5*(1+(rand()%(100)/(float)(100)))),
                    0.5*(1+(rand()%(100)/(float)(100)))));
                }
                //材料阀值大于等于0.95，则设置为介质球
                else//glass
                {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    /*定义三个大球*/
    list[i++] = new Sphere(Vec3f(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(Vec3f(-4, 1, 0), 1.0, new Lambertian(Vec3f(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(Vec3f(4, 1, 0), 1.0, new Metal(Vec3f(0.7, 0.6, 0.5), 0.0));
    // list[i++] = new Sphere(Vec3f(-6,2,-6), 2.0, new Metal(Vec3f(0.7,0.6,0.5),0.0));
    // list[i++] = new Sphere(Vec3f( 6,2,-6), 2.0, new Metal(Vec3f(0.7,0.6,0.5),0.0));
    // list[i++] = new Sphere(Vec3f( 0,2,-7), 2.0, new Metal(Vec3f(0.7,0.6,0.5),0.0));

    // list[i++] = new Sphere(Vec3f(-2, 1, -4), 1.0, new Dielectric(1.5));
    // list[i++] = new Sphere(Vec3f(2, 1, -4), 1.0, new Dielectric(1.5));
    // list[i++] = new Sphere(Vec3f(0, 1, 0), 1.0, new Dielectric(1.5));

    // list[i++] = new Sphere(Vec3f(-4, 1, -2), 1.0, new Lambertian(Vec3f(0.4, 0.2, 0.1)));
    // list[i++] = new Sphere(Vec3f(4, 1, -2), 1.0, new Lambertian(Vec3f(0.4, 0.2, 0.1)));
    // list[i++] = new Sphere(Vec3f(-6, 1, 0), 1.0, new Metal(Vec3f(0.7, 0.6, 0.5), 0.0));
    // list[i++] = new Sphere(Vec3f(6, 1, 0), 1.0, new Metal(Vec3f(0.7, 0.6, 0.5), 0.0));
    cout << "sphere number: " << i << endl;
    return new HitableList(i, list);
}

Vec3f color(Ray &ray, Hitable *world, int depth)
{
    HitRecord hitrecord;
    //TODO:modify this threshold 0.001
    if(world->hit(ray, 0.001, (numeric_limits<float>::max)(), hitrecord))
    {
        Ray scattered;
        Vec3f attenuation;
        if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, attenuation, scattered)
            && depth < 50)
        {
            return attenuation * color(scattered, world, depth + 1);
        }
        else
        {
            return Vec3f(0, 0, 0);
        }
    }
    else
    {
        float t = 0.5 * (ray.direction_.y + 1.0);
        return (1.0 - t) * Vec3f(1.0, 1.0, 1.0) + t * Vec3f(0.5, 0.7, 1.0);
    }
}

int main()
{
    int nx = 600;
    int ny = 300;
    int times = 100;
    //Camera camera(90, float(nx)/float(ny));
    Vec3f lookfrom(13,2,3);
    Vec3f lookat(0,0,0);
    float focus_distance = 10.0;
    float aperture = 0.1;
    //散焦效果相机
    Camera camera(20, float(nx)/float(ny), lookfrom, lookat, Vec3f(0,1,0), aperture, focus_distance);
    vector<Vec3f> pixeldata;
    Hitable *world = random_scene();

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < times; s++)
            {
                float u = float(i + rand()%(100)/(float)(100)) / float(nx);
                float v = float(j + rand()%(100)/(float)(100)) / float(ny);
                Ray ray = camera.get_uv_ray(u, v);
                col += color(ray, world, 0.0);
            }
            col /= float(times);
            col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
            if(col.x != col.x)
            {
                col.x = (float)0.0;
                cerr << "color.x is out of range!!!" << endl;
            }
            if(col.y != col.y)
            {
                col.y = (float)0.0;
                cerr << "color.y is out of range!!!" << endl;
            }
            if(col.z != col.z)
            {
                col.z = (float)0.0;
                cerr << "color.z is out of range!!!" << endl;
            }
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            //cout << ir << " " << ig << " " << ib << endl;
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "random_scene_test3.ppm";
    save_ppm_image(filename, nx, ny, data);

    cout << "test finished!"<< endl;
    return 0;
}

#endif