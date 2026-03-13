#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 二叉树最小深度 - 根节点到最近叶子节点的最短路径上的节点数量
// 叶子节点 - 左右孩子都为空

// 递归写法
// not root => 0
// root is leaf => 1
// left is null but right => go right /
// right is null but left => go left  /
// left and right => recursive and add 1
int minDepth(const TreeNode *root) {
    if (not root) return 0;
    if (not root->left and not root->right) return 1;
    // 两者至少有一者非空
    if (not root->left) return 1 + minDepth(root->right);
    if (not root->right) return 1 + minDepth(root->left);
    return 1 + min(minDepth(root->left), minDepth(root->right));

    // 不能直接写 1 + min(f(left), f(right))
    // 因为有某个孩子为空的时候, 某一边压根没有树, 但是计算答案的时候却会被当作一个最小深度为 0 的树
}

// 非递归写法 - 层序遍历
// 思路 - 第一次遇到叶子节点的时候就是最小深度
int minDepthBFS(const TreeNode *root) {
    if (not root) return 0;
    deque q{root};
    int depth = 1;
    while (not q.empty()) {
        size_t size = q.size();

        for (int i = 0; i < size; ++i) {
            auto node = q.front();
            q.pop_front();

            if (not node->left and not node->right) return depth;
            if (node->left) q.push_back(node->left);
            if (node->right) q.push_back(node->right);
        }
        depth++;
    }
    return depth;
}
