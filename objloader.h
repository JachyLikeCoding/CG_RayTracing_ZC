#ifndef OBJLOADER_H
#define OBJLOADER_H
#include "basic.h"
#include "material.h"
#include "modelhitable.h"
#include "hitablelist.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
#include <mmsystem.h>
#ifdef __cplusplus
}
#endif
#pragma comment(lib, "winmm.lib")
#else
#if defined(__unix__) || defined(__APPLE__)
#include <sys/time.h>
#else
#include <ctime>
#endif
#endif

#ifdef __clang__
#pragma clang diagnostic push
// #if __has_warning("-Wzero-as-null-pointer-constant")
// #pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
// #endif
#endif

class timerutil {
 public:
#ifdef _WIN32
  typedef DWORD time_t;

  timerutil() { ::timeBeginPeriod(1); }
  ~timerutil() { ::timeEndPeriod(1); }

  void start() { t_[0] = ::timeGetTime(); }
  void end() { t_[1] = ::timeGetTime(); }

  time_t sec() { return (time_t)((t_[1] - t_[0]) / 1000); }
  time_t msec() { return (time_t)((t_[1] - t_[0])); }
  time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000); }
  time_t current() { return ::timeGetTime(); }

#else
#if defined(__unix__) || defined(__APPLE__)
  typedef unsigned long int time_t;

  void start() { gettimeofday(tv + 0, &tz); }
  void end() { gettimeofday(tv + 1, &tz); }

  time_t sec() { return static_cast<time_t>(tv[1].tv_sec - tv[0].tv_sec); }
  time_t msec() {
    return this->sec() * 1000 +
           static_cast<time_t>((tv[1].tv_usec - tv[0].tv_usec) / 1000);
  }
  time_t usec() {
    return this->sec() * 1000000 +
           static_cast<time_t>(tv[1].tv_usec - tv[0].tv_usec);
  }
  time_t current() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return static_cast<time_t>(t.tv_sec * 1000 + t.tv_usec);
  }

#else  // C timer
  // using namespace std;
  typedef clock_t time_t;

  void start() { t_[0] = clock(); }
  void end() { t_[1] = clock(); }

  time_t sec() { return (time_t)((t_[1] - t_[0]) / CLOCKS_PER_SEC); }
  time_t msec() { return (time_t)((t_[1] - t_[0]) * 1000 / CLOCKS_PER_SEC); }
  time_t usec() { return (time_t)((t_[1] - t_[0]) * 1000000 / CLOCKS_PER_SEC); }
  time_t current() { return (time_t)clock(); }

#endif
#endif

 private:
#ifdef _WIN32
  DWORD t_[2];
#else
#if defined(__unix__) || defined(__APPLE__)
  struct timeval tv[2];
  struct timezone tz;
#else
  time_t t_[2];
#endif
#endif
};

class Mtlloader
{
public:
  vector<Material *> material_list_;
  bool loadmtl(const string filepath);
  bool find_material(string mtl_name, Material *&mtl);
};

bool Mtlloader::find_material(string mtl_name, Material *&mtl)
{
  for(int i = 0; i < material_list_.size(); i++)
  {
    if(material_list_[i]->material_name_ == mtl_name)
    {
      mtl = material_list_[i];
      return true;
    }
  }
  cout << "Error! Can not find material : " << mtl_name << endl;
  return false;
}


class Objloader
{
public:
  vector<Vec3f> vertices_;
  vector<Vec3f> normals_;
  vector<Vec2f> texcoords_;
  HitableList hitablelist_;
  Mtlloader mtl_loader_;
  vector<ModelHitable*> modellist_;
  bool loadobj(const char* filename, const char* basepath = NULL);
};


bool Objloader::loadobj(const char* filename, const char* basepath)
{
  tinyobj::attrib_t attrib;
  vector<tinyobj::shape_t> shapes;
  vector<tinyobj::material_t> materials;

  timerutil t;
  t.start();
  string warn;
  string err;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename,
                              basepath, true);
  t.end();
  printf("loading Parsing time: %lu [msecs]\n", t.msec());
  //vertices:
  for(int v = 0; v < attrib.vertices.size() / 3; v++) {
    Vec3f tmp(static_cast<const double>(attrib.vertices[3 * v + 0]),
              static_cast<const double>(attrib.vertices[3 * v + 1]),
              static_cast<const double>(attrib.vertices[3 * v + 2]));
    vertices_.push_back(tmp);
  }
  //normals:
  for(int n = 0; n < attrib.normals.size() / 3; n++) {
    Vec3f tmp(static_cast<const double>(attrib.normals[3 * n + 0]),
              static_cast<const double>(attrib.normals[3 * n + 1]),
              static_cast<const double>(attrib.normals[3 * n + 2]));
    normals_.push_back(tmp);
  }
  //texcoords:
  for(int v = 0; v < attrib.texcoords.size() / 2; v++) {
    texcoords_.push_back(Vec2f(static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1])));
  }

  //shapes:(modelhitable list)每个shape加载一个model
  //TODO: remember to modify i < shapes.size()
  //for(int i = 0; i < fmin(shapes.size(),10); i++)
  for(int i = 0; i < shapes.size(); i++)
  {
    int mtl_index = shapes[i].mesh.material_ids[0];
    string material_name = materials[mtl_index].name.c_str();
    Material *mtl_ptr;
    mtl_loader_.find_material(material_name, mtl_ptr);
    cout << "shapes[" << i << "] material: " << material_name << endl;

    ModelHitable *model = new ModelHitable(mtl_ptr);

    int index_offset = 0;
    
    //对于每个面片
    for (int f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) 
    {
      int fnum = shapes[i].mesh.num_face_vertices[f];//判断是四边形还是三角形
      if(fnum == 3)
      {
        for (int v = 0; v < fnum; v++) 
        {
          tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
          Vertex data;
          data.m_position_ = vertices_[idx.vertex_index];
          data.m_normal_ = normals_[idx.normal_index];
          if(texcoords_.size() > 0)
          {
            data.m_texcoord_ = texcoords_[idx.texcoord_index];
          }
          model->m_indices_.push_back(model->m_vertices_.size());
          model->m_vertices_.push_back(data);
        }
      }else{
        cout << "Wrong! fnum != 3\n";
      }
      index_offset += fnum;
    }
    modellist_.push_back(model);
  }

  //just for debug:
  // for(int i = 0; i < modellist_.size(); i++)
  // {
  //   cout << "model " << i << ": \n";
  //   for(int j = 0; j < modellist_[i]->m_vertices_.size(); j++)
  //   {
  //     cout << modellist_[i]->m_vertices_[j].m_position_ << endl;
  //   }
  //   for(int k = 0; k < modellist_[i]->m_indices_.size(); k++)
  //   {
  //     cout << modellist_[i]->m_indices_[k] << endl;
  //   }
  // }

  return true;
}



#endif