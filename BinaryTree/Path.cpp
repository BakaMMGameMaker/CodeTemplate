#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 路径类问题
// - 是否必须从根节点出发
// - 是否必须到叶子节点结束
// - 路径上需要记载什么信息

// 路经总和 - 是否存在根到叶子的路径是的和为 target
bool hasPathSum(const TreeNode *root, int target) {
    if (not root) return false;
    // 遇到叶子节点
    if (not root->left and not root->right) return target == root->val;
    return hasPathSum(root->left, target - root->val) or hasPathSum(root->right, target - root->val);
}

// 非递归写法 1 - DFS
// 栈存当前节点与从根走到当前节点的路径和
// 如果弹出一个叶子节点且和为 target, 返回 true
// 否则压栈左右孩子并更新路径和
bool hasPathSumDFS(const TreeNode *root, int target) {
    if (not root) return false;

    vector st{pair{root, root->val}};

    while (!st.empty()) {
        auto [node, sum] = st.back();
        st.pop_back();
        if (not node->left and not node->right and sum == target) return true;

        if (node->right) st.emplace_back(node->right, sum + node->right->val);
        if (node->left) st.emplace_back(node->left, sum + node->left->val);
    }
    return false;
}

// BFS - 思路没什么区别
bool hasPathSumBFS(const TreeNode *root, int target) {
    if (not root) return false;

    deque q{pair{root, root->val}};
    while (not q.empty()) {
        auto [node, sum] = q.front();
        q.pop_front();

        if (not node->left and not node->right and sum == target) return true;
        if (node->left) q.emplace_back(node->left, sum + node->left->val);
        if (node->right) q.emplace_back(node->right, sum + node->right->val);
    }
    return false;
}
