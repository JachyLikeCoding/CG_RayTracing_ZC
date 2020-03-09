 #ifndef MATERIAL_H
 #define MATERIAL_H

 #include "basic.h"
 #include "hitable.h"

// class MaterialAttribute
// {
// public:
//     MaterialAttribute(){}
// };

// class Kd : public MaterialAttribute
// {
//     /*
//     Kd:使用RGB值指定漫反射率。
//     “ r g b”是大气中红色，绿色和蓝色分量的值。 g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Kd():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// };

// class Ka : public MaterialAttribute
// {
//     /*
//     Ka:使用RGB值指定环境反射率。
//     “ r g b”是该颜色的红色，绿色和蓝色分量的值。 g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。 r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Ka():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// }; 

// class Ks : public MaterialAttribute
// {
//     /*
//     Ks:使用RGB值指定镜面反射率。 
//     “r g b”是大气中红色，绿色和蓝色分量的值。g和b参数是可选的。 
//     如果仅指定r，则假定g和b等于r。r g b值通常在0.0到1.0的范围内。 
//     超出此范围的值会相应地增加或降低反射率。
//     */
// public:
//     float r_, g_, b_;
//     Ks():MaterialAttribute()
//     {
//         r_ = 0.0;
//         g_ = 0.0;
//         b_ = 0.0;
//     }
// };

// class Tf :public MaterialAttribute
// {
// 	/*
//     Tf：透射率。“r g b”是大气中红色，绿色和蓝色分量的值。 
//     g和b参数是可选的。 如果仅指定r，则假定g和b等于r。
//     r g b值通常在0.0到1.0的范围内。超出此范围的值会相应增加或降低反射率。
//     */
// public:
// 	float r_, g_, b_;
// 	Tf() :MaterialAttribute() 
// 	{
// 		r_ = 0.0;
// 		g_ = 0.0;
// 		b_ = 0.0;
// 	}
// };

// class Ns:public MaterialAttribute
// {
//     /*
//     Ns:指定当前材质的镜面反射指数。 
//     这定义了镜面高光的焦点。 “指数”是镜面指数的值。 
//     高指数导致紧密，集中的高光。 Ns值通常在0到1000之间。
//     */
// public:
//     float exponent_;
//     Ns():MaterialAttribute()
//     {
//         exponent_ = 1;
//     }
// };

// class Tr:public MaterialAttribute
// {
//     /*
//     Tr:当前材料的透明率。 
//     如果Tr = 0，则表示它是完全不透明的。 
//     值越高，材料越透明。 Tr值通常在0.0到1.0之间。
//     */
// public:
//     float ratio_;
//     Tr():MaterialAttribute()
//     {
//         ratio_ = 0;
//     }
// };

// class Ni:public MaterialAttribute
// {
//     /*
//     Ni:光密度
//     指定表面的光密度。也称为折射率。值的范围是0.001至10。
//     值1.0表示光在通过时不会弯曲，通过增加物体的光密度会增加弯曲量。
//     玻璃的折射率约为1.5。小于1.0的值会产生奇怪的结果，因此不建议使用。
//     */
// public:
//     float optical_dencity_;
//     Ni():MaterialAttribute()
//     {
//         optical_dencity_ = 1;
//     }
// };

// class Material
// {
// public:
//     string mtlname_;
//     int illum_;
//     Kd kd_;
//     Ka ka_;
//     Ks ks_;
//     Ns ns_;
//     Tf tf_;
//     Tr tr_;
//     Ni ni_;

//     bool self_luminous_;
//     bool transparent_;
//     bool specular_;
//     bool diffuse_;

//     Material(){}
//     Material(string mtlname)
//     {
//         mtlname_ = mtlname;
//         self_luminous_ = false;
//         transparent_ = false;
//         specular_ = false;
//         diffuse_ = false;
//     }

// };

// #endif

class Material
{
public:
    virtual bool scatter(Ray &ray_in, HitRecord &record, Vec3f &attenuation, Ray &scattered) const = 0;
};


#endif