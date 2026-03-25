#pragma once
#include <vector>
using namespace std;

// 八叉树

/*
 * 优点
 * - 适合大规模空间查询，做视锥裁剪、范围查询、邻近搜索、粗略碰撞检测时很高效
 * - 能跳过大片空区域，稀疏场景里收益很大，尤其是地形、体素、静态场景
 * - 层次化管理强，查询时可以快速排除整块空间，而不是逐个物体检查
 * - 适合静态或低频更新场景
 * 缺点
 * - 构建和维护复杂，比包围体复杂，实现、调试、参数选择都更麻烦
 * - 动态物体更新成本高，物体移动后要重新插入或调整节点
 * - 对象跨多个节点，大物体或细长物体不容易放，可能导致重复存储或挂在高层节点
 * - 性能依赖场景分布，场景很密集、分布不均匀时树会退化或不够平衡
 * - 有额外内存开销，节点指针、层次信息等
 * 适合
 * - 大型静态场景
 * - 稀疏三维空间
 * - 场景管理、可见性裁剪、粗粒度碰撞检测
 */

// 三维空间中的点
struct Point {
    double x, y, z;
};

// 八叉树节点
struct OctreeNode {
    double cx, cy, cz; // 节点的中心
    double half;       // 节点覆盖正方体的半长

    vector<Point> points;    // 节点包含的点
    OctreeNode *children[8]; // 节点的八个孩子
    bool divided;            // 是否已被分割

    OctreeNode(double x, double y, double z, double h)
        : cx(x), cy(y), cz(z), half(h), divided(false) {
        for (int i = 0; i < 8; ++i) children[i] = nullptr;
    }

    // Point p 是否属于该 OctreeNode
    bool contains(const Point &p) const {
        return p.x >= cx - half && p.x < cx + half &&
               p.y >= cy - half && p.y < cy + half &&
               p.z >= cz - half && p.z < cz + half;
    }

    // Point p 属于 OctreeNode 的哪一个孩子
    int getChildIndex(const Point &p) const {
        int index = 0;
        if (p.x >= cx) index |= 1;
        if (p.y >= cy) index |= 2;
        if (p.z >= cz) index |= 4;
        return index;
    }

    // 分割
    void subdivide() {
        double q = half / 2.0;
        for (int i = 0; i < 8; ++i) {
            double nx = cx + ((i & 1) ? q : -q);
            double ny = cy + ((i & 2) ? q : -q);
            double nz = cz + ((i & 4) ? q : -q);
            children[i] = new OctreeNode(nx, ny, nz, q);
        }
        divided = true;
    }

    void insert(const Point &p, int capacity = 2, int depth = 0, int maxDepth = 8) {
        if (!contains(p)) return; // 如果节点不包含点 p，返回

        // 如果当前节点未被分割且还能存
        if (!divided && (points.size() < capacity || depth >= maxDepth)) {
            points.push_back(p);
            return;
        }

        // 容量不足，如果还没细分，进行细分
        if (!divided) {
            subdivide();
            // 把自己的点复制到孩子里
            for (const auto &oldPoint : points) {
                int idx = getChildIndex(oldPoint);
                children[idx]->insert(oldPoint, capacity, depth + 1, maxDepth);
            }
            points.clear(); // 自己不再占有点
        }

        int idx = getChildIndex(p);
        children[idx]->insert(p, capacity, depth + 1, maxDepth);
    }
};
