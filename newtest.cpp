#include <iostream>
#include <fstream>
#include "ray.h"
#include "imagesaver.h"
#include "hitablelist.h"
#include "sphere.h"
#include "movingsphere.h"
#include "camera.h"
#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "bvh.h"
#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "std_image.h"
#include "rectangle.h"
#include "cube.h"

#define testnum 1

//解决color为NAN的情况
inline Vec3f de_nan(const Vec3f &color)
{
    Vec3f temp = color;
    if(!(temp.x == temp.x)) temp.x = 0;
    if(!(temp.y == temp.y)) temp.y = 0;
    if(!(temp.z == temp.z)) temp.z = 0;
    return temp;
}


#if testnum == 1
void cornellbox(Hitable **scene, Camera **camera, float aspect)
{
    Hitable **list = new Hitable*[8];
    int i = 0;
    Material *red = new Lambertian2(new Constant_texture(Vec3f(0.65,0.05,0.05)));
    Material *white = new Lambertian2(new Constant_texture(Vec3f(0.73,0.73,0.73)));
    Material *green = new Lambertian2(new Constant_texture(Vec3f(0.12,0.45,0.15)));
    Material *light = new Diffuse_material(new Constant_texture(Vec3f(15,15,15)));
    list[i++] = new Turn_normal(new YZ_rectangle(0, 555, 0, 555, 555, green));
    list[i++] = new YZ_rectangle(0, 555, 0, 555, 0, red);
    list[i++] = new XZ_rectangle(213, 343, 227, 332, 554, light);
    list[i++] = new Turn_normal(new XZ_rectangle(0, 555, 0, 555, 555, white));
    list[i++] = new XZ_rectangle(0, 555, 0, 555, 0, white);
    list[i++] = new Turn_normal(new XY_rectangle(0, 555, 0, 555, 555, white));
    list[i++] = new Cube(Vec3f(130,0,65), Vec3f(295,165,230), white);
    list[i++] = new Cube(Vec3f(265,0,295), Vec3f(430,330,460), white);
    *scene =  new HitableList(i, list);
    Vec3f lookfrom(278,278,-800);
    Vec3f lookat(278,278,0);
    float focus_distance = 10.0;
    float aperture = 0.0;
    *camera = new Camera(40, aspect, lookfrom, lookat, Vec3f(0,1,0), aperture, focus_distance, 0.0, 1.0);
}

Vec3f color(Ray &ray, Hitable *world, int depth)
{
    HitRecord hitrecord;
    //modify this threshold 0.001
    if(world->hit(ray, 0.001, MAXFLOAT, hitrecord))
    {
        Ray scattered;
        Vec3f albedo;
        float pdf;
        //TODO:CHECK
        ray.direction_ = Vec3f(0,-1,0);
        Vec3f emitted = hitrecord.mtl_ptr_->emitted(ray, hitrecord,hitrecord.u_, hitrecord.v_, hitrecord.hitpoint_); 
        if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, albedo, scattered, pdf) && depth < 50)
        {
            Vec3f on_light = Vec3f(213 + drand48()*(343-213), 554, 227+drand48()*(332-227));
            Vec3f to_light = on_light - hitrecord.hitpoint_;
            float distance = to_light.length();
            if(to_light.normalize().dotProduct(hitrecord.hitnormal_) < 0)
            {
                return emitted;
            }
            float light_area = (343-213)*(332-227);
            float light_cos = fabs(to_light.y);
            if(light_cos < kEpsilon)
            {
                return emitted;
            }
            pdf = distance * distance / (light_cos * light_area);
            scattered = Ray(hitrecord.hitpoint_, to_light, ray.time_);
            //return attenuation * color(scattered, world, depth + 1) + emitted;
            return emitted + albedo * hitrecord.mtl_ptr_->scatter_pdf(ray, hitrecord, scattered) 
                             * color(scattered, world, depth+1) / pdf;
        }
        else
            return emitted;
    }
    else
    {
        return Vec3f(0.0, 0.0, 0.0);
    }
}

int main()
{
    int nx = 400;
    int ny = 400;
    int times = 200;
    Camera *camera;
    vector<Vec3f> pixeldata;
    Hitable *scene;
    cornellbox(&scene, &camera, float(nx)/float(ny));

    for(int j = ny - 1; j >= 0; j--)
    {
        for(int i = 0; i < nx; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < times; s++)
            {
                float u = float(i + rand()%(10000)/(float)(10000)) / float(nx);
                float v = float(j + rand()%(10000)/(float)(10000)) / float(ny);
                Ray ray = camera->get_uv_ray(u, v);
                col += de_nan(color(ray, scene, 0.0));
            }
            col /= float(times);
            col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
            int ir = int(255.99 * col.x);
            int ig = int(255.99 * col.y);
            int ib = int(255.99 * col.z);
            pixeldata.push_back(Vec3f(ir, ig, ib));
        }
    }
    Vec3f *data = &pixeldata[0];
    string filename = "cornellbox_directlight_change_emitted_test.ppm";
    save_ppm_image(filename, nx, ny, data);

    cout << "test finished!"<< endl;
    return 0;
}
#endif