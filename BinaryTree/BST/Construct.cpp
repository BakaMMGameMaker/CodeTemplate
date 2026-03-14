#include "../TreeNode.h"
#include <bits/stdc++.h>
using namespace std;

// 有序数组转平衡 BST
TreeNode *build(const vector<int> &nums, int l, int r) {
    if (l > r) return nullptr;

    int mid = l + ((r - l) >> 1); // 找中间节点
    auto root = new TreeNode(nums[mid]);
    root->left = build(nums, l, mid - 1);
    root->right = build(nums, mid + 1, r);
    return root;
}

TreeNode *sortedToBST(const vector<int> &nums) { return build(nums, 0, static_cast<int>(nums.size()) - 1); }

// 迭代写法
// 思路 - 每次存当前节点和节点对应的数组区间
// 然后取中点, 若左区间存在就创建左孩子并令其入队, 右侧同理
struct NodeInfo {
    TreeNode *node; // 具体的节点, 用于更新数值
    // 以这个节点为根时, 其整棵树覆盖的区间
    int l;
    int r;
};

TreeNode *sortedToBSTIterative(const vector<int> &nums) {
    if (nums.empty()) return nullptr;
    auto root = new TreeNode;
    queue<NodeInfo> q;
    q.push({root, 0, static_cast<int>(nums.size()) - 1});

    while (!q.empty()) {
        auto [node, l, r] = q.front();
        q.pop();

        int mid = l + ((r - l) >> 1); // 根节点在 nums 中对应的下标
        node->val = nums[mid];

        if (l <= mid - 1) {
            // 左区间存在
            node->left = new TreeNode;
            q.push({node->left, l, mid - 1});
        }
        if (mid + 1 <= r) {
            // 右区间存在
            node->right = new TreeNode;
            q.push({node->right, mid + 1, r});
        }
    }
    return root;
}
