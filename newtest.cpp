// #include <iostream>
// #include <fstream>
// #include "ray.h"
// #include "imagesaver.h"
// #include "hitablelist.h"
// #include "sphere.h"
// #include "movingsphere.h"
// #include "camera.h"
// #include "material.h"
// #include "lambertian.h"
// #include "metal.h"
// #include "dielectric.h"
// #include "bvh.h"
// #include "texture.h"
// #include "rectangle.h"
// #include "cube.h"
// #include "pdf.h"
// #define testnum 1

// #if testnum == 1
// void cornellbox(Hitable **scene, Camera **camera, float aspect)
// {
//     Hitable **list = new Hitable*[8];
//     int i = 0;
//     Material *red = new Lambertian(new Constant_texture(Vec3f(0.65,0.05,0.05)));
//     Material *white = new Lambertian(new Constant_texture(Vec3f(0.73,0.73,0.73)));
//     Material *green = new Lambertian(new Constant_texture(Vec3f(0.12,0.45,0.15)));
//     Material *light = new Diffuse_light(new Constant_texture(Vec3f(15,15,15)),new Constant_texture(Vec3f(0,0,0)));
//     Material *aluminum = new Metal(Vec3f(0.8, 0.85, 0.88), 0.0);
//     Material *glass = new Dielectric(1.5);
//     list[i++] = new Turn_normal(new YZ_rectangle(0, 555, 0, 555, 555, green));
//     list[i++] = new YZ_rectangle(0, 555, 0, 555, 0, red);
//     list[i++] = new Turn_normal(new XZ_rectangle(213, 343, 227, 332, 554, light));
//     list[i++] = new Turn_normal(new XZ_rectangle(0, 555, 0, 555, 555, white));
//     list[i++] = new XZ_rectangle(0, 555, 0, 555, 0, white);
//     list[i++] = new Turn_normal(new XY_rectangle(0, 555, 0, 555, 555, white));
//     list[i++] = new Sphere(Vec3f(190, 90, 90), 90, white);
//     list[i++] = new Sphere(Vec3f(260, 90, 260), 90, glass);
//     //list[i++] = new Translate(new Yrotate(new Cube(Vec3f(0, 0, 0), Vec3f(165, 165, 165), white), -18), Vec3f(130, 0, 65));
//     //list[i++] = new Translate(new Yrotate(new Cube(Vec3f(0, 0, 0), Vec3f(165, 330, 165), white), 15), Vec3f(265, 0, 320));
//     //list[i++] = new Translate(new Cube(Vec3f(0, 100, 0), Vec3f(165, 330, 165), white), Vec3f(265, 0, 295));
//     //list[i++] = new Translate(new Cube(Vec3f(0, 150, 0), Vec3f(165, 330, 165), white), Vec3f(130, 0, 65));
//     *scene =  new HitableList(i, list);
//     Vec3f lookfrom(278,278,-800);
//     Vec3f lookat(278,278,0);
//     float focus_distance = 10.0;
//     float aperture = 0.0;
//     *camera = new Camera(40, aspect, lookfrom, lookat, Vec3f(0,1,0), aperture, focus_distance, 0.0, 1.0);
// }

// Vec3f color(Ray &ray, Hitable *world, Hitable *lightshape, int depth)
// {
//     HitRecord hitrecord;
    
//     if(world->hit(ray, 0.001, MAXFLOAT, hitrecord))
//     {
//         ScatterRecord srecord;
//         Vec3f emitted = hitrecord.mtl_ptr_->emitted(ray, hitrecord, hitrecord.u_, hitrecord.v_, hitrecord.hitpoint_); 

//         if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, srecord) && depth < 50)
//         {
//             if(srecord.is_specular_)
//             {
//                 return srecord.attenuation_ * color(srecord.specular_ray_, world, lightshape, depth+1);
//             }
//             else
//             {
//                 Hitable_PDF plight(lightshape, hitrecord.hitpoint_);
//                 Mixture_PDF p(&plight, srecord.pdf_ptr_);
//                 Ray scattered = Ray(hitrecord.hitpoint_, p.generate_random(), ray.time_);
//                 float pdf = p.pdf_value(scattered.direction_);
//                 delete srecord.pdf_ptr_;
//                 return emitted + srecord.attenuation_ * hitrecord.mtl_ptr_->scatter_pdf(ray, hitrecord, scattered) 
//                              * color(scattered, world, lightshape, depth+1) / pdf;
//             }
//         }
//         else
//             return emitted;
//     }
//     else
//     {
//         return Vec3f(0.0, 0.0, 0.0);
//     }
// }

// int main()
// {
//     int nx = 400;
//     int ny = 400;
//     int times = 500;
//     Camera *camera;
//     vector<Vec3f> pixeldata;
//     Hitable *scene;
//     cornellbox(&scene, &camera, float(ny)/float(nx));
//     Hitable *lightshape = new XZ_rectangle(213, 343, 227, 332, 554, 0);
//     Hitable *glasssphere = new Sphere(Vec3f(190, 90,190), 90, 0);
//     Hitable *a[2];
//     a[0] = lightshape;
//     a[1] = glasssphere;
//     HitableList hlist(2, a);

//     for(int j = ny - 1; j >= 0; j--)
//     {
//         for(int i = 0; i < nx; i++)
//         {
//             Vec3f col(0, 0, 0);
//             for(int s = 0; s < times; s++)
//             {
//                 float u = float(i + drand48()) / float(nx);
//                 float v = float(j + drand48()) / float(ny);
//                 Ray ray = camera->get_uv_ray(u, v);
//                 col += de_nan(color(ray, scene, lightshape, 0.0));
//             }
//             col /= float(times);
//             col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
//             int ir = int(255.99 * col.x);
//             int ig = int(255.99 * col.y);
//             int ib = int(255.99 * col.z);
//             pixeldata.push_back(Vec3f(ir, ig, ib));
//         }
//     }
//     Vec3f *data = &pixeldata[0];
//     string filename = "cornellbox_debug_test_cube7.ppm";
//     save_ppm_image(filename, nx, ny, data);

//     cout << "test finished!"<< endl;
//     return 0;
// }
// #endif