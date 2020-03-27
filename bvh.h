#ifndef BVH_H
#define BVH_H

#include "hitable.h"

class BVH_node : public Hitable
{
public:
    Hitable *left_;
    Hitable *right_;
    AABB box_;

    BVH_node(){}
    //BVH_node(int size, Hitable **list, float t0, float t1);
    BVH_node(vector<Hitable *> &list, int start, int end);

    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const;
    AABB surrounding_box(AABB &box0, AABB &box1) const;
    Hitable *getleftchild() { return left_; }
    Hitable *getrightchild() { return right_; }
    virtual bool isleaf() const { return false; }
    static void destroy_tree(Hitable *root);
};



//获取包围盒
bool BVH_node::bounding_box(float t0, float t1, AABB &box) const
{
    box = box_;
    return true;
}

//重构包围盒
AABB BVH_node::surrounding_box(AABB &box0, AABB &box1) const
{
    Vec3f boxmin(fmin(box0.getmin()[0], box1.getmin()[0]),
                fmin(box0.getmin()[1], box1.getmin()[1]),
                fmin(box0.getmin()[2], box1.getmin()[2]));
    Vec3f boxmax(fmax(box0.getmax()[0], box1.getmax()[0]),
            fmax(box0.getmax()[1], box1.getmax()[1]),
            fmax(box0.getmax()[2], box1.getmax()[2]));
    return AABB(boxmin, boxmax);
}

//判断包围盒（树的一个结点）是否与光线发生碰撞
bool BVH_node::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    if(box_.hit(t_min, t_max, ray))
    {
        //检验左右子树是否有碰撞，更新碰撞信息
        HitRecord left_record, right_record;
        bool is_left_hit = left_->hit(ray, t_min, t_max, left_record);
        bool is_right_hit = right_->hit(ray, t_min, t_max, right_record);
        //左右都撞，撞到了重叠部分，取较近的碰撞点
        if(is_left_hit && is_right_hit)
        {
            hitrecord = (left_record.t_ < right_record.t_)?(left_record):(right_record);
            return true;
        }
        else if(is_left_hit)
        {
            hitrecord = left_record;
            return true;
        }
        else if(is_right_hit)
        {
            hitrecord = right_record;
            return true;
        }
        else
        {
            return false;
        }
    }
    else{
        return false;
    }  
}

int compare_x(const void *a, const void *b)
{
    AABB leftbox, rightbox;
    Hitable *hita = *(Hitable**)a;
    Hitable *hitb = *(Hitable**)b;
    if(!hita->bounding_box(0,0,leftbox) || !hitb->bounding_box(0,0,rightbox))
    {
        cerr << "BVH node has no bounding box." << endl;
    }
    if(leftbox.min_.x - rightbox.min_.x < 0.0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
    
}

int compare_y(const void *a, const void *b)
{
    AABB leftbox, rightbox;
    Hitable *hita = *(Hitable**)a;
    Hitable *hitb = *(Hitable**)b;
    if(!hita->bounding_box(0,0,leftbox) || !hitb->bounding_box(0,0,rightbox))
    {
        cerr << "BVH node has no bounding box." << endl;
    }
    if(leftbox.min_.y - rightbox.min_.y < 0.0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
    
}

int compare_z(const void *a, const void *b)
{
    AABB leftbox, rightbox;
    Hitable *hita = *(Hitable**)a;
    Hitable *hitb = *(Hitable**)b;
    if(!hita->bounding_box(0,0,leftbox) || !hitb->bounding_box(0,0,rightbox))
    {
        cerr << "BVH node has no bounding box." << endl;
    }
    if(leftbox.min_.z - rightbox.min_.z < 0.0)
    {
        return -1;
    }
    else
    {
        return 1;
    }
    
}


/** BVH树构建
 * 1) randomly choose an axis
 * 2) sort the primitives using library qsort
 * 3) put half in each subtree
 * */
// BVH_node::BVH_node(int size, Hitable **list, float t0, float t1)
// {
//     //随机选一个轴
//     int axis = int(3 * rand());
//     if(axis == 0)//x:
//         qsort(list, size, sizeof(Hitable *), compare_x);
//     else if(axis == 1)//y:
//         qsort(list, size, sizeof(Hitable *), compare_y);
//     else//z:
//         qsort(list, size, sizeof(Hitable *), compare_z);
    
//     if(size == 1)
//     {
//         left_ = right_ = list[0];
//     }
//     else if(size == 2)
//     {
//         left_ = list[0];
//         right_ = list[1];
//     }
//     else
//     {
//         left_ = new BVH_node(size/2, list, t0, t1);
//         right_ = new BVH_node(size-size/2, list+size/2, t0, t1);
//     }
//     AABB leftbox, rightbox;
//     if(!left_->bounding_box(t0, t1, leftbox) || !right_->bounding_box(t0, t1, rightbox))
//     {
//         cerr << "BVH node has no bounding box." << endl;
//     }
//     box_ = surrounding_box(leftbox, rightbox);
//     cout << "BVH contruction finished!" << endl;
// }


BVH_node::BVH_node(vector<Hitable *> &list, int start, int end)
{
    //随机选一个轴
    int axis = int(3 * rand());
    if(axis == 0)
        sort(&list[start], &list[end], compare_x);
    else if(axis == 1)
        sort(&list[start], &list[end], compare_y);
    else if(axis == 2)
        sort(&list[start], &list[end], compare_z);

    int size = end - start;
    if(size == 1)
    {
        left_ = right_ = list[start];
    }
    else if(size == 2)
    {
        left_ = list[start];
        right_ = list[start + 1];
    }
    else
    {
        left_ = new BVH_node(list, start, start + size/2);
        right_ = new BVH_node(list, start + size/2, end);
    }
    AABB leftbox, rightbox;
    if(!left_->bounding_box(0, 0, leftbox) || !right_->bounding_box(0, 0, rightbox))
    {
        cerr << "BVH node has no bounding box." << endl;
    }
    box_ = surrounding_box(leftbox, rightbox);
    cout << "BVH contruction finished!" << endl;
}

void BVH_node::destroy_tree(Hitable *root)
{
    if(root == nullptr || root->isleaf())
    {
        return;
    }
    BVH_node *broot = reinterpret_cast<BVH_node *>(root);
    destroy_tree(broot->left_);
    destroy_tree(broot->right_);
    delete broot;
    root = nullptr;
}
#endif