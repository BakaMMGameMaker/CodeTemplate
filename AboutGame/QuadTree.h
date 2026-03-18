#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <cmath>

// 四叉树法查询玩家附近物品

// 查询快的原因 - 每个节点维护一个矩形区域，如果圆和该节点不相交，那么整一颗子树都能直接剪掉，只有相邻的节点才会继续往下搜索

// 功能表述：节点容量满了就分裂，先删后插，支持动态点对象

class QuadTree {
    struct Object {
        int id;
        double x;
        double y;
    };

    // 表示平面上的一个矩形区域
    struct Rect {
        double minX, minY, maxX, maxY; // 矩形边界

        // 点是否在矩形内 / 边界上
        bool contains(double x, double y) const {
            return x >= minX && x <= maxX && y >= minY && y <= maxY;
        }

        // 判断矩形是否与给定圆相交
        bool intersectsCircle(double cx, double cy, double r) const {
            // 核心思路 - 找出矩形中离圆心最近的点，看这个最近的点到圆心的距离是否不超过半径

            // 矩形内里圆心最近的点
            double closestX = std::max(minX, std::min(cx, maxX)); // cx 在矩形左右边界之间，结果是 cx；在左边，结果是 minX；在右边，结果是 maxX
            double closestY = std::max(minY, std::min(cy, maxY)); // cy 在矩形上下边界之间，结果是 cy；在上面，结果是 minY；在右边，结果是 maxY

            // 最近点到圆心的距离平方
            double dx = closestX - cx;
            double dy = closestY - cy;
            return dx * dx + dy * dy <= r * r;
        }
    };

    // 四叉树节点
    struct Node {
        Rect bounds;  // 当前节点对应矩形范围
        int capacity; // 当前节点最多能容纳多少元素 - 如果当前节点存的点数还没有超过 capacity，就放在当前节点，否则细分成四个子节点
        bool divided; // 此节点是否已经被分裂 - false => 还是叶子节点，true => 已经被切成四个子区域了

        std::vector<int> ids; // 当前节点存了哪些物体

        // 四个子节点 - 使用 unique_ptr: 子节点动态创建，父节点独占其子节点，无需手写析构
        std::unique_ptr<Node> nw; // 左上
        std::unique_ptr<Node> ne; // 右上
        std::unique_ptr<Node> sw; // 左下
        std::unique_ptr<Node> se; // 右下

        Node(const Rect &rect, int cap) : bounds(rect), capacity(cap), divided(false) {}

        // 细分节点为四个子节点
        void subdivide() {
            // 当前矩形的中心点
            double midX = (bounds.minX + bounds.maxX) * 0.5;
            double midY = (bounds.minY + bounds.maxY) * 0.5;

            nw = std::make_unique<Node>(Rect{bounds.minX, midY, midX, bounds.maxY}, capacity);
            ne = std::make_unique<Node>(Rect{midX, midY, bounds.maxX, bounds.maxY}, capacity);
            sw = std::make_unique<Node>(Rect{bounds.minX, bounds.minY, midX, midY}, capacity);
            se = std::make_unique<Node>(Rect{midX, bounds.minY, bounds.maxX, midY}, capacity);

            divided = true;
        }
    };

    std::unique_ptr<Node> root;
    std::unordered_map<int, Object> objects;
    int nodeCapacity; // 每个节点的容量

    // 把编号为 id 的物品插入到给定的节点中
    bool insert(Node *node, int id) {
        const Object &obj = objects[id];
        if (!node->bounds.contains(obj.x, obj.y)) return false; // 这个物体根本不在节点范围内

        // 如果节点还没有被细分成四个子节点，并且当前容量还充足
        if (!node->divided && node->ids.size() < node->capacity) {
            node->ids.push_back(id);
            return true;
        }

        // 如果节点还没有被细分，但是容量不够了
        if (!node->divided) {
            node->subdivide(); // 分裂节点 (注意只是创建子节点指针，还没有搬运数据)

            // 旧节点中所有的物品 ids 列表
            std::vector<int> oldIds;
            oldIds.swap(node->ids);

            for (int oldId : oldIds) {
                bool ok = insertIntoChildren(node, oldId); // 尝试插入到四个子节点中的其中一个。只要有一个成功，ok 就为真
                if (!ok) node->ids.push_back(oldId);       // 如果四个子节点都插入失败 (理论上不会)，那么插入回自己的 vector 里
            }
        }

        // 节点已被细分，直接尝试在子节点插入原本打算插入的物品
        if (insertIntoChildren(node, id)) return true;

        // 子节点插入失败，回退并插入到自己的列表中
        node->ids.push_back(id);
        return true;
    }

