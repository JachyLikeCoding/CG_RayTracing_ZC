# CG_RayTracing_ZC
Computer graphics course task - ray tracing

### 程序实现功能
实现了Monte Carlo光线跟踪算法，完成了3个指定场景和一个自定场景的绘制。
1. 实现了4种不同材料；
2. 实现了八叉树加速
3. 实现了AABB包围盒
4. 实现了

### 实验环境
操作系统: MacOS
语言: C++
依赖库: tiny_obj_loader.h

### 数据结构
1. 材质(Material)
Material材质作为基类，其下继承各种不同的材质。本程序中主要实现了4类材料：自发光材料，漫反射材料，镜面材料和金属材料。
模型的材质属性信息放在.mtl文件中，借助tiny_obj_loader.h模型加载头文件解析提取。
mtl文件格式大致如下所示，材质的参数包括Kd, Ka, Ks, Tr, Ns, Ni等。
> newmtl material_name
>	- Illum
>	- Kd …
>	- Ka …
>	- Ns …
     
 因而在本程序中，我将材料划分为4种：self_illumious（自发光），diffuse（漫反射），specular（镜面反射）和transparent（投射）。材料的数据结构如下图3.1所示。全部类放在Material.h文件中。

![material_structure](/result/material_structure.png)

2. 几何物体
几何物体信息放在.obj文件中，文件格式如下所示，第一个名为default的group存放网格中点的信息（包括点的位置和法向，每个面）。
>mtllib obj_file_name
>* g default
>	- v …
>	- vt …
>	- vn …
>* s off/1
> 1. g group1_name
>	 - f …
> 2.  g group2_name
>	 - f …
>	 - …
> 3.  g ...
>	 -  …

为了比较好的表示出层次结构，同时避免代码的冗余，我是用了设计模式中的“装饰模式”。类图如下所示：

![CG_datastructure](/result/CG_datastructure.png)


Object为所有几何图形的父类，包括了寻找交点和得到表面信息方法。而MultiObject里面多了一个Object \*的队列，里面存放Object指针，所以MultiObject和Object是聚合关系，一个MultiObject可以有多个Object。而TriangleMesh则是**同种材质**的三角面片的组合，比如场景1中的球由多个面片组成。Group是多个TrangleMesh，它们会有一个名字，如pCube1。而Scene则包含了是场景中所有景物。相比于单独出每个三角面片作为，一个求交物体，这样设计的好处：

1.	加速。因为当遍历整个场景中的物体，如果该三角面片所属的group与光线没有交线，那整个group中的三角面片都不用跟该光线求交，因而可以提高光线跟踪效率。

2.	层次结构灵活。虽然在设计初期数据结构的关系应该如图3.3所示。但在实际中，MultiObject的子类中队列可以放入Object的任何子类指针。比如为调试方便，我添加了球Sphere这个数据结构，如下图所示。这个object同样可以被放入Scene的实例中。一些TriangleMesh实例同样也可以放入场景Scene的实例中。

![hierarchical_data_structure](/result/hierarchical_data_structure.png)

###	算法
**Input**: obj和mtl文件，视点坐标系和视角大小，输出图片大小，递归深度depth和每个像素点采样数目N
**Output**: 视角中的场景作为输出图片
```
1.	读入obj和mtl文件并正确放入数据结构中
2.	遍历图片中每个像素位置，在每个像素中随机选择N个点，从视点到这个点发射出光线
3.	if 当前递归深度<=N
	    求这个光线与场景中相交的最近的点，计算该点处物体的法向和UV坐标*
	else
	    return 背景颜色
4.	判断物体材质类型：
	if 自发光return ka*材质颜色
		if 散射 
			hitColor=计算与光源直接接受的光
			hitColor+=在半径为1的表面上随机选择一个方向作为发生光线方向，depth+=1，返回第3步
			hitColor*=kd
		if 反射
			计算反射光线方向
			depth+=1，返回第3步
			hitColor+=ks*反射颜色*phong高光函数
		if 折射
			计算折射光线方向，depth+=1，返回第3步
			计算反射光线方向，返回第3步
			hitColor+=tr*折射颜色+ (1-tr)*反射颜色
```
### 实验结果
- 场景1如下图所示，图像大小为400\*480。其中每列代表每个像素点取多少个采样点，每行代表最大递归深度。

  ![scene1_results](/result/scene1_results.png)

- 场景2如下图所示，图像大小为400\*600。

  ![scene2_results](/result/scene2_results.png)

### 结果分析
1.	从场景1中可以明显观察出采样点数目越大图像噪音越小，递归深度越大整体光亮度越强（从图中透明球的颜色就可以观察出来）。而在场景2中递归深度增大对场景亮度提升并不明显，大约是因为没有四周和顶部的墙壁的原因，大部分射到垂直的墙壁上的光都反射到坏境中了。
2.	其实场景1和场景2绘制的是有一定问题的。场景1中两侧墙壁颜色有些过于鲜艳。而透明的球颜色因为场景颜色比较暗，投射出的颜色是没问题的，但它的阴影中应该存在一个聚焦的高亮区域，高亮区域大概应该在下图中用红色圈标志的地方。

![analysis](/result/analysis.png)

3.	Monte Carlo的方法我用在漫反射的非直接光线(indirect light)选择光线，因而上中下三个墙壁左边偏红色，右边偏蓝色。上面墙壁颜色虽然暗但实际上有非直接光线对它的反射光存在。

### 总结
不夸张的说，我真的是跪着把这门作业写完的。这学期才开始写代码，很多东西都不记得了，复习基础知识就花了不少时间，我甚至因此复习了一遍概率论。而写代码也不是一帆风顺，从基础数据结构的构建到，到读mtl和obj文件，再到光线跟踪图像绘制，代码都是我一点一点写出来的。而由于功能不完整不能调试分块调试，使得最后调试过程bug太多而异常艰辛。特别是其中有一阵子颜色调不出来，我都一度想放弃了。还好最后还是找到了bug，使得绘制工作能顺利进行。加上我是用了双向光线跟踪方法（即已经知道光源位置了），虽然速度上会快一些，但牺牲了一定的真实感，所以其实结果我自己也并不满意。因而后续我还是会对代码进行修改，提升绘制效果。

不过便纵过程有点艰辛，但在看到绘制结果的一瞬间还是很兴奋的。而且在做这个作业的时候我确实学到了不少知识，包括加深了对指针的理解，实践了一个设计模式，和有了更深的对跟踪绘制的理解。相信这些能力和制售对以后的学习会大有帮助。
