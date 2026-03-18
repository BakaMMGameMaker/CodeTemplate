#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <cstdint>

// 网格法快速查询在玩家附近的物品

class SpatialHashGrid {
    // 物体
    struct Object {
        int id;   // 物品 id
        double x; // x 坐标
        double y; // y 坐标
    };

    // 网格坐标
    struct CellCoord {
        int cx;
        int cy;

        bool operator==(const CellCoord &other) const {
            return cx == other.cx && cy == other.cy;
        }
    };

    struct CellCoordHash {
        size_t operator()(const CellCoord &c) const {
            return (static_cast<size_t>(static_cast<uint32_t>(c.cx)) << 32) ^
                   static_cast<uint32_t>(c.cy);
        }
    };

    double cellSize; // 网格大小

    std::unordered_map<CellCoord, std::unordered_set<int>, CellCoordHash> grid; // 每个格子里有哪些物体 id

    std::unordered_map<int, Object> objects; // id -> 物体信息

    int getCellX(double x) const {
        return static_cast<int>(floor(x / cellSize));
    }

    int getCellY(double y) const {
        return static_cast<int>(floor(y / cellSize));
    }

    CellCoord getCell(double x, double y) const {
        return {getCellX(x), getCellY(y)};
    }

public:
    explicit SpatialHashGrid(double cellSize) : cellSize(cellSize) {}

    // 插入新物体
    bool insert(int id, double x, double y) {
        if (objects.count(id)) return false; // 已存在

        objects[id] = {id, x, y};       // 哈希表中记录物体
        CellCoord cell = getCell(x, y); // 物体所在的网格
        grid[cell].insert(id);          // 添加新物体到网格所包含的物体
        return true;
    }

    // 删除物体
    bool remove(int id) {
        auto it = objects.find(id);
        if (it == objects.end()) return false; // 没有这个物体 id

        const Object &obj = it->second;         // 具体物体
        CellCoord cell = getCell(obj.x, obj.y); // 物体所在网格

        auto gridIt = grid.find(cell);
        if (gridIt != grid.end()) {
            gridIt->second.erase(id);                       // 网格包含的物体去掉这个物体
            if (gridIt->second.empty()) grid.erase(gridIt); // 如果网格空了，删除这个网格
        }

        objects.erase(it);
        return true;
    }

    // 更新物体位置
    bool update(int id, double newX, double newY) {
        auto it = objects.find(id);
        if (it == objects.end()) return false;

        Object &obj = it->second;
        CellCoord oldCell = getCell(obj.x, obj.y);
        CellCoord newCell = getCell(newX, newY);

        // 如果物体跑到另一个网格里了
        if (!(oldCell == newCell)) {
            auto gridIt = grid.find(oldCell);
            if (gridIt != grid.end()) {
                gridIt->second.erase(id);
                if (gridIt->second.empty()) grid.erase(gridIt);
            }
            grid[newCell].insert(id);
        }

        obj.x = newX;
        obj.y = newY;
        return true;
    }

    // 查询圆形范围内所有物体 id
    std::vector<int> queryRadius(double px, double py, double radius) const {
        std::vector<int> result;
        double radiusSquared = radius * radius;

        // 获取包围盒边界
        int minCellX = getCellX(px - radius);
        int maxCellX = getCellX(px + radius);
        int minCellY = getCellY(py - radius);
        int maxCellY = getCellY(py + radius);

        for (int cx = minCellX; cx <= maxCellX; ++cx) {
            for (int cy = minCellY; cy <= maxCellY; ++cy) {
                CellCoord cell{cx, cy};
                auto it = grid.find(cell);
                if (it == grid.end()) continue;

                for (int id : it->second) {
                    const Object &obj = objects.at(id);
                    double dx = obj.x - px;
                    double dy = obj.y - py;
                    if (dx * dx + dy * dy <= radiusSquared) result.push_back(id);
                }
            }
        }
        return result;
    }
};


