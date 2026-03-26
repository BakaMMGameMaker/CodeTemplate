#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

// AABB 包围盒
/*

AABB
优点
* 计算快，相交测试简单
* 存储开销小，只要两个三维向量：min 和 max
* 更新容易，对静态物体尤其方便
* 适合做粗略碰撞检测，射线-AABB、AABB-AABB 检测很高效
* 缓存/并行计算友好。实现简单，工程上性价比高

缺点
* 物体旋转时包围变松，由于盒子必须和坐标轴对齐，物体一旋转，AABB 会膨胀很多
* 方向适应性差，对细长、斜放、旋转明显的模型包围不紧
* 误判较多，包围不紧会带来更多后续精测开销

适用场景
* 粗阶段碰撞检测
* BVH 节点包围体
* 射线加速结构
* 静态模型或旋转不频繁的对象
*

包围球

优点
* 旋转不影响包围体，对动态旋转物体方便
* 相交测试简单，球-球检测只需要比较中心距和半径和，适合快速粗筛
* 方向无关，不存在物体旋转范围变大的问题
* 插值和动态更新简单，对运动物体、角色、粒子等很省事

缺点
* 包围不紧，尤其是长条状、扁平状、盒状物体，空余空间会很大
* 误判多，包围往往偏松，进入精测阶段的次数可能增加
* 对场景划分能力弱，它是单个对象的包围体，不像八叉树那样能组织整个空间

适用场景
* 旋转频繁的动态物体
* 快速粗碰撞
* 角色近似、粒子、简单物理对象
* 视锥裁剪中的快速预判

### 八叉树
* 本质：空间划分结构
* 强项：管理大规模场景、加速查询
* 弱项：维护复杂，不适合直接做精细包围

### AABB
* 本质：单物体包围体
* 强项：计算快、实现简单、工程上通用
* 弱项：旋转后包围变松

### 包围球
* 本质：单物体包围体
* 强项：旋转稳定、检测简单
* 弱项：对非球形物体通常不够紧
*/

using std::vector;

constexpr float INF = std::numeric_limits<float>::infinity();
constexpr float EPS = 1e-8f;

// 向量
struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}

    Vec3 operator +(const Vec3 &rhs) const { return Vec3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vec3 operator -(const Vec3 &rhs) const { return Vec3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vec3 operator *(float s) const { return Vec3(x * s, y * s, z * s); }
    Vec3 operator /(float s) const { return Vec3(x / s, y / s, z / s); }

    float operator[](int axis) const {
        if (axis == 0) return x;
        if (axis == 1) return y;
        return z;
    }
};

inline Vec3 minVec(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        std::min(a.x, b.x),
        std::min(a.y, b.y),
        std::min(a.z, b.z)
    );
}

inline Vec3 maxVec(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        std::max(a.x, b.x),
        std::max(a.y, b.y),
        std::max(a.z, b.z)
    );
}

inline float dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

struct Ray {
    Vec3 origin; // 发射点
    Vec3 dir;    // 方向

    Ray() {}
    Ray(const Vec3 &o, const Vec3 &d) : origin(o), dir(d) {}
};

struct AABB {
    Vec3 min; // xmin ymin zmin 左下角
    Vec3 max; // xmax ymax zmax 右上角

    AABB() {
        min = Vec3(INF, INF, INF);
        max = Vec3(-INF, -INF, -INF);
    }

    AABB(const Vec3 &mn, const Vec3 &mx) : min(mn), max(mx) {}

    void expand(const Vec3 &p) {
        min = minVec(min, p);
        max = maxVec(max, p);
    }

    void expand(const AABB &box) {
        min = minVec(min, box.min);
        max = maxVec(max, box.max);
    }

    // 盒中心点
    Vec3 centroid() const {
        return (min + max) * 0.5f;
    }

    // 左下角指向右上角的向量
    Vec3 extent() const {
        return max - min;
    }

    // 最长轴
    int longestAxis() const {
        Vec3 e = extent();
        if (e.x >= e.y && e.x >= e.z) return 0;
        if (e.y >= e.x && e.y >= e.z) return 1;
        return 2;
    }

    // 光线 ray 是否与盒体 AABB 相交
    bool intersect(const Ray &ray, float tMin, float tMax) const {
        for (int axis = 0; axis < 3; ++axis) {
            float invD = 1.0f / ray.dir[axis];
            float t0 = (min[axis] - ray.origin[axis]) * invD;
            float t1 = (max[axis] - ray.origin[axis]) * invD;

            if (invD < 0.0f) std::swap(t0, t1);

            tMin = std::max(tMin, t0);
            tMax = std::min(tMax, t1);

            if (tMax < tMin) return false;
        }
        return true;
    }
};

// 三角形
struct Triangle {
    Vec3 v0, v1, v2; // 顶点

    Triangle() {}
    Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c) : v0(a), v1(b), v2(c) {}

    // 三角形包围盒
    AABB bounds() const {
        AABB box;
        box.expand(v0);
        box.expand(v1);
        box.expand(v2);
        return box;
    }

    // 三角形中心
    Vec3 centroid() const {
        return (v0 + v1 + v2) / 3.0f;
    }
};

