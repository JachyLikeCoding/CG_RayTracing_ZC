#include <iostream>
#include <fstream>
#include "basic.h"

#include "imagesaver.h"
#include "hitablelist.h"
#include "sphere.h"
#include "movingsphere.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "bvh.h"
#include "texture.h"
#include "rectangle.h"
#include "cube.h"
#include "pdf.h"
#include "raytracer.h"
#include "objloader.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "std_image_write.h"

using namespace std;

Raytracer tracer;
Objloader objloader;
Mtlloader mtlloader;

void cornellbox()
{
    tracer.init_tracer(256, 256);//长宽
    Material *red_mat = new Lambertian("red", new Constant_texture(Vec3f(0.95,0.05,0.05)));
    Material *white_mat = new Lambertian("white", new Constant_texture(Vec3f(0.85,0.75,0.65)));
    Material *green_mat = new Lambertian("green", new Constant_texture(Vec3f(0.05,0.05,0.95)));
    Material *light_mat = new Diffuse_light("light", new Constant_texture(Vec3f(10,10,10)),new Constant_texture(Vec3f(10,10,10)));
    Material *aluminum_mat = new Metal("alumium_mat", Vec3f(0.8, 0.85, 0.88), 0.0);
    Material *glass_mat = new Dielectric("glass", 1.5);

    Hitable *greenwall = new Turn_normal(new YZ_rectangle(0, 555, 0, 555, 555, green_mat));
    Hitable *redwall = new YZ_rectangle(0, 555, 0, 555, 0, red_mat);
    Hitable *light = new Turn_normal(new XZ_rectangle(213, 343, 227, 332, 554, light_mat));
    Hitable *top = new Turn_normal(new XZ_rectangle(0, 555, 0, 555, 555, white_mat));
    Hitable *bottom = new XZ_rectangle(0, 555, 0, 555, 0, white_mat);
    Hitable *back = new Turn_normal(new XY_rectangle(0, 555, 0, 555, 555, white_mat));
    Hitable *whitesphere = new Sphere(Vec3f(190, 90, 90), 90, white_mat);
    Hitable *glasssphere = new Sphere(Vec3f(260, 90, 260), 90, glass_mat);
    //list[i++] = new Translate(new Yrotate(new Cube(Vec3f(0, 0, 0), Vec3f(165, 165, 165), white), -18), Vec3f(130, 0, 65));
    //list[i++] = new Translate(new Yrotate(new Cube(Vec3f(0, 0, 0), Vec3f(165, 330, 165), white), 15), Vec3f(265, 0, 320));
    //list[i++] = new Translate(new Cube(Vec3f(0, 100, 0), Vec3f(165, 330, 165), white), Vec3f(265, 0, 295));
    //list[i++] = new Translate(new Cube(Vec3f(0, 150, 0), Vec3f(165, 330, 165), white), Vec3f(130, 0, 65));
    tracer.add_object(bottom);
    tracer.add_object(greenwall);
    tracer.add_object(redwall);
    tracer.add_object(back);
    tracer.add_object(top);
    //tracer.add_object(light);
    tracer.add_object(whitesphere);
    tracer.add_object(glasssphere);
    tracer.add_important_sampling(light);
    
    cout << "objects counts: " << tracer.m_objects.size() << endl;
    cout << "important samples counts: " << tracer.m_samplelist.m_list_.size() << endl;
    Vec3f lookfrom(278,278,-800);
    Vec3f lookat(278,278,0);
    Camera *cameratmp = new Camera(40, (float)1.0, lookfrom, lookat, Vec3f(0,1,0));
    tracer.m_config.m_camera = cameratmp;
}

