#ifndef MESHHITABLE_H
#define MESHHITABLE_H
#include "hitable.h"

class MeshHitable : public Hitable
{
public:
    AABB m_box_;
    Material *mtl_ptr_;
    vector<Vertex> m_vertices_;
    vector<Vec3f> m_facenormal_;
    vector<unsigned int> m_indices_;
    
    MeshHitable(){}
    virtual ~MeshHitable(){}
    void setvertices(const vector<Vertex> &vertices, const vector<unsigned int> &indices)
    {
        m_vertices_ = vertices;
        m_indices_ = indices;
    }
    virtual void prerender();
    virtual bool hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const;
    virtual bool bounding_box(float t0, float t1, AABB &box) const
    {
        box = m_box_;
        return true;
    }

    bool triangle_hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord, 
                        const Vertex &p0, const Vertex &p1, const Vertex &p2, const Vec3f &normal) const;
};

bool MeshHitable::triangle_hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord, 
                        const Vertex &p0, const Vertex &p1, const Vertex &p2, const Vec3f &normal) const
{
    float n_dot_dir = normal.dotProduct(ray.direction_);
    // no intersection.
    if (fabs(n_dot_dir - 0.0) < kEpsilon)
        return false;
    float d = -normal.dotProduct(p0.m_position_);
    float t = -(normal.dotProduct(ray.origin_) + d) / n_dot_dir;
    if (t < t_min || t > t_max)
        return false;
    hitrecord.t_ = t;
    hitrecord.hitpoint_ = ray.get_t_location(t);
    hitrecord.mtl_ptr_ = mtl_ptr_;
    // judge inside or not.
    Vec3f r = hitrecord.hitpoint_ - p0.m_position_;
    Vec3f q1 = p1.m_position_ - p0.m_position_;
    Vec3f q2 = p2.m_position_ - p0.m_position_;
    float q1_squaredLen = q1.x * q1.x + q1.y * q1.y + q1.z * q1.z; 
    float q2_squaredLen = q2.x * q2.x + q2.y * q2.y + q2.z * q2.z; 
    float q1_dot_q2 = q1.dotProduct(q2);
    float r_dot_q1 = r.dotProduct(q1);
    float r_dot_q2 = r.dotProduct(q2);
    float determinant = 1.0f / (q1_squaredLen * q2_squaredLen - q1_dot_q2 * q1_dot_q2);

    float omega1 = determinant * (q2_squaredLen * r_dot_q1 - q1_dot_q2 * r_dot_q2);
    float omega2 = determinant * (-q1_dot_q2 * r_dot_q1 + q1_squaredLen * r_dot_q2);
    if (omega1 + omega2 > 1.0f || omega1 < 0.0f || omega2 < 0.0f)
    {
        return false;
    }
    hitrecord.hitnormal_ = p0.m_normal_ * (1.0f - omega1 - omega2) + p1.m_normal_ * omega1 + p2.m_normal_ * omega2;
    //TODO: CHECK TEXTURE COORD
    Vec2f texcoord = p0.m_texcoord_ * (1.0f - omega1 - omega2) + p1.m_texcoord_ * omega1 + p2.m_texcoord_ * omega2;
    hitrecord.u_ = texcoord.x;
    hitrecord.v_ = texcoord.y;
    if (hitrecord.hitnormal_.dotProduct(ray.direction_) > 0.0f)
    {
        hitrecord.hitnormal_ = -hitrecord.hitnormal_;
    }   
    return true;
}


bool MeshHitable::hit(Ray &ray, float t_min, float t_max, HitRecord &hitrecord) const
{
    HitRecord tmp_record;
    bool has_hit = false;

    float closest_now = t_max;
    for(int x = 0; x < m_indices_.size(); x += 3)
    {
        int index1 = m_indices_[x];
        int index2 = m_indices_[x+1];
        int index3 = m_indices_[x+2];
        //cout << "mesh hitable vertex index: " << index1 << ", " << index2 << ", " << index3 << endl;
        if(triangle_hit(ray, t_min, closest_now, tmp_record,
                        m_vertices_[index1], m_vertices_[index2], m_vertices_[index3], m_facenormal_[x/3]))
        {
            //cout << "triangle_hit: " << m_vertices_[index1].m_position_ << "," << m_vertices_[index2].m_position_ <<", "<<  m_vertices_[index3].m_position_ 
            //        <<", facenormal = "<< m_facenormal_[x/3] << endl;
            hitrecord = tmp_record;
            closest_now = tmp_record.t_;
            has_hit = true;
        }
    }
    return has_hit;
}

void MeshHitable::prerender()
{
    //calculate AABB bounding box
    if(m_indices_.empty())
        return;
    Vec3f min_point(+MAXFLOAT, +MAXFLOAT, +MAXFLOAT);
    Vec3f max_point(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);

    for(int i = 0; i < m_vertices_.size(); i++)
    {
        min_point.x = fmin(min_point.x, m_vertices_[i].m_position_.x);
        min_point.y = fmin(min_point.y, m_vertices_[i].m_position_.y);
        min_point.z = fmin(min_point.z, m_vertices_[i].m_position_.z);
        max_point.x = fmin(max_point.x, m_vertices_[i].m_position_.x);
        max_point.y = fmin(max_point.y, m_vertices_[i].m_position_.y);
        max_point.z = fmin(max_point.z, m_vertices_[i].m_position_.z);
    }

    if(fabs(max_point.x - min_point.x) < kEpsilon)
    {
        min_point.x -= 0.0001;
        max_point.x += 0.0001;
    }

    if(fabs(max_point.y - min_point.y) < kEpsilon)
    {
        min_point.y -= 0.0001;
        max_point.y += 0.0001;
    }

    if(fabs(max_point.z - min_point.z) < kEpsilon)
    {
        min_point.z -= 0.0001;
        max_point.z += 0.0001;
    }

    //face normal
    for(int x = 0; x < m_indices_.size(); x+=3)
    {
        int index0 = m_indices_[x];
        int index1 = m_indices_[x+1];
        int index2 = m_indices_[x+2];
        Vec3f normal = (m_vertices_[index1].m_position_ - m_vertices_[index0].m_position_)
                    .crossProduct(m_vertices_[index2].m_position_ - m_vertices_[index0].m_position_);
        normal.normalize();
        m_facenormal_.push_back(normal);
    }
    m_box_ = AABB(min_point, max_point);

    //如果超过100个三角形就建树
    if(m_indices_.size() > 300)
    {
        //build octree
    }
    else
    {

    }
}

#endif