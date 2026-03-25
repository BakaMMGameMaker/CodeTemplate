#pragma once
#include <vector>
using namespace std;

// 八叉树

/*
 * 优点
 * - 适合大场景空间管理，能快速缩小查询范围，比如视锥裁剪、范围查询、邻域搜索、射线粗筛
 * - 层次结构清晰，大量物体分布在大空间里时，可以减少无关检测
 * - 对稀疏场景友好，空旷区域不会像均匀网格那样浪费太多查询成本
 * - 便于做 LOD、可见性裁剪，游戏引擎、体素场景、点云处理中很常见。
 * 缺点
 * - 构建和维护复杂，动态物体多时，插入、删除、重建、节点拆分/合并比较麻烦
 * - 物体跨节点，大物体可能横跨多个子块，要重复存储或挂在父节点，影响效率
 * - 树不平衡时性能退化，物体分布不均匀时，局部层级很深
 * - 空间管理结构，不是直接的精确包围体，做两物体精细碰撞时，要配合 AABB、OBB、包围球等
 * 适用场景
 * - 大型静态或半静态 3D 场景
 * - 点云、体素、地图分块
 * - 空间查询、可见性裁剪、粗阶段碰撞检测
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