void cbox_scene()
{
    //step1：初始化画布和相机
    tracer.init_tracer(256, 256);//长宽
    Vec3f lookfrom(278,273,-800);
    Vec3f lookat(278,273,-799);
    Vec3f up(0,1,0);
    Camera *cameratmp = new Camera(39.3077, 1.0, lookfrom, lookat, up);
    tracer.m_config.m_camera = cameratmp;
    //预创建模型列表
    Material *red_mat = new Lambertian("cbox:red", new Constant_texture(Vec3f(0.95,0.05,0.05)));
    Material *white_mat = new Lambertian("cbox:white", new Constant_texture(Vec3f(0.85,0.75,0.65)));
    Material *blue_mat = new Lambertian("cbox:blue", new Constant_texture(Vec3f(0.05,0.05,0.95)));
    Material *light_mat = new Diffuse_light("cbox:light", new Constant_texture(Vec3f(10,10,10)),new Constant_texture(Vec3f(0,0,0)));
    Material *silver_mat = new Metal("cbox:silver", Vec3f(0.77, 0.79, 0.73), 0.0);
    Material *glass_mat = new Dielectric("cbox:glass", 1.5);
    
    mtlloader.material_list_.push_back(blue_mat);//0
    //material_list.push_back(glass_mat);//1
    mtlloader.material_list_.push_back(light_mat);//2
    mtlloader.material_list_.push_back(red_mat);//3
    mtlloader.material_list_.push_back(white_mat);//4
    //material_list.push_back(silver_mat);//5
    objloader.mtl_loader_ = mtlloader;
    //加载模型
    objloader.loadobj("cbox/cbox2.obj", "cbox/");
    cout << "model number:" << objloader.modellist_.size();
    
    for(int i = 0; i < objloader.modellist_.size(); i++)
    {
        //根据材质决定是否重要性采样
        if(objloader.modellist_[i]->mtl_ptr_->is_important_sample == true)
        {
            tracer.add_important_sampling(objloader.modellist_[i]);
        }
        else
        {
            tracer.add_object(objloader.modellist_[i]);
        }
    }
}

void bedroom()
{
    tracer.init_tracer(640, 360);
    Vec3f lookfrom(-0.5, 3, 5.5);
    Vec3f lookat(-0.5, 2, 0);
    Vec3f up(0, 1, 0);
    float focus_distance = 10.0;
    float aspect = (float)(1280)/(float)(720);
    float aperture = 0.0;
    Camera *cameratmp = new Camera(60, aspect, lookfrom, lookat, up, aperture, focus_distance);
    tracer.m_config.m_camera = cameratmp;
    
    //预创建模型列表
    Material *bottle_mat = new Metal("scene1:bottle", Vec3f(0.95,0.05,0.05), 0.0);
    Material *gold_mat = new Metal("scene1:gold", Vec3f(0.85,0.75,0.65), 0.0);
    Material *lamp_mat = new Metal("scene1:lamp", Vec3f(0.05,0.05,0.95), 0.0);
    Material *light1_mat = new Diffuse_light("scene1:light1", new Constant_texture(Vec3f(20,10,20)),new Constant_texture(Vec3f(0,0,0)));
    Material *light3_mat = new Diffuse_light("scene1:light3", new Constant_texture(Vec3f(20,20,10)),new Constant_texture(Vec3f(0,0,0)));
    Material *silver_mat = new Metal("scene1:silver", Vec3f(0.77, 0.79, 0.73), 0.0);
    Material *white_mat = new Lambertian("scene1:white", new Constant_texture(Vec3f(0.4,0.4,0.4)));
    //材质的序号要和mtl文件中一致
    mtlloader.material_list_.push_back(bottle_mat);//0
    mtlloader.material_list_.push_back(gold_mat);//1
    mtlloader.material_list_.push_back(lamp_mat);//2
    mtlloader.material_list_.push_back(light1_mat);//3
    mtlloader.material_list_.push_back(light3_mat);//4
    mtlloader.material_list_.push_back(silver_mat);//5
    mtlloader.material_list_.push_back(white_mat);//6
    objloader.mtl_loader_ = mtlloader;
    //加载模型
    objloader.loadobj("diningroom/diningroom.obj", "diningroom/");
    cout << "model number:" << objloader.modellist_.size();
    
    for(int i = 0; i < objloader.modellist_.size(); i++)
    {
        //根据材质决定是否重要性采样
        if(objloader.modellist_[i]->mtl_ptr_->is_important_sample == true)
        {
           tracer.add_important_sampling(objloader.modellist_[i]);
        }
        else
        {
            tracer.add_object(objloader.modellist_[i]);
        }
    }
    cout << "objects counts: " << tracer.m_objects.size() << endl;
    cout << "important samples counts: " << tracer.m_samplelist.m_list_.size() << endl;
}