struct HitInfo {
    float t;
    int triIndex;
    Vec3 bary; // 可选：存重心坐标

    HitInfo() : t(INF), triIndex(-1), bary() {}
};

inline bool intersectTriangle(const Ray &ray, const Triangle &tri, float &tHit, Vec3 &bary) {
    // Moller-Trumbore
    Vec3 e1 = tri.v1 - tri.v0;
    Vec3 e2 = tri.v2 - tri.v0;

    Vec3 p = cross(ray.dir, e2);
    float det = dot(e1, p);

    if (std::fabs(det) < EPS) return false;
    float invDet = 1.0f / det;

    Vec3 s = ray.origin - tri.v0;
    float u = dot(s, p) * invDet;
    if (u < 0.0f || u > 1.0f) return false;

    Vec3 q = cross(s, e1);
    float v = dot(ray.dir, q) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;

    float t = dot(e2, q) * invDet;
    if (t < EPS) return false;

    tHit = t;
    bary = Vec3(1.0f - u - v, u, v);
    return true;
}

struct Primitive {
    Triangle tri;
    AABB box;
    Vec3 center;
    int originalIndex;
};

struct BVHNode {
    AABB box;

    // 左右子节点的索引
    int left = -1;
    int right = -1;

    int start = 0; // 叶子节点中图元起始位置
    int count = 0; // 叶子节点中图元数量

    bool isLeaf() const {
        return left == -1 && right == -1;
    }
};

class BVH {
public:
    explicit BVH(const vector<Triangle> &triangles, int maxLeafSize = 2)
        : m_maxLeafSize(maxLeafSize) {
        m_primitives.reserve(triangles.size());
        for (int i = 0; i < triangles.size(); ++i) {
            Primitive p;
            p.tri = triangles[i];
            p.box = triangles[i].bounds();
            p.center = triangles[i].centroid();
            p.originalIndex = i;
            m_primitives.push_back(p);
        }

        if (!m_primitives.empty()) {
            buildRecursive(0, m_primitives.size());
        }
    }

    bool intersect(const Ray &ray, HitInfo &hit) const {
        if (m_nodes.empty()) return false;
        return intersectNode(0, ray, hit);
    }

    void printSummary() const {
        std::cout << "BVH nodes: " << m_nodes.size() << "\n";
        std::cout << "Primitives: " << m_primitives.size() << "\n";
    }

private:
    vector<Primitive> m_primitives; // 所有素体
    vector<BVHNode> m_nodes;        // 所有节点
    int m_maxLeafSize = 2;

    int buildRecursive(int start, int end) {
        int nodeIndex = m_nodes.size();
        m_nodes.push_back(BVHNode{});

        AABB nodeBox;
        AABB centroidBox;
        for (int i = start; i < end; ++i) {
            nodeBox.expand(m_primitives[i].box);
            centroidBox.expand(m_primitives[i].center);
        }

        m_nodes[nodeIndex].box = nodeBox; // 写入 push back 的 BVHNode{}

        int count = end - start;

        if (count <= m_maxLeafSize) {
            m_nodes[nodeIndex].start = start;
            m_nodes[nodeIndex].count = count;
            return nodeIndex;
        }

        int axis = centroidBox.longestAxis();
        int mid = (start + end) / 2;

        std::nth_element(
            m_primitives.begin() + start,
            m_primitives.begin() + mid,
            m_primitives.begin() + end,
            [axis](const Primitive &a, const Primitive &b) {
                return a.center[axis] < b.center[axis];
            }
        );

        if (mid == start || mid == end) {
            m_nodes[nodeIndex].start = start;
            m_nodes[nodeIndex].count = count;
            return nodeIndex;
        }

        int leftChild = buildRecursive(start, mid);
        int rightChild = buildRecursive(mid, end);

        m_nodes[nodeIndex].left = leftChild;
        m_nodes[nodeIndex].right = rightChild;

        return nodeIndex;
    }

    bool intersectNode(int nodeIndex, const Ray &ray, HitInfo &hit) const {
        const BVHNode &node = m_nodes[nodeIndex];

        if (!node.box.intersect(ray, EPS, hit.t)) return false; // 不与当前节点相交

        if (node.isLeaf()) {
            bool hasHit = false;
            for (int i = node.start; i < node.start + node.count; ++i) {
                float t;
                Vec3 bary;
                if (intersectTriangle(ray, m_primitives[i].tri, t, bary)) {
                    if (t < hit.t) {
                        hit.t = t;
                        hit.triIndex = m_primitives[i].originalIndex;
                        hit.bary = bary;
                        hasHit = true;
                    }
                }
            }
            return hasHit;
        }

        // 不是子节点，递归左右
        bool hitLeft = intersectNode(node.left, ray, hit);
        bool hitRight = intersectNode(node.right, ray, hit);

        return hitLeft || hitRight;
    }
};
