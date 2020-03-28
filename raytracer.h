#ifndef RAYTRACER_H
#define RAYTRACER_H
#include "basic.h"
#include "ray.h"
#include "camera.h"
#include "bvh.h"
#include "hitablelist.h"
#include "hitable.h"
#include "material.h"

class Raytracer
{
private:
    class Setting
    {
    public:
        Camera *m_camera;
        int m_maxdepth;
        int m_samplings;
        int m_width, m_height;
        int m_parallel_cpu;//0:串行 1:并行
        double start_frame, end_frame, total_frame;

        Setting():m_maxdepth(50), m_samplings(10), m_camera(nullptr), m_parallel_cpu(0) {}
        ~Setting()
        {
            if(m_camera)
                delete m_camera;
            m_camera = nullptr;
        }
    };

    class Manager
    {
    public:
    //texture
    //material
    };

public:
    BVH_node *m_root;
    Setting m_config;
    Manager m_manager;
    unsigned char *m_image;
    HitableList m_samplelist;
    vector<Hitable *> m_objects;

    void set_sampling_num(int sample_num) { m_config.m_samplings = sample_num; }
    void set_recursion_depth(int depth) { m_config.m_maxdepth = depth; }
    void set_parallelthread(int way) { m_config.m_parallel_cpu = way; }
    void add_object(Hitable *target);
    void add_important_sampling(Hitable *target);
    void begin_frame();
    void end_frame();
    void init_tracer(int width, int height);

    unsigned char *render(double &total_time);
    Vec3f tracing(Ray &ray, Hitable *scene, Hitable *light, int depth);
    void draw_pixels(int x, int y, const Vec3f &color);

private:
    void serial_render(Hitable *scene);
    void parallel_render(Hitable *scene);
};


void Raytracer::init_tracer(int width, int height)
{
    m_config.m_width = width;
    m_config.m_height = height;
    if(m_config.m_camera != nullptr)
    {
        delete m_config.m_camera;
    }

    //camera init
    Vec3f lookfrom(278,278,-800);
    Vec3f lookat(278,278,0);
    float focus_distance = 10.0;
    float aperture = 0.0;
    float aspect = (float)(m_config.m_width)/(float)(m_config.m_height);
    m_config.m_camera = new Camera(40, aspect, lookfrom, lookat, Vec3f(0,1,0), 
                        aperture, focus_distance);
    m_image = new unsigned char[m_config.m_width * m_config.m_height * 3];

    //clean up
    end_frame();
}


void Raytracer::add_important_sampling(Hitable *target)
{
    m_samplelist.add_object(target);
    add_object(target);
} 

void Raytracer::add_object(Hitable *target)
{
    m_objects.push_back(target);
} 

void Raytracer::begin_frame()
{
    cout << "begin frame~~" << endl;
    for(int x = 0; x < m_objects.size(); x++)
    {
        //prerender
        m_objects[x]->prerender();
    }
    if(m_root)
    {
        delete m_root;
    }
    //构建BVH树
    m_root = new BVH_node(m_objects, 0, m_objects.size());
}

void Raytracer::end_frame()
{
    cout << "end frame" << endl;
    BVH_node::destroy_tree(m_root);
    for(int i = 0; i < m_objects.size(); i++)
    {
        delete m_objects[i];
        m_objects[i] = nullptr;
    }
}

unsigned char *Raytracer::render(double &total_time)
{
    m_config.start_frame = clock();
    Hitable *hitable_node = reinterpret_cast<Hitable *>(m_root);
    
    //render
    if(m_config.m_parallel_cpu == 1)
    {
        parallel_render(hitable_node);
    }
    else
    {
        serial_render(hitable_node);
    }

    m_config.end_frame = clock();
    m_config.total_frame = static_cast<double>(m_config.end_frame - m_config.start_frame)/CLOCKS_PER_SEC;
    total_time = m_config.total_frame;

    return m_image;
}

void Raytracer::draw_pixels(int x, int y, const Vec3f &color)
{
    if(x < 0 || y < 0 || x > m_config.m_width || y > m_config.m_height)
    {
        cerr << "Error: image size is wrong.\n";
        return;
    }
    int index = (x + y * m_config.m_width) * 3;
    m_image[index] = static_cast<unsigned char>(255.99 * color.x);
    m_image[index + 1] = static_cast<unsigned char>(255.99 * color.y);
    m_image[index + 2] = static_cast<unsigned char>(255.99 * color.z);
}

void Raytracer::serial_render(Hitable *scene)
{
    for(int j = m_config.m_height - 1; j >= 0; j--)
    {
        for(int i = 0; i < m_config.m_width; i++)
        {
            Vec3f col(0, 0, 0);
            for(int s = 0; s < m_config.m_samplings; s++)
            {
                float u = float(i + drand48()) / float(m_config.m_width);
                float v = float(j + drand48()) / float(m_config.m_height);
                Ray ray = m_config.m_camera->get_uv_ray(u, v);
                col += de_nan(tracing(ray, scene, &m_samplelist, 0.0));
            }
            col /= float(m_config.m_samplings);
            col = Vec3f(sqrt(col.x), sqrt(col.y), sqrt(col.z));
            draw_pixels(i, j, col);
            cout << "draw pixel-----" << i << ", " << j << endl;
        }
    }
}

void Raytracer::parallel_render(Hitable *scene)
{
    //TODO: add parallel render
    cout << "Sorry, the parallel part has not finished...\n";
}


Vec3f Raytracer::tracing(Ray &ray, Hitable *world, Hitable *lightshape, int depth)
{
    HitRecord hitrecord;
    if(world->hit(ray, 0.001, MAXFLOAT, hitrecord))
    {
        ScatterRecord srecord;
        //TODO: check get material 
        Vec3f emitted = hitrecord.mtl_ptr_->emitted(ray, hitrecord, hitrecord.u_, hitrecord.v_, hitrecord.hitpoint_); 

        if(hitrecord.mtl_ptr_->scatter(ray, hitrecord, srecord) && depth < m_config.m_maxdepth)
        {
            if(srecord.is_specular_)
            {
                return srecord.attenuation_ * tracing(srecord.specular_ray_, world, lightshape, depth+1);
            }
            else
            {
                Vec3f dir;
                float pdf;
                Ray scattered;
                if(!m_samplelist.is_empty())
                {
                    Hitable_PDF plight(lightshape, hitrecord.hitpoint_);
                    Mixture_PDF p(&plight, srecord.pdf_ptr_);
                    dir = p.generate_random();
                    scattered = Ray(hitrecord.hitpoint_, dir, ray.time_);
                    pdf = p.pdf_value(scattered.direction_);
                }
                else
                {
                    dir = srecord.pdf_ptr_->generate_random();
                    scattered = Ray(hitrecord.hitpoint_, dir, ray.time_);
                    pdf = srecord.pdf_ptr_->pdf_value(dir);
                }
                
                delete srecord.pdf_ptr_;
                return emitted + srecord.attenuation_ * hitrecord.mtl_ptr_->scatter_pdf(ray, hitrecord, scattered) 
                             * tracing(scattered, world, lightshape, depth+1) / pdf;
            }
        }
        else
            return emitted;
    }
    else
    {
        return Vec3f(0.0, 0.0, 0.0);
    }
}
#endif