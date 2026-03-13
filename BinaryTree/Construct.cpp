#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 构造二叉树

// 前序与中序构造二叉树
// 前序是 中左右 中序是 左中右
// 所以前序首位一定是根 => 在中序中找到根的位置 => 左边就是左子树, 右边就是右子树, 根据左子树节点数量去前序中切分左右
TreeNode* buildTreeWithPreAndInOrder(const vector<int>& preorder, const vector<int>& inorder) {
    int n = static_cast<int>(preorder.size());
    unordered_map<int, int> pos; // 中序值对应的下标
    for (int i = 0; i < n; ++i) pos[inorder[i]] = i;

    function<TreeNode*(int, int, int, int)> dfs = [&](int pl, int pr, int il, int ir) -> TreeNode* {
        if (pl > pr or il > ir) return nullptr;

        // 前序遍历的第一个位置就是根节点
        int rootVal = preorder[pl];
        auto root = new TreeNode(rootVal);

        int k = pos[rootVal];  // 根节点在 inorder 中的位置
        int leftSize = k - il; // 左(il)|中(k)|右 => 左子树的节点个数

        // 左子树区间
        // 前序 => 中(pl)|左(pl+1...pl+leftSize)|右
        // 中序 => 左(il...k-1)|中(k)|右
        root->left = dfs(pl + 1, pl + leftSize, il, k - 1);
        root->right = dfs(pl + leftSize + 1, pr, k + 1, ir);
        return root;
    };
    return dfs(0, n - 1, 0, n - 1);
}

// 中后序构造二叉树
TreeNode* buildTreeWithInAndPostOrder(const vector<int>& inorder, const vector<int>& postorder) {
    int n = static_cast<int>(inorder.size());
    unordered_map<int, int> pos;
    for (int i = 0; i < n; ++i) pos[inorder[i]] = i;

    function<TreeNode*(int, int, int, int)> dfs = [&](int il, int ir, int pl, int pr) -> TreeNode* {
        if (il > ir or pl > pr) return nullptr;

        int rootVal = postorder[pr]; // 左右中，根节点位于区间末尾
        auto root = new TreeNode(rootVal);
        int k = pos[rootVal];
        int leftSize = k - il; // 左中右, 中在 k, 左开始于 il

        root->left = dfs(il, k - 1, pl, pl + leftSize - 1);
        root->right = dfs(k + 1, ir, pl + leftSize, pr - 1);
        return root;
    };

    return dfs(0, n - 1, 0, n - 1);
}

// 前序 + 后序不行的原因: 有些树前序后序一样, 无法确定唯一树, 根本原因是根节点位于边界, 无法清楚分开左右子树
// 特殊情况 - 满二叉树, 每个非叶子节点都有两个孩子
// 值必须不重复的原因 - 如果产生了重复值, 那么无法确定哪个才是根节点, 这就导致树无法唯一确定

// 有序数组转平衡 BST
TreeNode* build(const vector<int>& nums, int l, int r) {
    if (l > r) return nullptr;

    int mid = l + ((r - l) >> 1); // 找中间节点
    auto root = new TreeNode(nums[mid]);
    root->left = build(nums, l, mid - 1);
    root->right = build(nums, mid + 1, r);
    return root;
}

TreeNode* sortedToBST(const vector<int>& nums) {
    return build(nums, 0, static_cast<int>(nums.size()) - 1);
}

// 迭代写法
// 思路 - 每次存当前节点和节点对应的数组区间
// 然后取中点, 若左区间存在就创建左孩子并令其入队, 右侧同理
struct NodeInfo {
    TreeNode* node; // 具体的节点, 用于更新数值
    // 以这个节点为根时, 其整棵树覆盖的区间
    int l;
    int r;
};

TreeNode* sortedToBSTIterative(const vector<int>& nums) {
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