    bool insertIntoChildren(Node *node, int id) {
        return insert(node->nw.get(), id) ||
               insert(node->ne.get(), id) ||
               insert(node->sw.get(), id) ||
               insert(node->se.get(), id);
    }

    // 把编号为 id，坐标为 xy 的物品从给定节点中移除
    bool remove(Node *node, int id, double x, double y) {
        if (!node || !node->bounds.contains(x, y)) return false;

        // 遍历给定节点所包含的物品 id
        for (int i = 0; i < node->ids.size(); ++i) {
            // 找到了这个物体
            if (node->ids[i] == id) {
                // trick
                node->ids[i] = node->ids.back();
                node->ids.pop_back();
                return true;
            }
        }

        // 节点不包含物体，而节点又没有发生细分，说明物体不存在
        if (!node->divided) return false;

        // 尝试从子节点中删除这个物体
        return remove(node->nw.get(), id, x, y) ||
               remove(node->ne.get(), id, x, y) ||
               remove(node->sw.get(), id, x, y) ||
               remove(node->se.get(), id, x, y);
    }

    // 查询给定原型范围内的物品 id
    void query(Node *node, double cx, double cy, double r, double r2, std::vector<int> &result) const {
        if (!node || !node->bounds.intersectsCircle(cx, cy, r)) return; // 节点不存在或者节点和给定圆没有相交

        // 遍历节点包含的物体
        for (int id : node->ids) {
            const Object &obj = objects.at(id);
            double dx = obj.x - cx;
            double dy = obj.y - cy;
            // 如果节点在圆的范围内
            if (dx * dx + dy * dy <= r2) result.push_back(id);
        }

        // 没有子节点，可以返回了
        if (!node->divided) return;

        // 收集子节点中位于圆形范围内的物体
        query(node->nw.get(), cx, cy, r, r2, result);
        query(node->ne.get(), cx, cy, r, r2, result);
        query(node->sw.get(), cx, cy, r, r2, result);
        query(node->se.get(), cx, cy, r, r2, result);
    }

public:
    QuadTree(double minX, double minY, double maxX, double maxY, int capacity = 4)
        : root(std::make_unique<Node>(Rect{minX, minY, maxX, maxY}, capacity)), nodeCapacity(capacity) {}

    // 插入编号为 id，坐标为 xy 的物体
    bool insert(int id, double x, double y) {
        if (objects.count(id)) return false; // 物体已经存在

        if (!root->bounds.contains(x, y)) return false; // 坐标不存在于这个世界

        objects[id] = {id, x, y}; // 创建新的物体

        // 尝试插入到根节点中，如果容量超了，insert 会自动往下插
        if (!insert(root.get(), id)) {
            objects.erase(id); // 插入失败了，把物体删掉
            return false;
        }
        return true;
    }

    // 移除编号为 id 的物体
    bool remove(int id) {
        auto it = objects.find(id);
        if (it == objects.end()) return false; // 物体不存在

        // 获取物体的坐标
        double x = it->second.x;
        double y = it->second.y;
        bool ok = remove(root.get(), id, x, y); // 尝试在树中删除这个物体
        if (ok) objects.erase(it);              // 删除成功，在物品列表中也删除这个物体

        return ok;
    }

    // 更新编号为 id 的物品的坐标
    bool update(int id, double newX, double newY) {
        auto it = objects.find(id);
        if (it == objects.end()) return false; // 物品不存在

        if (!root->bounds.contains(newX, newY)) return false; // 新坐标非法

        // 物体的旧坐标
        double oldX = it->second.x;
        double oldY = it->second.y;

        if (!remove(root.get(), id, oldX, oldY)) return false; // 尝试从树中移除

        // 移除成功，设置物体的新坐标
        it->second.x = newX;
        it->second.y = newY;

        // 把移动过后的物体重新插入到树中
        if (!insert(root.get(), id)) {
            // 如果插入失败，还原为旧状态
            it->second.x = oldX;
            it->second.y = oldY;
            insert(root.get(), id);
            return false;
        }

        return true;
    }

    // 收集以 xy 为圆心，radius 为半径的圆中的所有物品 id
    std::vector<int> queryRadius(double x, double y, double radius) const {
        std::vector<int> result;
        double r2 = radius * radius;
        query(root.get(), x, y, radius, r2, result);
        return result;
    }
};