void veach_mis()
{
    tracer.init_tracer(768, 512);
    Vec3f lookfrom(0, 2, 15);
    Vec3f lookat(0, -2, 2.5);
    Vec3f up(0, 1, 0);
    float aspect = (float)(768)/(float)(512);
    Camera *cameratmp = new Camera(28, aspect, lookfrom, lookat, up);
    tracer.m_config.m_camera = cameratmp;
    //预创建模型列表
    Material *lambert7SG_mat = new Lambertian("mi:lambert7SG", new Constant_texture(Vec3f(0.4,0.4,0.4)));
    Material *light1_mat = new Diffuse_light("mi:light1",new Constant_texture(Vec3f(901.8,901.8,901.8)),new Constant_texture(Vec3f(0,0,0)));
    Material *light2_mat = new Diffuse_light("mi:light2",new Constant_texture(Vec3f(100,100,100)),new Constant_texture(Vec3f(0,0,0)));
    Material *light3_mat = new Diffuse_light("mi:light3",new Constant_texture(Vec3f(11.11,11.11,11.11)),new Constant_texture(Vec3f(0,0,0)));
    Material *light4_mat = new Diffuse_light("mi:light4",new Constant_texture(Vec3f(1.24,1.24,1.24)),new Constant_texture(Vec3f(0,0,0)));
    Material *light5_mat = new Diffuse_light("mi:light5",new Constant_texture(Vec3f(800,800,800)),new Constant_texture(Vec3f(0,0,0)));
    Material *plate1_mat = new Metal("mi:plate_1", Vec3f(0.07, 0.09, 0.13), 0.0);
    Material *plate2_mat = new Metal("mi:plate_2", Vec3f(0.07, 0.09, 0.13), 0.0);
    Material *plate3_mat = new Metal("mi:plate_3", Vec3f(0.07, 0.09, 0.13), 0.0);
    Material *plate5_mat = new Metal("mi:plate_5", Vec3f(0.07, 0.09, 0.13), 0.0);
    mtlloader.material_list_.push_back(lambert7SG_mat);//0
    mtlloader.material_list_.push_back(light1_mat);//1
    mtlloader.material_list_.push_back(light2_mat);//2
    mtlloader.material_list_.push_back(light3_mat);//3
    mtlloader.material_list_.push_back(light4_mat);//4
    mtlloader.material_list_.push_back(light5_mat);//5
    mtlloader.material_list_.push_back(plate1_mat);//6
    mtlloader.material_list_.push_back(plate2_mat);//7
    mtlloader.material_list_.push_back(plate3_mat);//8
    mtlloader.material_list_.push_back(plate5_mat);//9
    objloader.mtl_loader_ = mtlloader;
    objloader.loadobj("veach_mis/mis.obj", "veach_mis/");
    cout << "model number:" << objloader.modellist_.size();
    for(int i = 0; i < objloader.modellist_.size(); i++)
    {
        //根据材质决定是否重要性采样
        if(objloader.modellist_[i]->mtl_ptr_->is_important_sample == true)
        {
            tracer.add_important_sampling(objloader.modellist_[i]);
        }
        else
        {
            tracer.add_object(objloader.modellist_[i]);
        }
    }

}

int main()
{
    //init tracer
    tracer.set_recursion_depth(30);
    tracer.set_sampling_num(10);

    //choose scene:
    cbox_scene();
    //cornellbox();
    //bedroom();
    //veach_mis();

    //render:
    double total_time = 0.0;
    unsigned char * ret = nullptr;
    tracer.begin_frame();
    {
        ret = tracer.render(total_time);
    }
    tracer.end_frame();

    //write to image
    stbi_flip_vertically_on_write(1);
    stbi_write_png("./scene2result2.png", tracer.m_config.m_width, tracer.m_config.m_height, 3,
                tracer.m_image, tracer.m_config.m_width * 3);
    cout << "render finished!\n";
    cout << "total time: " << total_time << endl;

    return 0;
}
